#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ProjetoPPP.h"

int main() {
    int v;
    int prio = 0;
    lista* reservas = cria_lista();
    lista* prereservas = cria_lista();

    do {
        printf("\n 1 - Reserva");
        printf("\n 2 - Cancelar Reserva");
        printf("\n 3 - Cancelar Pré-Reserva");
        printf("\n 4 - Concluir Marcação");
        printf("\n 5 - Lista por Data");
        printf("\n 6 - Lista por Cliente");
        printf("\n 7 - Guardar Dados");
        printf("\n 8 - Carregar Dados");
        printf("\n 9 - Cancelar Operação ");
        printf("\n Introduza a sua escolha: \n");
        scanf("%d", &v);

        if (v == 1) {
            int temp2 = 0;
            struct horario* info = criar_horario(prio);
            if (inserir_horario(reservas, info, temp2) == 1) {
                ++prio;
                int temp;
                do {
                    printf("Quer fazer uma pré-reserva ?\n 1 - Sim \n 2 - Não \n");
                    scanf("%d", &temp);
                    if (temp == 1) {
                        temp2 = 1;
                        inserir_horario(prereservas, info, temp2);
                    } else if (temp != 2 && temp != 1) {
                        printf("Opção Inválida\n");
                    } 
                } while (temp != 1 && temp != 2);

            }
        } else if (v == 2) {
            struct horario* horario = criar_horario(0);
            cancelar_horario(reservas, horario);
            ordena_prio(prereservas);
            if (prereservas->inicio != NULL) {
                no* atual = prereservas->inicio;
                no* ant = NULL;
                while (atual != NULL) {
                    if(inserir_horario(reservas, atual->horario, 0) == 0) {
                        if (ant == NULL) {
                            prereservas->inicio = atual->prox;
                        } else {
                            ant->prox = atual->prox;
                        }
                        no* temp = atual;
                        atual = atual->prox;
                        free(temp);
                        printf("A reserva livre encontrada foi preenchida com a pré reserva\n");
                    }else{
                    ant = atual;
                    atual = atual->prox;
                    }
                }
            }
        
        } else if (v == 3) {
            struct horario* horario = criar_horario(0);
            cancelar_horario(prereservas, horario);
        } else if (v == 4) {
            struct horario* horario = criar_horario(0);
            concluir_reserva(reservas, horario);
            concluir_reserva(prereservas, horario);
        } else if (v == 5) {

            printf("Reservas -------------\n");
            ordena_crescente(reservas);
            imprime_lista(reservas);
            printf("\nPre-Reservas ---------\n");
            ordena_crescente(prereservas);
            imprime_lista(prereservas);

        } else if (v == 6) {

            ordena_decrescente(reservas);
            ordena_decrescente(prereservas);
            char nome1[50];
            char apelido1[50];
            printf("Introduza o Nome e o Apelido: \n");
            getchar();
            scanf("%s", nome1);
            scanf("%s", apelido1);
            printf("Reservas --------\n");
            reserva_cliente(nome1, apelido1, reservas);
            printf("Pre-Reservas ----\n");
            reserva_cliente(nome1, apelido1, prereservas);

        }else if (v == 7){

            guardar(reservas, prereservas); //guarda num ficheiro

        }else if (v == 8){

            carregar(reservas, prereservas); //carrega as reservas e pre reservas num ficheiro

        }else if (v == 9) {

            printf("\nOperação Cancelada,\nObrigado!\n");

        } else {

            printf("\nOpção Inválida\n");
        }

    } while (v != 9);

    destroi_lista(reservas); //destroi as listas das reservas para libertar memoria
    destroi_lista(prereservas); //destroi as listas da pre reserva para libertar memoria
    return 0;
}


