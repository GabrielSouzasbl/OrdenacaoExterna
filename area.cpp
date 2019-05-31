#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "area.h"
#include "registro.h"

using namespace std;

	struct tipoarea
	{
		vector<Registro> itens; 	//um vetor dinamico para registros
		int NumCelOcupadas;			//contador de células ocupadas pelo vetor acima
		int posicao_ativa[MEM_INT]; //cada indice corresponde a uma posicao (1 significa ativa, 0 desativada); 
	};

/* cria uma área estatica e a retorna */
TipoArea cria_area()
{
	TipoArea Area;

	return Area;
}

/* inicializa a area previamente criada com 0s */
void FAVazia(TipoArea *Area)
{ 	
	Area->itens.clear();
	Area->NumCelOcupadas = 0;	

	for(int i=0; i<MEM_INT; i++)
		Area->posicao_ativa[i] = 0;
}

/* verifica se ha pelo menos 1 posição ativa na área */
int Evazia(tipoarea *Area)
{
	for(int i=0; i<Area->NumCelOcupadas; i++)
		if(Area->posicao_ativa[i])
			return 1;

	return 0;
}

/* retorna o número de elementos presentes na área */
int ObterNumCelOcupadas(TipoArea *Area)
{ 
	return Area->NumCelOcupadas;
}

/* retorna o registro presente na área na posição solicitada */
Registro retorna_registro(TipoArea* Area, int posicao)
{
	/* se estiver dentro do intervalo retorna a posicao */
	if(posicao < ObterNumCelOcupadas(Area) || posicao >= 0)
		return Area->itens[posicao];
}

/* insere um elemento na área 
	indice_posição - é utilizado para saber qual indice deve se marcar 
	ordena_ativa - se (1), então ordena a area | se (2) então ativa a posição */
void InsereItem(Registro A, TipoArea *Area, int indice_posicao, int ordena_ativa, Analise* estatistica)
{
	if(Area == NULL) 
		return;

    if(Area->NumCelOcupadas == MEM_INT) 
    { 
    	cout <<"\n\nA lista esta cheia!\n";
    	return;
 	}

 	/* insere item no final da lista */
 	Area->itens.push_back(A);

 	/* aumenta contador de itens */
 	Area->NumCelOcupadas++;

 	if(ordena_ativa == 1) //ordena vetor
 	{
	 	/* ordenação interna dentro da area */
	 	quickSort(Area, Area->NumCelOcupadas, estatistica); 
	 	return;		
 	}
 	else if(ordena_ativa == 2) //ativa posição
 	{
 		/* ativa o indice da posicao que recebeu um registro até que ela não seja mais necessária, quando é desativada*/
 		Area->posicao_ativa[indice_posicao] = 1;
 	}
}

/* retira o primeiro elemento da area o retorna, e reposiciona todos os outros */
void RetiraPrimeiro(TipoArea *Area, Registro *A)
{ 
	/*verifica se a Area existe */
	if(Area == NULL) 
		return;
	/* verifica se a Area é vazia */
    if(Area->NumCelOcupadas == 0) 
       	return;

 	/*retornando valor a ser removido */
 	*A = *Area->itens.begin();

	/*deletando utilizando método de classe vector */
 	Area->itens.erase(Area->itens.begin());

 	/* reduz o número de itens presentes na estrutura */
	Area->NumCelOcupadas--;
}

/* retira o último elemento da área e o retorna */
void RetiraUltimo(TipoArea *Area, Registro *A)
{ 
	/*verifica se a Area existe */
	if(Area == NULL) 
		return;
	
	/* verifica se a Area é vazia */
	if(Area->NumCelOcupadas == 0) 
		return;

	/*retornando valor a ser removido */
	*A = Area->itens.back();

	/*deletando utilizando método de classe vector */
	Area->itens.pop_back();

	/* reduz o número de itens presentes na estrutura */
	Area->NumCelOcupadas--;
}

/* se um registro é marcado e se torna maior de todos o sua nota é somado 101 (apenas para fins de comparação, não alterando seu valor )*/
float maior_de_todos(float nota, int marcado)
{
	/*se ja estivar marcado ele é maior de todos */   //OBS: comparação entre dois "maior de todos" analisa valor original.
	if(marcado)
		return nota+101.0;
	else
		return nota; //se nao estivar marcado retorna original
}

/* verifica se ha pelo menos uma posição inativa na fita (caso todas estiverem ativas ele desativa e retorna 0 avisando que se inicia um novo bloco)*/
int fim_de_fita(TipoArea* Area)
{
	/* se pelo menos uma estiver desmarcada continua processo */
	for(int i=0; i<Area->NumCelOcupadas; i++)
		if(!Area->posicao_ativa[i])
			return 1;


	/*desativa todas e retorna 0 para um novo bloco */
	for(int i=0; i<Area->NumCelOcupadas; i++)
		Area->posicao_ativa[i] = 0;

	return 0;
}

/* insere um novo elemento na área recostituindo seu heap (pai MENOR que filhos )*/
void insercao_heap(TipoArea* Area, Registro entra, Registro saiu, Analise* estatistica)
{
	c_hit(estatistica);
	if(entra.nota < saiu.nota)
	{		
		/* posicao é marcada */
		Area->posicao_ativa[0] = 1;
	}

	Area->itens[0] = entra;

	heap_refaz(Area, 0, Area->NumCelOcupadas-1, estatistica);
}

/* faz uma movimentação com ajuda de um auxiliar, preservando a marcação no registro correto */
void troca(TipoArea* Area, int pai, int filho)
{
	Registro aux;
	int aux_maior_de_todos;

	/* troca registros */
	aux = Area->itens[pai];
	Area->itens[pai] = Area->itens[filho];
	Area->itens[filho] = aux;

	/*troca maior de todos (flag)*/
	aux_maior_de_todos = Area->posicao_ativa[pai];
	Area->posicao_ativa[pai] = Area->posicao_ativa[filho];
	Area->posicao_ativa[filho] = aux_maior_de_todos;
}


/* METODOS PARA ORDENAÇÃO INTERNA DA AREA */

/* funções para execução de um heapsort */
void heapsort(TipoArea* Area, int tam, Analise* estatistica)
{
	Registro aux;

	heap_constroi(Area, tam, estatistica);

	while(tam > 1)
	{		
		troca(Area, tam-1, 0);
		tam--;
		
		heap_refaz(Area, 0, tam-1, estatistica); //refaz o heap
	}
}

void heap_constroi(TipoArea* Area, int tam, Analise* estatistica)
{
	int esq;

	esq = (tam/2)-1;
	
	while(esq >= 0)
	{
		heap_refaz(Area, esq, tam-1, estatistica);
		esq--;
	}
}

void heap_refaz(TipoArea* Area, int esq, int dir, Analise* estatistica)
{
	int i = esq;
	int j = i*2 + 1; /* j é o primeiro filho de i */

	Registro aux = Area->itens[i];  /* aux é igual pai de j */
	int salva_ativa = Area->posicao_ativa[i];
	while(j <= dir)
	{
		c_hit(estatistica);
		if(j < dir && maior_de_todos(Area->itens[j].nota, Area->posicao_ativa[j]) > maior_de_todos(Area->itens[j+1].nota, Area->posicao_ativa[j+1]))
		{
			j++; // j recebe outro filho de i
		}

		c_hit(estatistica);
		if(maior_de_todos(aux.nota, Area->posicao_ativa[i]) <= maior_de_todos(Area->itens[j].nota, Area->posicao_ativa[j]))
		{
			break; // heap foi refeito corretamente
		}

		troca(Area, i, j);

		i = j;
		j = i*2 + 1;
	}
}


/* funções para execuções de um quicksort interno */
void quickSort(TipoArea* A, int n, Analise* estatistica)
{
	quickSort_ordena(A, 0, n-1, estatistica);
}

void quickSort_ordena(TipoArea* A, int esq, int dir, Analise* estatistica)
{
	int i, j;
	quickSort_particao(A, esq, dir, &i, &j, estatistica);

	if(esq < j)
		quickSort_ordena(A, esq, j, estatistica);

	if(i < dir)
		quickSort_ordena(A, i, dir, estatistica);
}

void quickSort_particao(TipoArea* A, int esq, int dir, int* i, int* j, Analise* estatistica)
{
	float pivo, aux;
	*i = esq;
	*j = dir;

	pivo = A->itens[(*i + *j)/2].nota;
	
	do
	{
		c_hit(estatistica);
		while(!(pivo <= A->itens[*i].nota))
		{
			(*i)++;
			c_hit(estatistica);
		}
		
		c_hit(estatistica);
		while(pivo < A->itens[*j].nota)
		{
			(*j)--;
			c_hit(estatistica);
		}
		
		c_hit(estatistica);
		if(*i <= *j)
		{
			aux = A->itens[*i].nota;
			A->itens[*i].nota = A->itens[*j].nota;
			A->itens[*j].nota = aux;
			
			(*i)++;
			(*j)--;
		}
	}while(*i <= *j);
}

