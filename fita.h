#ifndef FITA_H_
#define FITA_H_

#define FITAS 20 		 /* quantidade de fitas disponivel */
#define TAM_BLOCO 500000 /* quantidade de blocos máxima por fita */

#include "area.h"

	typedef struct fita Fita;

/* cria um vetor com metade das fitas totais dinamicamente e o retorna */
Fita* cria_fita();

/* fechar deleta lógica e fisicamente as fitas criadas */
void apaga_fita(Fita* disk, int marcador);

/* zera todas as fitas */
void zera_fita(Fita* disk);

/* cria um arquivo fisico e o atribui a uma fita */
void set_arquivo(Fita* disk, int nome);

/* retorna um arquivo presente em uma fita */
FILE* get_file(Fita* disk);

/* verifica se a fita está ativa */
short estado_da_fita(Fita* disk);

/* reativa fitas com condicional */
void set_ativa(Fita* disk);

/* grava area em fita */
void salva_em_fita(Fita* disk, TipoArea* Area, Analise* estatistica);


/* MÉTODOS DE ORDENAÇÃO QUE UTILIZAM FITAS */

/* cria blocos ordenados internamente */
void criacao_de_blocos(FILE** principal, Fita* entrada, Fita* saida, long total, Analise* estatistica); //total vai ser argv
void criacao_de_blocos_aux(Fita* entrada, FILE* principal, long total, Analise* estatistica);

/* cria blocos ordenados com seleção */
void substituicao_selecao(FILE** principal, Fita* entrada, Fita* saida, long total, Analise* estatistica);
void substituicao_selecao_aux(Fita* entrada, FILE* principal, long total, Analise* estatistica);

/* método que intercala os blocos ordenados previamente para ordenação */
int intercalacao_balanceada(Fita* entrada, Fita* saida, Analise* estatistica);

/* conta quantos blocos tem nas fitas de entrada */
int contador_blocos(Fita* disk);

/* faz uma leitura no arquivo especificado */
int leitura(Fita* disk, Registro* A, int qual_bloco, Analise* estatistica);

/* faz uma escrita no arquivo especificado */
void escrita(Fita* disk, Registro A, Analise* estatistica);

/* verifica dentro da área qual é o menor elemento e retorna seu indice */
int procura_menor(TipoArea Area, Analise* estatistica);

/* verifica a quantidade de registros ainda presentes no bloco */
int verifica_tam_bloco(Fita* disk);

/* verifica a quantidade de blocos presentes no arquivo da fita */
int verifica_bloco(Fita* disk);

/* retira todos os elementos ordenadamente da área e grava em arquivo */
void esgota_area(TipoArea* Area, Fita* entrada, int qual_fita, Analise* estatistica);


#endif //FITA_H_