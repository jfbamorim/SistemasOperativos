// Created by Jorge Amorim on 17/11/2020.
typedef struct {
    int tipo;               // Tipo de Consulta: 1-Normal, 2-COVID19, 3-Urgente
    char descricao[100];    // Descrição da Consulta
    int pid_consulta;       // PID do processo que quer fazer a consulta
} Consulta;
