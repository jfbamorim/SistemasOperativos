#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

typedef struct {
    int tipo;               // Tipo de Consulta: 1-Normal, 2-COVID19, 3-Urgente
    char descricao[100];    // Descrição da Consulta
    int pid_consulta;       // PID do processo que quer fazer a consulta
} Consulta;

char comecaConsulta;
const char *last_signal_name;

void inicioConsulta(int sinalID){
    comecaConsulta = 1;
}
void handle_signal(int signal, int pidcons){
    switch(signal){
        //C4 - Trata o sinal SIGHUP
        case SIGHUP:
            last_signal_name = "SIGHUP";
            printf("Consulta iniciada para o processo %d", pidcons);
            sleep(10);
            break;
        //C5 - Trata o sinal SIGTERM se o anterior foi o SIGHUP
        case SIGTERM:
            if (last_signal_name == 'SIGHUP') {
                last_signal_name = 'SIGTERM';
                kill(pidcons, SIGKILL);
                printf("Consulta concluída para o processo %d", pidcons);
            }
            else{
                perror("Consulta não pode terminar dado que não começou");
            }
            break;
        //C6 - Trata o sinal SIGUSR2 e apaga o ficheiro
        case SIGUSR2:
            last_signal_name = 'SIGUSR2';
            remove("pedidoconsulta.txt");
            kill(pidcons, SIGKILL);
            printf("Consulta não é possível para o processo %d", pidcons);
            break;
        case SIGINT:
            last_signal_name = 'SIGINT';
            printf("Paciente cancelou o pedido");
            remove("pedidoconsulta.txt");
            kill(pidcons, SIGKILL);
            break;
        default:
            return;
    }
}

int main() {
    int pid, tpcons, cpid;
    char descript[100];
    FILE *fp;
    Consulta *p;

    // C1 - Ler os dados do utilizador
    printf("Introduza o tipo de consulta:\n");
    scanf("%d", &tpcons);

    printf("Introduza a descrição:\n");
    scanf("%s", descript);

    //Apresentar os dados inseridos
    printf("Os dados inseridos foram:\n");
    printf("Tipo de Consulta: %d\n", tpcons);
    printf("Descrição:%s\n", descript);

    //Criar um objeto do tipo Consulta e obter o PID
    pid = fork();
    if (pid == -1){
        cpid = getpid();
    }

    //C2 - Criar um elemento do tipo Consulta
    p = malloc(sizeof (Consulta));
    p->tipo = tpcons;
    strcpy(p->descricao, descript);
    p->pid_consulta = pid;

    //C2 - Escreve as informações num novo ficheiro
    fp = fopen ("pedidoconsulta.txt", "w+");
    if(!fp){
        printf("Não foi possível criar o ficheiro");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d;%s;%d", p->tipo, p->descricao, p->pid_consulta);
    fclose(fp);

    //C3 - Le o PID do ficheiro srvconsultas.pid
    FILE *psrv;
    psrv = fopen("srvconsultas.pid", "r");
    if(!psrv){
        printf("Não foi possível ler o ficheiro do lado do servidor");
        exit(EXIT_FAILURE);
    }
    int srvpid = fgetc(psrv);
    signal(SIGUSR1, inicioConsulta);
    return EXIT_SUCCESS;
}