#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "Consulta.h"

// ************** VARIAVEIS GLOBAIS DE PROGRAMA ********************************************/
Consulta lista_consultas[10];
int consultasNormal, consultasCOVID, consultasUrgente, consultasPerdidas, ultimaConsulta=0;
// *****************************************************************************************/

//método para contar o numero de consultas por sessão
void contatipoconsulta(int itipo){
    switch(itipo){
        case 1:
            consultasNormal++;
            break;
        case 2:
            consultasCOVID++;
            break;
        case 3:
            consultasUrgente++;
            break;
    }
}
//modulo para criação e/ou atualização do ficheiro statsconsultas.dat
void adminencerra(){
    //Ler o ficheiro anterior e recuperar os dados
    //e somar os novos valores, para atualizar o ficheiro
    /*FILE * fp;
    remove("srvconsultas.txt");
    fp = fopen("statsconsultas.dat", "w");
    if (fp){
        fprintf("Perdidas: %d", consultasPerdidas);
        fprintf("Tipo 1: %d", 0);
        fprintf("Tipo 2: %d", 0);
        fprintf("Tipo 3: %d", 0);
        fclose(fp);
    }*/
    exit(EXIT_SUCCESS);
}

//método para inicio de consulta, agregando todos os dados necessarios e validando se é possivel iniciar a consulta
//envia os sinais necessários ao cliente e cria processo filho através do fork
void iniciaConsulta(){
    int ctipo, cpid;
    char cdesc[100], chartipo[1], charpid[10];
    FILE * fp;
    //S3.1) Lê a informação do ficheiro PedidoConsulta.txt
    fp = fopen("pedidoconsulta.txt", "r");
    if (fp == NULL) {
        perror("fopen()");
    }
    //preencher as variaveis para colocar no output
    fgets(chartipo, 10, fp);
    fgets(cdesc, 100, fp);
    fgets(charpid, 10, fp);

    fclose(fp);

    ctipo = atoi(chartipo);
    cpid = atoi(charpid);

    //S3.2) Escreve no ecrã a mensagem
    printf("Chegou novo pedido de consulta do tipo %d, descricao %s e PID %d\n", ctipo, cdesc, cpid);

    //S3.3) Verifica se a Lista de Consultas tem alguma “vaga”.
    if(ultimaConsulta > 10){
        printf("Lista de consultas cheia\n");
        consultasPerdidas+=1;
        kill(cpid, SIGUSR2);
    }
    else{
        //Contagem do tipo de consulta
        contatipoconsulta(ctipo);
        lista_consultas[ultimaConsulta].tipo = ctipo;
        strcpy(lista_consultas[ultimaConsulta].descricao, cdesc);
        lista_consultas[ultimaConsulta].pid_consulta = cpid;
        printf("Consulta agendada para a sala %d\n", ultimaConsulta++);

        contatipoconsulta(ctipo);

        int child = fork();
        if(child == 0){
            kill(cpid, SIGHUP);
            sleep(10);
            printf("Consulta terminada na sala %d\n", ultimaConsulta-1);
            kill(cpid, SIGTERM);
            exit(EXIT_SUCCESS);
        }
        else{
            wait(NULL);
            //Apagar o registo da consulta do array, decrementando o valor da ultima consulta
            ultimaConsulta--;
        }
    }
}

// inicializador do Servidor
void init(){
    int contador;
    //S1) - Inicia a lista de consultas com o campo tipo = –1
    for(contador=1; contador < 10; ++contador){
        lista_consultas[contador].tipo = -1;
    }

    //Inicialização das variaveis do Servidor
    consultasNormal = 0;
    consultasCOVID = 0;
    consultasUrgente = 0;
    consultasPerdidas = 0;
    ultimaConsulta = 0;

    //S2) - Regista o PID do seu processo no ficheiro SrvConsultas.pid
    int pid = getpid();
    if (pid < 0){
        perror("Erro");
        exit(EXIT_FAILURE);
    }
    if(pid!=0){
        FILE *srvc;
        srvc = fopen("srvconsultas.txt", "w+");
        fprintf(srvc, "%d", pid);
        fclose(srvc);

        printf("O Servidor encontra-se à espera da próxima consulta..\n");
    }
}
int main(){
    init();
    //S3) - Arma e trata o sinal SIGUSR1
    signal(SIGUSR1, iniciaConsulta);
    signal(SIGINT, adminencerra);
    while(1);
}
