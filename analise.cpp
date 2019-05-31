#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

#include "analise.h"

using namespace std;

	/*estrutura analise e seus campos */
	struct analise
	{
		long transf; 	/* guardará o número de transferências feitas pela memória (interna/externa)*/
		long comp; 		/* guardará o número de comparações de chave */
		double time; 	/* guardará o tempo gasto para execução de um método */
		clock_t inicio; /* guardará o inicio da execução de um método */
		clock_t fim;	/* guardará o fim da execução de um método */	
	};

/*cria aloca dinamicamente e retorna uma estrutura analise dinamicamente */
Analise* cria_analise()
{
	Analise* A;
	
	A = new Analise;

	/*zera todos os indices da estrutura */
	zera_analise(A);

	/* retornando alocação devida */
	return A;
}

/*deleta a estrutura passada por paramentro dinamicamente */
void deleta_analise(Analise* A)
{
	delete A;
}

/* acrescenta +1 a cada vez que o método fizer uma transferência entre memórias (interna/externa)*/
void t_hit(Analise* A)
{
	A->transf++;
}

/* acrescenta +1 a cada vez que o método fizer uma comparação de chave */
void c_hit(Analise* A)
{
	A->comp++;
}

/* salva o clock inicial de execução de um método */
void start_time(Analise* A)
{
	A->inicio = clock();
}
/* salva o clock final de execução de um método */
void stop_time(Analise* A)
{
	A->fim = clock();
	time_hit(A); //faz a conversão 
}

/* método de conversão para ms do tempo gasto para executar um método */
void time_hit(Analise* A)
{
	A->time = (A->fim - A->inicio)*1000.0/CLOCKS_PER_SEC;
}

/* imprime a analise feita na tela com todos os seus campos e formatações */
void mostrar_est(Analise* A)
{
	cout <<"------------------------------------------------";
	cout <<fixed <<"\nNUMERO DE TRANSFERENCIAS:\t|" <<A->transf
		 <<"\nNUMERO DE COMPARACOES:\t\t|" <<A->comp
		 <<"\nTEMPO DE EXECUCAO\t\t|" <<setprecision(4) <<A->time <<" ms" <<endl;
	cout <<"------------------------------------------------\n";
}

/* deixa default todos os campos da estrutura passada como parâmetro */
void zera_analise(Analise* A)
{
	A->transf = 0;
	A->comp = 0;
	A->time = 0;
	A->inicio = 0;
	A->fim = 0;
}