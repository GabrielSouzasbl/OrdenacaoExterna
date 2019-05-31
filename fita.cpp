#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "fita.h"
#include "registro.h"

using namespace std;

	struct tipoarea
	{
		vector<Registro> itens; 	//um vetor dinamico para registros
		int NumCelOcupadas;			//contador de células ocupadas pelo vetor acima
		int posicao_ativa[MEM_INT]; //cada indice corresponde a uma posicao (1 significa ativa, 0 desativada); 
	};

	struct fita
	{
		FILE* arq;					//um arquivo binário que representa o disco. 
		short ativa;				// 1 indica fita ativa, 0 inativa.
		long int n_blocos;			// quantidade de blocos presentes no arquivo.
		int tam_blocos[TAM_BLOCO];  // quantidade de registros presentes em cada bloco do arquivo. (cada indice é um bloco e guarda um int com a quantidade)
	};

/* cria dinamicamente metade do total de fitas pré-estabelecidas e retorna */
Fita* cria_fita()
{
	Fita* disk = new fita[FITAS/2];

	return disk;
}

/* fecha todos os arquivos, depois remove fisicamente e por fim apaga a fita */
void apaga_fita(Fita* disk, int marcador)
{
	char aux[FITAS/2];
	
	//loop para fechamento do arquivo 
	for(int i=0; i<FITAS/2; i++)
		fclose(disk[i].arq);

	//verifica se são fitas de entrada ou saida 
	switch(marcador)
	{
		case 1: //entrada
		{
			//loop para remover fitas
			for(int i=0; i<FITAS/2; i++)
			{
				sprintf(aux, "%d.bin", (i+1));
				remove(aux);
			}
		} break;

		case 2:	//saida
		{
			//loop para remover fitas
			for(int i=10; i<FITAS; i++)
			{
				sprintf(aux, "%d.bin", (i+1));
				remove(aux);
			}
		} break;		
	}

	delete disk;
}

/* deixa todos os campos da fita com 0*/
void zera_fita(Fita* disk)
{
	disk->n_blocos = 0;
	disk->ativa = 0;

	for(int i=0; i<TAM_BLOCO; i++)
		disk->tam_blocos[i] = 0;
}

/* recebe um nome e cria arquivo com o mesmo */
void set_arquivo(Fita* disk, int nome)
{
	char aux[FITAS/2];

	sprintf(aux, "%d.bin", nome);

	if((disk->arq = fopen(aux, "w+b")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		return;
	}
}

/*retorna o arquivo existente dentro da priemira fita */
FILE* get_file(Fita* disk)
{
	return disk[0].arq;
}

/* retorna 1 para fita ativa, 0 para inativa */
short estado_da_fita(Fita* disk)
{
	return disk->ativa; //retorna 0 ou 1
}

/* ativa todas fitas que tiverem 1 ou mais blocos ainda em seus arquivos */
void set_ativa(Fita* disk)
{
	for(int i=0; i<FITAS/2; i++)
		if(disk[i].n_blocos > 0)
			disk[i].ativa = 1;
		else
			disk[i].ativa = 0;
}

/* recebe uma área e a salva no arquivo presente na fita */
void salva_em_fita(Fita* disk, TipoArea* Area, Analise* estatistica)
{
	/* ativa a fita */
	disk->ativa = 1;
	
	/* salva os registros em fita */
	int i=0;

	/* loop de gravação em fita */
	while(fwrite(&(Area->itens[i]), sizeof(Registro), 1, disk->arq) == 1 && i < Area->NumCelOcupadas-1) 
	{
		t_hit(estatistica);
		i++;	
	}
	
	/* aumenta o numero de blocos */
	disk->n_blocos++;

	/* recebe a quantidade de células ocupadas por aquele bloco */
	disk->tam_blocos[disk->n_blocos-1] = Area->NumCelOcupadas;
}

/* cria blocos ordenados internamente baseados no tamanho de area sugerido */
void criacao_de_blocos(FILE** principal, Fita* entrada, Fita* saida, long total, Analise* estatistica)
{
	/* arquivos criados em main sendo atribuidos as fitas */
	for(int i=0; i<FITAS/2; i++)
	{
		set_arquivo(&entrada[i], i+1); //cria arquivos com nome de 1 a 10
		zera_fita(&entrada[i]);

		set_arquivo(&saida[i], i+11); //cria arquivos com nome de 11 a 20
		zera_fita(&saida[i]);
	}

	/* chama funcao auxiliar que faz criacao de blocos ordenados */
	criacao_de_blocos_aux(entrada, *principal, total, estatistica);
}

/* iterativa para nao estourar a pilha de execução */
/*auxiliar de criacao de blocos ordenados */
void criacao_de_blocos_aux(Fita* entrada, FILE* principal, long total, Analise* estatistica)
{
	long i = 0, parar = 0;
	TipoArea Area = cria_area();
	Registro reg_aux;
	
	/*total representa o número de arquivos que vamos ordenar */
	while(parar != total)
	{
		FAVazia(&Area);

		int cont = 0;

		/* preenche a area com os primeiros registros */
		while(cont < MEM_INT && parar < total)
		{
			t_hit(estatistica);
			fread(&reg_aux, sizeof(Registro), 1, principal); //leitura em arquivo principal

			InsereItem(reg_aux, &Area, -1, 1, estatistica); //inserção em area (ordenado internamente)
			cont++;
			parar++;
		}

		salva_em_fita(&entrada[i], &Area, estatistica); //grava na primeira fita.

		fflush(entrada[i].arq);


		/* se ler menos que 10 registros é porque chegou no final do arquivo (fim da criação de blocos) */
		if(parar == total)
			break;

		/* quando esgotar as fitas volte a preencher a primeira fita */
		if(i == 9)
			i = -1;

		i++;
	}
}

/* criação de blocos por método de substituição por seleção */
void substituicao_selecao(FILE** principal, Fita* entrada, Fita* saida, long total, Analise* estatistica)
{
	/* arquivos criados em main sendo atribuidos as fitas */
	for(int i=0; i<FITAS/2; i++)
	{
		set_arquivo(&entrada[i], i+1); //cria arquivos com nome de 1 a 10
		zera_fita(&entrada[i]);

		set_arquivo(&saida[i], i+11); //cria arquivos com nome de 11 a 20
		zera_fita(&saida[i]);
	}

	/* chama função auxiliar que faz criacao de blocos ordenados */
	substituicao_selecao_aux(entrada, *principal, total, estatistica);	
}

/*função auxiliar de criação de blocos por substituição por seleção */
void substituicao_selecao_aux(Fita* entrada, FILE* principal, long total, Analise* estatistica)
{

	int cont_escrita = 0; //variavel que indica qual fita será preenchida 
	int parar = 0;		 //variavel que indica quando devemos parar de ler do arquivo principal 

	/*entra na area */
	Registro entra;
	/*sai da area */
	Registro sai;

	TipoArea Area = cria_area();
	FAVazia(&Area);

	/*preenche a area com os primeiros registros do arquivo principal */
	while(parar < MEM_INT && parar < total)
	{
		t_hit(estatistica);
		fread(&sai, sizeof(Registro), 1, principal); //leitura em arquivo 1 a 1
		InsereItem(sai, &Area, -1, 3, estatistica);  //insere registros sem ordenar internamente 
		parar++;
	}

	/* faz o primeiro heap em área (pais MENORES que filhos)*/
	heap_constroi(&Area, ObterNumCelOcupadas(&Area), estatistica);

	/*referente ao goto linha 286 */
	inicio:

		entrada[cont_escrita].n_blocos++;

		/* total representa a quantidade de máx de arquivos que vamos ordenar */
		while(parar < total)
		{
			//recebe primeira e menor posicao da area
			sai = retorna_registro(&Area, 0);

			//escreve ela no arquivo corrente
			escrita(&entrada[cont_escrita], sai, estatistica);

			//le prox registro do arquivo original
			t_hit(estatistica);
			fread(&entra, sizeof(Registro), 1, principal);
			parar++;

			//insere na posicao que saiu e refaz heap
			//se retornar 0 continua, se retornar 1 break e vai pra próx bloco
			insercao_heap(&Area, entra, sai, estatistica);

			/*verifica se tem pelo menos uma "não marcada" */
			/*se todas estiverem marcadas desmarca (desativa)*/
			if(!fim_de_fita(&Area))
				goto prox_bloco;
		}

		/* se acabar a leitura do arquivo principal */
		if(parar == total)
		{
			//esgota area dentro da fita e retorna finalizando
			esgota_area(&Area, entrada, cont_escrita, estatistica);
			return;
		}

		/*referente ao goto linha 268*/
		prox_bloco:
			cont_escrita++; //passa a escrita para a próx fita 

			if(cont_escrita == (FITAS/2)) //se chegar ao final das fitas volta para a primeira 
				cont_escrita = 0;

	goto inicio;
}

/* intercala os blocos previamente criados dobrando seu tamanho, quando o houver apenas 1 bloco com todos os registros a intercação acabou */
int intercalacao_balanceada(Fita* entrada, Fita* saida, Analise* estatistica)
{
	/*não ha necessidade de intercalação */
	if(contador_blocos(entrada) == 1)
		return 0;

	Registro A;

	/* auxiliar de area para intercalação */
	TipoArea Area = cria_area();	

	/*criação de variaveis */
	long int cont_leitura = 0; 				//indica qual fita estamos lendo 
	long int cont_escrita = 0; 				//indica qual fita vamos gravar
	long int qual_bloco = 1;   				//indica qual bloco estamos acessando 
	long int menor = 0;		  				//recebe o indice do menor elemento da área
	long int quantas_intercalacoes = 0;		//quantas intercalações foram feitas, se terminar com 1, significa que acabou a ordenação desta parte 

	/*sobe ponteiro de todos os arquivos */
	for(int i=0; i<FITAS/2; i++)
	{
		fseek(entrada[i].arq, 0, 0);
		fseek(saida[i].arq, 0, 0);
	}

	/*enquanto houver fitas ativas (com blocos ativos)*/
	while(verifica_bloco(entrada))
	{
		quantas_intercalacoes++;

		FAVazia(&Area);
		saida[cont_escrita].n_blocos++;
		cont_leitura = 0;

		/*preenche a area com o primeiro item de cada fita */
		while(cont_leitura < MEM_INT)
		{
			if(estado_da_fita(&entrada[cont_leitura])) 							  //verifica se a fita em questão está ativa
				if(leitura(&entrada[cont_leitura], &A, qual_bloco, estatistica)) //faz leitura
					InsereItem(A, &Area, cont_leitura, 2, estatistica);			//insere item em área

			cont_leitura++;		
		}

		/*loop nao esgota a area, esgota apenas as fitas */
		while(verifica_tam_bloco(entrada))
		{
			/*pesquisa o menor dentro da area, despresando os desativados */
			menor = procura_menor(Area, estatistica);

			/*escreve menor na fita de saida */
			escrita(&saida[cont_escrita], Area.itens[menor], estatistica);	
			
			/*lê o prox registro da fita em que saiu o menor */
			if(leitura(&entrada[menor], &A, qual_bloco, estatistica))
				Area.itens[menor] = A; /* adiciona o registro sobrescrevendo o menor retirado */
			else
				Area.posicao_ativa[menor] = 0; //desativa a posicao do menor na area
		}

		/* loop para esgotar area, com verificação */
		while(Evazia(&Area))
		{
			/*pesquisa o menor dentro da area, despresando os desativados */
			menor = procura_menor(Area, estatistica);	

			/*escreve menor na fita de saida */
			escrita(&saida[cont_escrita], Area.itens[menor], estatistica);
			
			Area.posicao_ativa[menor] = 0; //desativa a posicao do menor na area
		}

		set_ativa(entrada); //reativa fitas

		cont_escrita++; 
		qual_bloco++;

		if(cont_escrita == (FITAS/2))
			cont_escrita = 0;
	}
	/* se houver apenas uma intercalação quer dizer que criou um bloco completamente ordenado FIM */
	if(quantas_intercalacoes == 1)
		return 0;

	return 1;
}

/* a cada leitura feita na fita o tam_bloco reduz em 1 */
int leitura(Fita* disk, Registro* A, int qual_bloco, Analise* estatistica)
{
	if(estado_da_fita(disk))
	{
		t_hit(estatistica);
		fread(A, sizeof(Registro), 1, disk->arq);

		/* reduz a quantidade de registros a serem lidos do bloco */
		disk->tam_blocos[qual_bloco-1]--;

		/* se nao tiver mais registros desativa a fita */
		if(disk->tam_blocos[qual_bloco-1] == 0)
		{
			disk->ativa = 0; //desativa fita momentaneamente
			disk->n_blocos--; //fita desativada, então fim de bloco, reduz um.
		}

		return 1;
	}
	return 0;
}

/*método verifica quantos blocos existem nas fitas de entrada, caso haja mais que 1 ha necessidade de intercalação do contrário não */
int contador_blocos(Fita* disk)
{
	int cont = 0;
	for(int i=0; i<FITAS/2; i++)
		if(disk[i].n_blocos > 0)
			cont++;

	return cont;
}

/* a cada escrita em fita, o tam_bloco aumenta em 1 */
void escrita(Fita* disk, Registro A, Analise* estatistica)
{
	/* ativa a fita caso não esteja */
	if(!estado_da_fita(disk))
		disk->ativa = 1;

	t_hit(estatistica);
	fwrite(&A, sizeof(Registro), 1, disk->arq);

	disk->tam_blocos[disk->n_blocos-1]++;
}

/* procura menor elemento presente na área e retorna seu indice */
int procura_menor(TipoArea Area, Analise* estatistica)
{
	int i, j;
	Registro aux = Area.itens[0];
	int guarda_indice = 0;

	/*loop para garantir 'aux' com a primeira posição ativa da area */
	for(i = 0; i<Area.NumCelOcupadas; i++)
		if(Area.posicao_ativa[i])
		{
			guarda_indice = i;
			aux = Area.itens[i];
			break;
		}

		/* compara a partir da proxima posição até encontrar o menor elemento presente na area */
	for(int j=i+1; j<Area.NumCelOcupadas; j++)
		if(Area.posicao_ativa[j])
		{
			c_hit(estatistica);
			if(aux.nota > Area.itens[j].nota)
			{
				aux = Area.itens[j];
				guarda_indice = j;
			}
		}

	return guarda_indice;
}

/* verifica se ha pelo menos 1 fita ativa */
int verifica_tam_bloco(Fita* disk)
{
	for(int i = 0; i<FITAS/2; i++)
		if(disk[i].ativa)
			return 1;
	
	return 0;
}

/* verifica se ha pelo menos 1 bloco ativo */
int verifica_bloco(Fita* disk)
{
	for(int i = 0; i<FITAS/2; i++)
		if(disk[i].n_blocos > 0)
			return 1;

	return 0;
}

/* esgota toda a área na fita enviada via parametro, quando houver so elementos marcados estes vão para a próx fita em questão */
void esgota_area(TipoArea* Area, Fita* entrada, int qual_fita, Analise* estatistica)
{
	/* ordena do maior para o menor (respeitando a marcação)*/
	heapsort(Area, ObterNumCelOcupadas(Area), estatistica);

	int cont = ObterNumCelOcupadas(Area)-1;

	Registro aux;

	/* verifica se a posição esta desmarcada */
	if(!Area->posicao_ativa[cont])
	{
		/*posições não marcadas vão para o bloco corrente */
		while(ObterNumCelOcupadas(Area) > 0 && !Area->posicao_ativa[cont])
		{
			RetiraUltimo(Area, &aux);
			escrita(&entrada[qual_fita], aux, estatistica);
			cont--;
		}

		/* modifica qual_fita e aumenta um bloco no próx. */

		if(ObterNumCelOcupadas(Area) > 0)
		{
			qual_fita++;
			if(qual_fita == 10)
				qual_fita = 0;

			entrada[qual_fita].n_blocos++;
		}
		else
			return;
	}
	
	/* posições marcadas vão para o próximo bloco */
	while(ObterNumCelOcupadas(Area) > 0)
	{
		RetiraUltimo(Area, &aux);
		escrita(&entrada[qual_fita], aux, estatistica);
	}

}

