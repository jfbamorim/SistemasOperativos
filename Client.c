// Created by Jorge Amorim on 16/11/2020.
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "Consulta.h"

// ************** VARIAVEIS GLOBAIS DE PROGRAMA ************************/
int last_signal_name;
int spid;
int pid, tpcons;
char descript[100];
// *********************************************************************/
void handle_signal(int signal){
    switch(signal){
        //C4 - Trata o sinal SIGHUP
        case SIGHUP:
            last_signal_name = SIGHUP;
            printf("Consulta iniciada para o processo %d\n", spid);
            sleep(10);
            break;
            //C5 - Trata o sinal SIGTERM se o anterior foi o SIGHUP
        case SIGTERM:
            if (last_signal_name == SIGHUP) {
                last_signal_name = SIGTERM;
                printf("Consulta concluída para o processo %d\n", spid);
                remove("pedidoconsulta.txt");
                exit(EXIT_SUCCESS);
                break;
            }
            else{
                perror("Consulta não pode terminar dado que não começou\n");
                break;
            }
            //C6 - Trata o sinal SIGUSR2 e apaga o ficheiro
        case SIGUSR2:
            last_signal_name = SIGUSR2;
            remove("pedidoconsulta.txt");
            kill(spid, SIGKILL);
            printf("Consulta não é possível para o processo %d\n", spid);
            break;
        case SIGINT:
            last_signal_name = SIGINT;
            printf("Paciente cancelou o pedido\n");
            remove("pedidoconsulta.txt");
            kill(spid, SIGKILL);
            exit(EXIT_SUCCESS);
    }
}

void escreveFicheiro(){
    FILE *fp;
    char srvpid[10];
    fp = fopen("pedidoconsulta.txt", "w+");
    if (!fp) {
        printf("Não foi possível criar o ficheiro\n");
        exit(EXIT_FAILURE);
    }

    Consulta *c;
    //C2) - Criar um elemento do tipo Consulta
    c = malloc(sizeof (Consulta));
    c->tipo = tpcons;
    strcpy(c->descricao, descript);
    c->pid_consulta = pid;

    fprintf(fp, "%d\n", c->tipo);
    fprintf(fp, "%s\n", c->descricao);
    fprintf(fp, "%d\n", c->pid_consulta);
    fclose(fp);

    //C3 - Le o PID do ficheiro srvconsultas.pid
    FILE *psrv;
    psrv = fopen("srvconsultas.txt", "r");
    if (!psrv) {
        printf("Não foi possível ler o ficheiro do lado do servidor\n");
        exit(EXIT_FAILURE);
    }

    fgets(srvpid, 10, psrv);
    spid = atoi(srvpid);

    // Envio do sinal SIGUSR1 para o Servidor
    kill(spid, SIGUSR1);


    signal(SIGHUP, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGUSR2, handle_signal);
    signal(SIGINT, handle_signal);
    while(1);

}

void handle_alarm(int signal){
    escreveFicheiro();
}

int main() {
    // C1 - Ler os dados do utilizador
    printf("Introduza o tipo de consulta:\n");
    scanf("%d", &tpcons);

    printf("Introduza a descrição:\n");
    scanf("%s", descript);

    //Criar um objeto do tipo Consulta e obter o PID
    pid = getpid();

    //C2) - Escreve as informações num novo ficheiro
    if(!(fopen("pedidoconsulta.txt", "r"))) {
        escreveFicheiro();
    }
    else{
        printf("Outro pedido de consulta a ser processado neste momento. Aguarde 10 segundos, pff.\n");
        signal(SIGALRM, handle_alarm);
        signal(SIGINT, handle_signal);
        alarm(10);
        while(1);
    }
}
