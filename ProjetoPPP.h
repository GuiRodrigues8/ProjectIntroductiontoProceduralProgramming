#ifndef PROJETOPPP_H
#define PROJETOPPP_H

struct horario {
    int data;
    int hora;
    int tempo;
    int prio;
    char* nome;
    char* apelido;
    char* tipo;
};

typedef struct no {
    struct horario* horario;
    struct no* prox;
} no;

typedef struct lista {
    struct no* inicio;
    struct no* fim;
} lista;

struct lista* cria_lista();

int data_lhe(int d, int m, int a, int h, int min, int t); 

struct horario* criar_horario(int prio);

int inserir_horario(lista* lista_reservas, struct horario* novo_horario, int temp2);

void cancelar_horario(lista* lista_reservas, struct horario* horario);

void concluir_reserva(lista* l, struct horario* horario);

void ordena_decrescente(lista* l);

void ordena_crescente(lista* l);

void ordena_prio(lista* l);

void imprime_horario(struct horario* horario);

void imprime_lista(struct lista* l);

void reserva_cliente(char* nome, char* apelido, lista* l);

void guardar(lista* reservas, lista* prereservas);

void carregar(lista* reservas, lista* prereservas);

void destroi_lista(struct lista *l);


#endif