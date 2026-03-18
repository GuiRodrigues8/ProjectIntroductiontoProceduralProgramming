#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ProjetoPPP.h"


struct lista* cria_lista() {
    lista* l = malloc(sizeof(struct lista));
    if (l != NULL) {
        l->inicio = NULL;
        l->fim = NULL;
    }
    return l;
}

int data_lhe(int d, int m, int a, int h, int min, int t) {
    if (a < 2023 || d < 1 || d > 31 || m < 1 || m > 12 || h < 8 || h > 17 || (min != 0 && min != 30 ) || (t != 1 && t!= 2 ) || (t == 2 && min == 30 && h == 17)) {
        printf("Formato errado\n\n");
        return 0;
    }
    return 1;
}

struct horario* criar_horario(int prio) {
    struct horario* novo_horario = (struct horario*)malloc(sizeof(struct horario));
    if(novo_horario == NULL){
        perror("Erro ao alocar\n");
        return NULL;
    }
    int d, m, a, h, min, t;
    char s[50], s1[50];
    do {
        printf("Introduza o dia ( 1 - 31 ): \n");
        scanf("%d", &d);
        printf("Introduza o mês ( 1 - 12 ): \n");
        scanf("%d", &m);
        printf("Introduza o ano: \n");
        scanf("%d", &a);
        printf("Introduza a hora ( 8 - 18h ): \n");
        scanf("%d", &h);
        printf("Introduza os minutos ( 0 ou 30 ): \n");
        scanf("%d", &min);
        printf("Tipo de reserva: \n 1 - Lavagem \n 2 - Manutenção\n");
        scanf("%d", &t);
        printf("Introduza o nome: \n");
        getchar();
        scanf("%s", s);
        printf("Introduza o apelido: \n");
        getchar();
        scanf("%s", s1);

    } while (!data_lhe(d, m, a, h, min, t));
    if(t == 1){
        novo_horario->tempo = 30; //duração da lavagem
        novo_horario->tipo = malloc(strlen("Lavagem") + 1);
        strcpy(novo_horario->tipo, "Lavagem");
    }else if(t == 2){
        novo_horario->tempo = 100; // devido ao formato que usamos para guardar as horas, a duração tem de ser de 100 minutos
        novo_horario->tipo = malloc(strlen("Manutenção") + 1);
        strcpy(novo_horario->tipo, "Manutenção");
    }
    if(prio == 0){
        novo_horario->prio = 0;
    }else {
        novo_horario->prio = prio;
    }
    novo_horario->nome = malloc(strlen(s) + 1);
    novo_horario->apelido = malloc(strlen(s1) + 1);
    strcpy(novo_horario->nome, s);
    strcpy(novo_horario->apelido, s1);
    novo_horario->data = a * 10000 + m * 100 + d;
    novo_horario->hora = h * 100 + min;
    return novo_horario;
}



int inserir_horario(lista* lista_reservas, struct horario* novo_horario, int temp2) {
    no* aux = lista_reservas->inicio;
    if (temp2 == 0) {
        while (aux != NULL) { //percorre a lista toda, se houver uma data igual, avisa
            if (aux->horario->data == novo_horario->data &&((aux->horario->hora >= novo_horario->hora && aux->horario->hora < novo_horario->hora + novo_horario->tempo) ||(novo_horario->hora >= aux->horario->hora && novo_horario->hora < aux->horario->hora + aux->horario->tempo))) {
                printf("Data não disponível.\n");
                return 1;      //O if verifica todas as condições possíveis onde possa haver conflito de horário;
            }
            aux = aux->prox;
        }
    }

    no* novo_no = (no*)malloc(sizeof(no));
    if (novo_no == NULL) {
        perror("Erro ao alocar\n"); //Confirmar a alocação do novo_no
        return -1;
    }

    novo_no->horario = novo_horario;
    novo_no->prox = NULL;

    if (lista_reservas->inicio == NULL) {  //inserir na lista vazia
        lista_reservas->inicio = novo_no;
        lista_reservas->fim = novo_no;
    } else if (novo_horario->data <= lista_reservas->inicio->horario->data && novo_horario->hora <= lista_reservas->inicio->horario->hora) {
        // inserir no início da lista
        novo_no->prox = lista_reservas->inicio;
        lista_reservas->inicio = novo_no;
    } else {
        aux = lista_reservas->inicio;
        while (aux->prox != NULL && (aux->prox->horario->data < novo_horario->data || (aux->prox->horario->data == novo_horario->data && aux->prox->horario->hora < novo_horario->hora))) {
            aux = aux->prox;
        }
        novo_no->prox = aux->prox;
        aux->prox = novo_no;
    }

    printf("Reserva criada com sucesso!!\n");
    return 0;
}

void cancelar_horario(lista* lista_reservas, struct horario* horario) {
    no* atual = lista_reservas->inicio;
    no* ant = NULL;
 //procura determinada data dum horario nas reservas e apaga
    while (atual != NULL) {
        if (atual->horario->data == horario->data && atual->horario->hora == horario->hora) {
            if (ant == NULL) {
                lista_reservas->inicio = atual->prox;
            } else {
                ant->prox = atual->prox;
            }

            free(atual);
            free(atual->horario->apelido);
            free(atual->horario->nome);
            free(atual->horario);
            printf("Reserva cancelada com sucesso!!\n");
            return;
        }

        ant = atual;
        atual = atual->prox;
    }

    printf("Reserva não existe.\n");
}

void concluir_reserva(lista* l, struct horario* horario) {
    no* atual = l->inicio;

    while (atual != NULL && (atual->horario->data < horario->data || (atual->horario->data == horario->data && atual->horario->hora <= horario->hora))) {
        no* temp = atual;

        l->inicio = atual->prox;
        atual = atual->prox;
        free(temp->horario->apelido);
        free(temp->horario->nome);
        free(temp->horario->tipo);
        free(temp->horario);
        free(temp);
    }
}

void ordena_decrescente(lista* l) {   
    no* atual = l->inicio;
    no* temp = NULL;  
    int aux;  
       
    while(atual != NULL) {   
        temp = atual->prox;  
        while(temp != NULL) {  
            if(atual->horario->data > temp->horario->data || (atual->horario->data == temp->horario->data && atual->horario->hora > temp->horario->hora)) {  
                aux = atual->horario->data;  
                atual->horario->data = temp->horario->data;  
                temp->horario->data = aux;  
            }  
            temp = temp->prox;  
        }  
        atual = atual->prox;  
    }      
}  


void ordena_crescente(lista* l) {   
    no* atual = l->inicio;
    no* temp = NULL;  
    int aux;

    while(atual != NULL) {   
        temp = atual->prox;  
        while(temp != NULL) {  
            if(atual->horario->data > temp->horario->data || (atual->horario->data == temp->horario->data && atual->horario->hora > temp->horario->hora)) {  
                aux = atual->horario->data;  
                atual->horario->data = temp->horario->data;  
                temp->horario->data = aux;  
            }  
            temp = temp->prox;  
        }  
        atual = atual->prox;  
    }      
}  


void ordena_prio(lista* l) {   
    no* atual = l->inicio;
    no* temp = NULL;  
    int aux;  

    while(atual != NULL) {   
        temp = atual->prox;  
        while(temp != NULL) {  
            if(atual->horario->prio > temp->horario->prio) {  
                aux = atual->horario->data;  
                atual->horario->data = temp->horario->data;  
                temp->horario->data = aux;  
            }  
            temp = temp->prox;  
        }  
        atual = atual->prox;  
    }      
}  


void imprime_horario(struct horario* horario) {
    if(horario == NULL){
        return;
    }
    int a, m, d, h, min;
    d = horario->data % 100;
    m = (horario->data / 100) % 100;
    a = horario->data / 10000;
    h = horario->hora / 100;
    min = horario->hora % 100;

    printf("%d/%d/%d %02d:%02d -> %s %s -> %s %d\n", d, m, a, h, min, horario->nome, horario->apelido, horario->tipo, horario->prio);
}


void imprime_lista(struct lista* l) {
    struct no* aux = l->inicio;
    while (aux != NULL) {
        imprime_horario(aux->horario);
        aux = aux->prox;
    }
}

void reserva_cliente(char* nome, char* apelido, lista* l) {
    no* atual = l->inicio;
    int n = 0;
    while (atual != NULL) {
        struct horario* horario = atual->horario;
        if (strcmp(horario->nome, nome) == 0 && strcmp(horario->apelido, apelido) == 0) {
            imprime_horario(horario);
            n = 1;
        }
        atual = atual->prox;
    }
    if( n == 0){
        printf("Cliente não encontrado\n");
    }
}

void guardar(lista* reservas, lista* prereservas){
    no* aux = reservas->inicio;
    no* aux1 = prereservas->inicio;
    char ficheiro[100];
    printf("Digite o nome do ficheiro onde quer guardar: ");
    scanf("%100s", ficheiro);
    FILE *f1 = fopen(ficheiro, "r");
    int a = 0;
    if(f1 != NULL ){
        printf("Cuidado!! Dados já presentes no ficheiro serão apagados: \n");
        do
        {
            printf("Deseja continuar? \n1 - Sim \n2 - Não \n");
            scanf("%d", &a);
            if(a != 1 && a != 2){
                printf("Opção inválida\n");
            }
            else if( a == 2){
                fclose(f1);
                return;
            }
        } while (a != 1 && a != 2);
    }


    if(aux == NULL && aux1 == NULL){
        printf("Não existem dados para gravar\n");
        fclose(f1);
        return;
    }

    fclose(f1);
    FILE *f = fopen(ficheiro, "w");
    fprintf(f, "RESERVAS ------- \n\n\n");
    while(aux != NULL){
        fprintf(f, "Data:\n");
        fprintf(f, "%d\n", aux->horario->data);
        fprintf(f, "Hora:\n");
        fprintf(f, "%d\n", aux->horario->hora);
        fprintf(f, "Tipo:\n");
        fprintf(f, "%s\n", aux->horario->tipo);
        fprintf(f, "Nome:\n");
        fprintf(f, "%s\n", aux->horario->nome);
        fprintf(f, "Apelido:\n");
        fprintf(f, "%s\n\n", aux->horario->apelido);

        aux = aux->prox;
    }

    fprintf(f, "PRE-RESERVAS ---- \n\n\n");

    while(aux1 != NULL){
        fprintf(f, "Data:\n");
        fprintf(f, "%d\n", aux1->horario->data);
        fprintf(f, "Hora:\n");
        fprintf(f, "%d\n", aux1->horario->hora);
        fprintf(f, "Tipo:\n");
        fprintf(f, "%s\n", aux1->horario->tipo);
        fprintf(f, "Nome:\n");
        fprintf(f, "%s\n", aux1->horario->nome);
        fprintf(f, "Apelido:\n");
        fprintf(f, "%s\n\n", aux1->horario->apelido);

        aux1 = aux1->prox;
    }

    printf("Dados guardados com sucesso");
    fclose(f);
   
}


void carregar(lista* reservas, lista* prereservas){
    char ficheiro[100];
    char linha[100];
    int data, hora, a;
    char nome[50], apelido[50], tipo[50];
    printf("Digite o nome do ficheiro que quer carregar: ");
    scanf("%100s", ficheiro);
    FILE *f = fopen(ficheiro, "r");
    if(f == NULL ){
        printf("Não existem dados para ser carregados: \n");
        fclose(f);
        return;
    }

    struct horario* novo_horario = NULL;

    while(fgets(linha, sizeof(linha), f) != NULL){
        if (strcmp(linha, "RESERVAS ------- \n") == 0) { //quando encontra isto começa a guardar na lista das reservas
            a = 1;
        }else if (strcmp(linha, "PRE-RESERVAS ---- \n") == 0) { //quando encontra isto começa a guardar na lista das pre reservas
            a = 2;
        }else if (strlen(linha) == 0){
            a = 0;
        }

        if(a != 0){
           if (strcmp(linha, "Data:\n") == 0) { //quando encontrar determinada string na linha, lê a linha seguinte e guarda a em determinado horario
                novo_horario = (struct horario*)malloc(sizeof(struct horario));
                fgets(linha, sizeof(linha), f);
                sscanf(linha, "%d", &data);
                novo_horario->data = data;
                
            } else if (strcmp(linha, "Hora:\n") == 0) {
                fgets(linha, sizeof(linha), f);
                sscanf(linha, "%d", &hora);
                novo_horario->hora = hora;

            } else if (strcmp(linha, "Tipo:\n") == 0) {
                fgets(linha, sizeof(linha), f);
                sscanf(linha, "%s", tipo);
                novo_horario->tipo = malloc(strlen(tipo) + 1);
                strcpy(novo_horario->tipo, tipo);

            } else if (strcmp(linha, "Nome:\n") == 0) {
                fgets(linha, sizeof(linha), f);
                sscanf(linha, "%s", nome);
                novo_horario->nome = malloc(strlen(nome) + 1);
                strcpy(novo_horario->nome, nome);

            } else if (strcmp(linha, "Apelido:\n") == 0) {
                fgets(linha, sizeof(linha), f);
                sscanf(linha, "%s", apelido);
                novo_horario->apelido = malloc(strlen(apelido) + 1);
                strcpy(novo_horario->apelido, apelido);
                if(a == 1){
                    inserir_horario(reservas, novo_horario, 0);
                }else if(a == 2){
                    inserir_horario(prereservas, novo_horario, 1);
                }
                novo_horario = NULL;
            }
        }

    }
    fclose(f);
    if (novo_horario != NULL) {
        free(novo_horario->apelido);
        free(novo_horario->nome);
        free(novo_horario->tipo);
        free(novo_horario);
    }
}



void destroi_lista(struct lista *l){ //limpa a lista e tudo o que está lá dentro até esta estar vazia
    while(l->inicio != NULL){
        struct no* temp = l->inicio;
        l->inicio = l->inicio->prox;
        free(temp->horario->apelido);
        free(temp->horario->nome);
        free(temp->horario->tipo);
        free(temp->horario);
        free(temp);
    }
    free(l);
}
