#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include "registro.h"
#include "area.h"

/* ------------------ QUICK SORT ------------------ */

/* método principal que faz gerenciamento das partes do arquivo a ser ordenado */
void QuicksortExterno(FILE** ArqLi , FILE** ArqEi , FILE** ArqLEs, int Esq, int Dir, Analise* estatistica);

/* faz leitura na parte superior do arquivo */
void LeSup(FILE** ArqLEs, Registro* UltLido, int* Ls, short* OndeLer, Analise* estatistica);

/* faz leitura na parte inferior do arquivo */
void LeInf(FILE** ArqLi ,Registro* UltLido, int* Li ,short* OndeLer, Analise* estatistica);

/* insere elemento lido em área ordenado */
void InserirArea(TipoArea* Area, Registro* UltLido, int* NRArea, Analise* estatistica);

/* escreve elemento passado por parâmento em arquivo */
void EscreveMax(FILE** ArqLEs, Registro A, int* Es, Analise* estatistica);

/*escreve elemento passado por parâmetro em arquivo */
void EscreveMin(FILE** ArqEi, Registro A, int* Ei, Analise* estatistica);

/* retira o maior elemento (ultimo) da area */
void RetiraMax(TipoArea* Area, Registro* A, int* NRArea);

/* retira o menor elemento (primeiro) da area */
void RetiraMin(TipoArea* Area, Registro* A, int* NRArea);

/* função que faz manipula todas as outras para ordenação */
void Particao(FILE** ArqLi, FILE** ArqEi, FILE** ArqLEs, TipoArea Area, int Esq, int Dir, int* i, int* j, Analise* estatistica);

/* ------------------ QUICK SORT ------------------ */





#endif //QUICKSORT_H_