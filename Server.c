#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "Consulta.h"

Consulta lista_consultas[10];
int consultasNormal, consultasCOVID, consultasUrgente, consultasPerdidas, ultimaConsulta=0;

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

void adminencerra(){
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

void iniciaConsulta(){
    int ctipo, cpid;
    char cdesc[100], chartipo[1], charpid[10];
    FILE * fp;
    fp = fopen("pedidoconsulta.txt", "r");
    if (fp == NULL) {
        perror("fopen()");
    }
    //preencher as variaveis para colocar no output
    fgets(chartipo, 10, fp);
    fgets(cdesc, 100, fp);
    fgets(charpid, 10, fp);

    ctipo = atoi(chartipo);
    cpid = atoi(charpid);

    printf("Chegou novo pedido de consulta do tipo %d, descricao %s e PID %d\n", ctipo, cdesc, cpid);

    //S.3.2 - Lista cheia e envia sinal ao Cliente
    if(ultimaConsulta >= 9){
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
        int child = fork();
        if(child == 0){
            kill(cpid, SIGHUP);
            sleep(10);
            printf("Consulta terminada na sala %d\n", ultimaConsulta-1);
            kill(cpid, SIGTERM);
            exit(0);
        }
        else{
            child = wait(NULL);
        }
    }
}

void init(){
    int contador;
    for(contador=1; contador < 10; ++contador){
        lista_consultas[contador].tipo = -1;
    }

    //Inicialização das variaveis do Servidor
    consultasNormal = 0;
    consultasCOVID = 0;
    consultasUrgente = 0;
    consultasPerdidas = 0;
    ultimaConsulta = 0;

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
    while(1){
        signal(SIGUSR1, iniciaConsulta);
        signal(SIGINT, adminencerra);
    }
}
