#include <iostream>
#include <vector>
#include <cfloat>
#include <limits.h>
#include <cstring>
#include "quicksort.h"

using namespace std;

	struct tipoarea
	{
		vector<Registro> itens; 	//um vetor dinamico para registros
		int NumCelOcupadas;			//contador de células ocupadas pelo vetor acima
		int posicao_ativa[MEM_INT]; //cada indice corresponde a uma posicao (1 significa ativa, 0 desativada); 
	};


/* recebe os extremos a serem ordenados e 3 ponteiros no arquivo */
void QuicksortExterno(FILE** ArqLi , FILE** ArqEi , FILE** ArqLEs, int Esq, int Dir, Analise* estatistica)
{ 
	int i, j;

	TipoArea Area = cria_area(); //cria uma área simulando memória interna 
	
	if(Dir - Esq < 1) 
		return;	

	FAVazia(&Area);

	//primeira divisão do arquivo 
	Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j, estatistica);
	
	if(i-Esq < Dir-j) /* ordene primeiro o subarquivo menor */
	{ 
		QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, estatistica);
		QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, estatistica);
	}
	else
	{ 
		QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir, estatistica);
		QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i, estatistica);
	}
}

/* a cada leitura superior no arquivo (ponteiro enviado por parâmentro) o apontador é decrementado */
void LeSup(FILE** ArqLEs, Registro* UltLido, int* Ls, short* OndeLer, Analise* estatistica)
{
	fseek(*ArqLEs, (*Ls-1) * sizeof(Registro), 0); //reposiciona ponteiro em arquivo

	t_hit(estatistica);
	fread(UltLido, sizeof(Registro) , 1 , *ArqLEs); //faz leitura 
	
	(*Ls)--;
	*OndeLer = 0;
}

/* a cada leitura inferior no arquivo (ponteiro enviado por parâmentro) o apontador é incrementado */
void LeInf(FILE** ArqLi, Registro* UltLido, int* Li ,short* OndeLer, Analise* estatistica)
{
	t_hit(estatistica); 
	fread(UltLido, sizeof(Registro), 1, *ArqLi); //faz leitura

	(*Li)++; 	
	*OndeLer = 1;
}

/* insere elemento UltimoLido em área de forma ordenada */
void InserirArea(TipoArea* Area, Registro* UltLido, int* NRArea, Analise* estatistica)
{ 
	/* Insere UltLido de forma ordenada na Area */
	InsereItem(*UltLido, Area, -1, 1, estatistica);
	/*recupera o numero de registros presentes na area*/ 
	*NRArea = ObterNumCelOcupadas(Area);
}

/* escreve o maior elemento no arquivo e decrementa ponteiro de escrita */
void EscreveMax(FILE** ArqLEs, Registro A, int* Es, Analise* estatistica)
{ 
	fseek(*ArqLEs, (*Es-1) * sizeof(Registro), 0); //posiciona ponteiro
	
	t_hit(estatistica);
	fwrite(&A, sizeof(Registro),1 , *ArqLEs); //faz escrita
	
	(*Es)--;
}

/* escreve o menor elemento no arquivo e incrementa ponteiro de escrita */
void EscreveMin(FILE** ArqEi, Registro A, int* Ei, Analise* estatistica)
{
 	t_hit(estatistica);
	fwrite(&A, sizeof(Registro), 1, *ArqEi); //faz escrita
	
	(*Ei)++; 
}

/* retira o ultimo (maior) elemento da área e o retorna para fim de comparação */
void RetiraMax(TipoArea* Area, Registro* A, int* NRArea)
{ 
	RetiraUltimo(Area, A); //retira último elemento
	*NRArea = ObterNumCelOcupadas(Area); //reduz área e atualiza variavel que referencia o tamanho da mesma.
}

/* retira o primeiro (menor) elemento da área e o retorna para fim de comparação */
void RetiraMin(TipoArea* Area, Registro* A, int* NRArea)
{
	RetiraPrimeiro(Area, A); //retirna primeiro elemento
	*NRArea = ObterNumCelOcupadas(Area); //reduz área e atualiza variavel que referencia o tamanho da mesma
}

/* faz partição de elementos maiores e menores que o pivô, no próprio arquivo */
void Particao(FILE** ArqLi, FILE** ArqEi, FILE** ArqLEs, TipoArea Area, int Esq, int Dir, int* i, int* j, Analise* estatistica)
{ 
	/* criação de variaveis */
	int Ls = Dir;	//apontador de leitura superior
	int Es = Dir;	//apontador de escrita superior
	int Li = Esq;	//apontador de leitura inferior
	int Ei = Esq;	//apontador de escrita inferior
	int	NRArea = 0; //Numero de registros em área
	float Linf = INT_MIN, Lsup = INT_MAX;  //limites máx e min (são inicializados com -oo e +oo)
	short OndeLer = 1; //variavel de alternâcia entre leituras no arquivo 
	
	/*auxiliares de registro */
	Registro UltLido;
	Registro A;
	
	fseek(*ArqLi, (Li-1) * sizeof(Registro), 0); //mantem ponteiro no inicio do arquivo
	fseek(*ArqEi, (Ei-1) * sizeof(Registro), 0); //mantem ponteiro no inicio do arquivo

	*i = Esq - 1;
	*j = Dir + 1;

	/* enquanto o ponteiro de leitura superior não cruzar o de leitura inferior */
	while (Ls >= Li)
	{ 
		/*preenche a área com elementos alterados (ultimo/primeiro) até seu tamanho total menos 1 */
		if (NRArea < MEM_INT - 1)
		{ 
			if(OndeLer)
				LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, estatistica); //leitura superior
			else 
				LeInf(ArqLi, &UltLido, &Li, &OndeLer, estatistica); //leitura inferior
			
			InserirArea(&Area, &UltLido, &NRArea, estatistica); //insere em area
			continue; //próx iteração do while
		}

		/* LEITURA DO ÚLTIMO ELEMENTO DA ÁREA */
		
		/* compara se os apontadores estão na mesma posição tanto de leitura quanto de escrita */
		if(Ls == Es)
			LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, estatistica);		
		else if(Li == Ei) 
			LeInf(ArqLi, &UltLido, &Li , &OndeLer, estatistica);
		
		else
		{ 
			if(OndeLer)
				LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, estatistica);
			else
				LeInf(ArqLi, &UltLido, &Li , &OndeLer, estatistica);
		}

		/* Verifica se o ultimo lido é maior que o limite superior */
		c_hit(estatistica);
		if(UltLido.nota > Lsup)
		{ 
			*j = Es; 
			EscreveMax(ArqLEs, UltLido, &Es, estatistica);
			continue; //próx iteração do while
		}
		
		/* Verifica se o ultimo lido é menor que o limite inferior */
		c_hit(estatistica);
		if(UltLido.nota < Linf)
		{ 
			*i = Ei;
			EscreveMin(ArqEi, UltLido, &Ei, estatistica);
			continue; //próx iteração do while
		}
		
		/*insere elemento em área caso não atenda as condições acima */				
		InserirArea(&Area, &UltLido, &NRArea, estatistica);
		
		if(Ei - Esq < Dir - Es)
		{ 
			RetiraMin(&Area, &A, &NRArea);
			EscreveMin(ArqEi, A, &Ei, estatistica);

			Linf = A.nota; //limite inferior passa a ser o minimo retirado 
		}		
		else
		{ 
			RetiraMax(&Area, &A, &NRArea);
			EscreveMax(ArqLEs, A, &Es, estatistica);

			Lsup = A.nota; //limite superior passa a ser o máx retirado				
		}
	}

	/* esgota a area escrevendo seus elementos ordenados no arquivo */
	while(ObterNumCelOcupadas(&Area) > 0)
	{
		RetiraMin(&Area, &A, &NRArea);
		EscreveMin(ArqEi, A, &Ei, estatistica);
	}

	fflush(*ArqLi);
	fflush(*ArqLEs);
	fflush(*ArqEi);
}

