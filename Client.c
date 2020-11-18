// Created by Jorge Amorim on 16/11/2020.
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "Consulta.h"

char *last_signal_name;
int spid;

void handle_alarm(){
    printf("Handle alarm");
}

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
                kill(spid, SIGKILL);
                printf("Consulta concluída para o processo %d\n", spid);
                break;
            }
            else{
                printf("%d, %d", last_signal_name, spid);
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
            break;
    }
}

int main() {
    //Passar para variaveis globais - por causa do alarme
    int pid, tpcons;
    char descript[100], srvpid[10];
    FILE *fp;
    Consulta *p;

    // C1 - Ler os dados do utilizador
    printf("Introduza o tipo de consulta:\n");
    scanf("%d", &tpcons);

    printf("Introduza a descrição:\n");
    scanf("%s", descript);

    //Criar um objeto do tipo Consulta e obter o PID
    pid = getpid();

    //C2 - Criar um elemento do tipo Consulta
    p = malloc(sizeof (Consulta));
    p->tipo = tpcons;
    strcpy(p->descricao, descript);
    p->pid_consulta = pid;

    //C2 - Escreve as informações num novo ficheiro
    FILE *ftest;
    if(!(fopen("pedidoconsulta.txt", "r"))) {
        fp = fopen("pedidoconsulta.txt", "w+");

        if (!fp) {
            printf("Não foi possível criar o ficheiro\n");
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "%d\n", p->tipo);
        fprintf(fp, "%s\n", p->descricao);
        fprintf(fp, "%d\n", p->pid_consulta);
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

        while (1) {
            signal(SIGHUP, handle_signal);
            signal(SIGTERM, handle_signal);
            signal(SIGUSR2, handle_signal);
            signal(SIGINT, handle_signal);
        }
    }
    else{
        perror("Outro pedido de consulta a ser processado neste momento. Aguarde 10 segundos, pff.\n");
        signal(SIGALRM, handle_alarm);
        alarm(10);
    }
}
