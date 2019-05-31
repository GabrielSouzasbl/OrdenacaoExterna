#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "registro.h"	/* trata os dados que seram utilizados pelo programa */
#include "area.h"		/* trata os métodos de área (Memória interna) disponivel para execução */
#include "fita.h"		/* trata os métodos que necessitam de fitas para execução*/
#include "quicksort.h"	/* trata os métodos do quicksort externo in-situ*/
#include "analise.h"	/* trata todas as contagem necessarias para estatistica de execucao */

using namespace std;

/* testa todos os erros que podem ser cometidos pelo usuário ao executar o programa e suas passagens de argumento */
int teste_arg(int argc, char* argv[]);
void arg_opcional(FILE* arq_entrada, FILE* arq_saida, long QUANTIDADE); //executa o -p caso solicitado
void nome_do_metodo(int nome); //apenas para imprimir o nome do método desejado de acordo com o argumento passado


/* aberturas de arquivo */
FILE* crescente();	//abre o arquivo crescente 
FILE* decrescente();//abre o arquivo decrescente 
FILE* aleatorio(); 	//abre o arquivo aleatório
FILE* provao();		//abre o provao pre-processado (apenas para quicksort externo)

/* estrutura propostas */
void IBBO(FILE* arq_entrada, char* argv[], Analise* estatistica);  //Intercalação Balanceada Blocos Ordenados 		  (método 1)
void IBSS(FILE* arq_entrada, char* argv[], Analise* estatistica);  //Intercalação Balanceada Substituição por Seleção (método 2)
int  QKSE(FILE* arq_entrada, char* argv[], Analise* estatistica);  //QuickSort Externo 								  (método 3)

int main(int argc, char* argv[])
{
	/* start em semente aleatória */
	srand(time(NULL));

	/*validando argumentos de entrada */
	if(!teste_arg(argc, argv))
		return 0;

	/* conteúdo de variável é o arquivo a ser ordenado */
	FILE* arq_entrada;

 	/* estrutura que captura dados para análise posterior (criação por default zera todos os indices) */ 
	Analise* estatistica;
	estatistica = cria_analise();

	/* menu interno que trata leva a cada uma das funções */
	switch(atoi(argv[3]))
	{
		case 1: 
		{ 
			if((arq_entrada = crescente()) == NULL) //abre arquivo ordenado ascendentemente
				return 0; 
		} break;

		case 2: 
		{ 
			if((arq_entrada = decrescente()) == NULL) //abre arquivo ordenado descendentemente
				return 0; 
		} break;

		case 3: 
		{ 
			if((arq_entrada = aleatorio()) == NULL) //abre arquivo desordenado aleatoriamente
				return 0; 
		} break;

		default: 
		{
			cout <<"\n\nDESCULPE DISPONIBILIZAMOS APENAS"
				 <<"\n[1] - ONDENADO ASCENDENTEMENTE"
				 <<"\n[2] - ORDENADO DESCENDENTEMENTE"
				 <<"\n[3] - DESORDENADO ALEATORIAMENTE"
				 <<"\n\nTENTE NOVAMENTE.\n\n";

			fflush(stdout);
			return 0;
		} break;
	}	

	/* menu interno que leva a cada uma das funções */
	switch(atoi(argv[1]))
	{
		case 1:	{ IBBO(arq_entrada, argv, estatistica); } break; //chama método 1

		case 2: { IBSS(arq_entrada, argv, estatistica); } break; //chama método 2

		case 3: { if(!QKSE(arq_entrada, argv, estatistica)) return 0; } break; //chama método 3

		default:
		{
			cout <<"\n\nDESCULPE DISPONIBILIZAMOS APENAS"
 			 <<"\n[1] - CRIACAO DE BLOCOS ORDENADOS INTERNAMENTE E INTERCALACAO BALANCEADA DE VARIOS CAMINHOS COM 2F FITAS. "
			 <<"\n[2] - CRIACAO DE BLOCOS PELO METODO DE SUBSTITUICAO POR SELECAO E INTERCALACAO BALANCEADA DE VARIOS CAMINHOS COM 2F FITAS. "
			 <<"\n[3] - QUICKSORT EXTERNO.\n";

			fflush(stdout);
		} break;
	}


	/* conteúdo de variavel é o resultado da ordenação */
	FILE* arq_saida;
	if((arq_saida = fopen("PROVAO_ORDENADO.bin", "rb")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		return 0;
	}

	/* teste de argumento opcional -p */
	if((!strcmp(argv[4], "-p")) || (!strcmp(argv[4], "-P"))) //imprimir arquivo pré e arquivo pós ordenação
			arg_opcional(arq_entrada, arq_saida, atoi(argv[2]));

	/* print estatisticas */

		/* mostrar na tela estatistica de criacaco de indices */
		cout <<"\n\n"; nome_do_metodo(atoi(argv[1]));
		mostrar_est(estatistica);

	fclose(arq_entrada);
	fclose(arq_saida);
	deleta_analise(estatistica); /* deleta a estrutura de analise dinamicamente */
	return 0;
}

/* menu teste de argumentos */
int teste_arg(int argc, char* argv[])
{	
	/* menu para leigos */
	if(argc == 1)
	{
		cout <<"\n\nBEM VINDO AO PROGRAMA - ANALISE ENTRE METODOS DE ORDENACAO EXTERNA -\n"
			 <<"POR: GABRIEL CARVALHO DE SOUZA."
			 <<"\nMAT: 17.1.4029"
			 <<"\nLEIA E SIGA OS PASSOS ABAIXO PARA COMPREENSÃO DA EXECUÇÃO.";

		cout <<"\n\n\t******** MENU ********"
			 <<"\n\n-O EXECUTAVEL DEVE SER INICIALIZADO COM OS SEGUINTES PARAMENTROS: "
			 <<"\n\n./<EXECUTAVEL> <MÉTODO> <QUANTIDADE> <SITUACAO> [-P] "
			 <<"\n\nONDE: "
			 <<"\n\nMETODO: "
			 <<"\n[1] - CRIACAO DE BLOCOS ORDENADOS INTERNAMENTE E INTERCALACAO BALANCEADA DE VARIOS CAMINHOS COM 2F FITAS. "
			 <<"\n[2] - CRIACAO DE BLOCOS PELO METODO DE SUBSTITUICAO POR SELECAO E INTERCALACAO BALANCEADA DE VARIOS CAMINHOS COM 2F FITAS. "
			 <<"\n[3] - QUICKSORT EXTERNO. "
			 <<"\n\nQUANTIDADE: "
			 <<"\n-UM INTEIRO ENTRE 1 E 471705(471.705). "
			 <<"\n\nSITUACAO: "
		 	 <<"\n[1] - ARQUIVO ORDENADO ASCENDENTEMENTE. "
			 <<"\n[2] - ARQUIVO ORDENADO DESCENDENTEMENTE."
			 <<"\n[3] - ARQUIVO DESORDENADO ALEATORIAMENTE."
			 <<"\n\n[-P]: "
			 <<"\n- É OPCIONAL, COM ELE SERA IMPRESSO NA TELA OS DADOS ANTES E APOS A ORDENACAO. "
			 <<"\n- SEM ELE, APENAS AS ESTATISTICAS SERAO MOSTRADAS. "
			 <<"\n\nEXEMPLO DE EXECUCAO: ./tp 1 200 2 -p\n\nBOA SORTE!" <<endl;

		return 0;
	}

	/* trata a opção opcional [-P]*/
	if(argv[4] == NULL)
		argv[4] = (char*)"flag";

	/* 5 sem o -p, 6 com o -p */
	if(argc != 4 && argc != 5)
	{
		cout <<"\nVOCE NAO DIGITOU O NUMERO CORRETO DE ARGUMENTOS NECESSARIOS PARA A PESQUISA\n\n"
			 <<"TENTE NOVAMENTE\n" <<endl;
		return 0;
	}

	/* numero de registros fora dos limites*/
	int quantidade = atoi(argv[2]);
	if(quantidade > 471705 || quantidade <= 0)
	{
		cout <<"\nDESCULPE A QUANTIDADE DE REGISTROS SELECIONADA ESTÁ FORA DOS LIMITES ESTABELECIDOS\n\n"
			 <<"EXECUTE ./tp  PARA MAIS INFORMAÇÕES\n";
		return 0;
	}

	return 1;
}

/* abre arquivo crescente devidamente trantando erro */
FILE* crescente()
{
	FILE* arqb;

	if((arqb = fopen("crescente.bin", "r+b")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		arqb = NULL;
		return arqb;
	}
	return arqb;
}

/* abre arquivo decrescente devidamente trantando erro */
FILE* decrescente()
{
	FILE* arqb;

	if((arqb = fopen("decrescente.bin", "r+b")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		arqb = NULL;
		return arqb;
	}
	return arqb;	
}

/* abre arquivo aleatorio devidamente trantando erro */
FILE* aleatorio()
{
	FILE* arqb;

	if((arqb = fopen("PROVAO.bin", "r+b")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		arqb = NULL;
		return arqb;
	}
	return arqb;	
}

/* para o quicksort abriremos este arquivo pois a ordenação por esse método é in-situ*/
FILE* provao()
{
	FILE* arqb;

	if((arqb = fopen("PROVAO_ORDENADO.bin", "r+b")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		arqb = NULL;
		return arqb;
	}
	return arqb;	
}

void IBBO(FILE* arq_entrada, char* argv[], Analise* estatistica)
{

	/*divide os arquivos criados em fitas de entrada e saida */	

	Fita* entrada = cria_fita(); //criando 10(TAM SUGERIDO) fitas de entrada 	
	Fita* saida = cria_fita();	// criando 10(TAM SUGERIDO) fitas de saida 

	/*iniciando time para criacao de blocos */
	start_time(estatistica);

	criacao_de_blocos(&arq_entrada, entrada, saida, atoi(argv[2]), estatistica);	

	short flag_final = 1; //flag indica em qual fita está o arquivo final : (1 saida) | (0 entrada);

	/* intercalação balanceada */
	while(intercalacao_balanceada(entrada, saida, estatistica))
	{	
		if(!intercalacao_balanceada(saida, entrada, estatistica))
		{
			flag_final = 0;
			break;
		}
	}
	/* gravando tempo de criacao de blocos */
	stop_time(estatistica);


	//está na primeira fita de saida
	if(flag_final)
	{
		fclose(get_file(saida));
		rename("11.bin", "PROVAO_ORDENADO.bin");
	}  

	//está na primeira fita de entrada
	else 
	{
		fclose(get_file(entrada));
		rename("1.bin", "PROVAO_ORDENADO.bin");
	}

	apaga_fita(entrada, 1);
	apaga_fita(saida, 2);
}

void IBSS(FILE* arq_entrada, char* argv[], Analise* estatistica)
{
	/*divide os arquivos criados em fitas de entrada e saida */	

	Fita* entrada = cria_fita(); //criando 10(TAM SUGERIDO) fitas de entrada 	
	Fita* saida = cria_fita();	// criando 10(TAM SUGERIDO) fitas de saida 

	/*iniciando time para criacao de blocos */
	start_time(estatistica);

	substituicao_selecao(&arq_entrada, entrada, saida, atoi(argv[2]), estatistica);	

	short flag_final = 1; //flag indica em qual fita está o arquivo final : (1 saida) | (0 entrada);

	/* intercalação balanceada */
	while(intercalacao_balanceada(entrada, saida, estatistica))
	{	
		if(!intercalacao_balanceada(saida, entrada, estatistica))
		{
			flag_final = 0;
			break;
		}
	}

	/* gravando tempo de criacao de blocos */
	stop_time(estatistica);

	//está na primeira fita de saida
	if(flag_final)
	{
		fclose(get_file(saida));
		rename("11.bin", "PROVAO_ORDENADO.bin");
	}  

	//está na primeira fita de entrada
	else 
	{
		fclose(get_file(entrada));
		rename("1.bin", "PROVAO_ORDENADO.bin");
	}

	apaga_fita(entrada, 1);
	apaga_fita(saida, 2);
}

int QKSE(FILE* arq_entrada, char* argv[], Analise* estatistica)
{
	FILE *ArqLEs;   /* Gerencia o Ls e o Es */
	FILE *ArqLi;    /* Gerencia o Li */
	FILE *ArqEi;    /* Gerencia o Ei */

	switch(atoi(argv[3]))
	{
		case 1: 
		{
			if((ArqLi = crescente()) == NULL)
				return 0;
		} break;

		case 2: 
		{
			if((ArqLi = decrescente()) == NULL)
				return 0;
		} break;

		case 3: 
		{
			if((ArqLi = aleatorio()) == NULL)
				return 0;
		} break;
	}

	/* --------------PRÉ-PROCESSAMENTO DO PROVÃO --------------*/
	FILE* ordenado_quick;
	Registro aux;
	long cont = 0;

	if((ordenado_quick = fopen("PROVAO_ORDENADO.bin", "w+b")) == NULL)
	{
		cout <<"\n\nFALHA NA CRIACAO DO ARQUIVO\n";
		return 0;
	}

	/* copia do arquivo sugerido para o provao a ser ordenado */
	while(fread(&aux, sizeof(Registro), 1, ArqLi) == 1 && cont < atoi(argv[2]))
	{
		fwrite(&aux, sizeof(Registro), 1, ordenado_quick);
		cont++;
	}

	fclose(ArqLi);
	fclose(ordenado_quick);
	/* --------------PRÉ-PROCESSAMENTO DO PROVÃO --------------*/


	/* abertura dos ponteiros no arquivo a ser ordenado */
	if((ArqLi  = provao()) == NULL) return 0;	
	if((ArqLEs = provao()) == NULL) return 0;
	if((ArqEi  = provao()) == NULL) return 0;
	
	// iniciando time para criacao de blocos 
	start_time(estatistica);

	QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, atoi(argv[2]), estatistica);

	/* gravando tempo de criacao de blocos */
	stop_time(estatistica);
	
	fflush(ArqLi);

	fclose(ArqEi); 
	fclose(ArqLEs);
	fclose(ArqLi);

	return 1;
}

void arg_opcional(FILE* arq_entrada, FILE* arq_saida, long QUANTIDADE)
{
	fseek(arq_entrada, 0, 0);
	fseek(arq_saida, 0, 0);

	cout <<"\n\nARQUIVO SELECIONADO PARA ORDENAÇÃO:\n\n";
	print_arquivo(arq_entrada, QUANTIDADE);

	cout <<"\n\nARQUIVO ORDENADO PELO MÉTODO ESCOLHIDO:\n\n";
	print_arquivo(arq_saida, QUANTIDADE);
}

void nome_do_metodo(int nome)
{
	switch(nome)
	{
		case 1:
			cout <<"INTERCALACAO BALANCEADA CRIACAO DE BLOCOS ORDENADOS INTERNAMENTE\n";
		break;

		case 2:
			cout <<"INTERCALACAO BALANCEADA CRIACAO DE BLOCOS POR SUBSTITUICAO POR SELECAO\n";
		break;

		case 3:
			cout <<"QUICKSORT EXTERNO\n";
		break;
	}
}

