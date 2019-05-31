#ifndef AREA_H_
#define AREA_H_

#include "registro.h"
#include "analise.h"

#define MEM_INT 10 /* tamanho da memória interna disponivel */

typedef struct tipoarea TipoArea;

/* cria uma area */
TipoArea cria_area();

/* inicializa a área vazia */
void FAVazia(TipoArea *Area);

/* verifica se ainda há posição vazia na área */
int Evazia(tipoarea *Area);

/* retorna número de registros em área */
int ObterNumCelOcupadas(TipoArea *Area);

/* retorna registro presente em area na posição desejada */
Registro retorna_registro(TipoArea* Area, int posicao);

/* Insere um item na area dependendo da necessidade ele ativa a posição ou ordena a própria area */
void InsereItem(Registro A, TipoArea *Area, int indice_posicao, int ordena_ativa, Analise* estatistica);

/* retira primeiro registro da area */
void RetiraPrimeiro(TipoArea *Area, Registro *A);

/* retira o último registro da area */
void RetiraUltimo(TipoArea *Area, Registro *A);

/* função retorna mesmo numero 10x maior (não altera conteudo, apenas para fim de comparação) */
float maior_de_todos(float nota, int marcado);

/* verifica se ainda ha elementos não marcados na fita */
int fim_de_fita(TipoArea* Area);

/* insere um item na primeira prosição e verifica o heap */
void insercao_heap(TipoArea* Area, Registro entra, Registro saiu, Analise* estatistica);

/* troca a posicao do pai com a do filho e seus respectivos campos */
void troca(TipoArea* Area, int pai, int filho);

/* METODOS PARA ORDENAÇÃO INTERNA DA AREA */

/* funções para execução de um heapsort */
void heapsort(TipoArea* Area, int tam, Analise* estatistica);
void heap_constroi(TipoArea* Area, int tam, Analise* estatistica);
void heap_refaz(TipoArea* Area, int esq, int dir, Analise* estatistica);

/* funções para execuções de um quicksort interno */
void quickSort(TipoArea* A, int n, Analise* estatistica);
void quickSort_ordena(TipoArea* A, int esq, int dir, Analise* estatistica);
void quickSort_particao(TipoArea* A, int esq, int dir, int* i, int* j, Analise* estatistica);

#endif //AREA_H_