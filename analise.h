#ifndef ANALISE_H_
#define ANALISE_H_

	/* nome da estrutura que tratará da analise de dados */
	typedef struct analise Analise;

/* cria, aloca a estrutura dinamicamente e a retorna */
Analise* cria_analise();

/* deleta a estrutura passada como paramentro dinamicamente */
void deleta_analise(Analise* A);

/* contabiliza o número de transferências entre memória (externa/interna) feitas pelo método */
void t_hit(Analise* A);

/* contabiliza o número de comparações de chaves feitas pelos métodos */
void c_hit(Analise* A);

/* salva o time INICIO de execução de um determinado método */
void start_time(Analise* A);

/*salva o time FIM de execução de um determinado método e faz a contabilidade */
void stop_time(Analise* A);

/*método auxiliar que faz a conversão do time para ms */
void time_hit(Analise* A);

/* imprime a analise de dados completa na tela */
void mostrar_est(Analise* A);

/*deixa default todos os campos da estrutura */
void zera_analise(Analise* A);


#endif