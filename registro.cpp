#include <iostream>
#include <iomanip>
#include "registro.h"

using namespace std;

/* não recebe parametros, cria aloca dinamicamente e retorna um registro  */
Registro* cria_registro()
{
	Registro *A;
	A = new Registro;

	return A;
}

/*trata a exclusão de um registro, recebido como parametro */
void deleta_registro(Registro* A)
{
	delete A;
}

/* imprime todos os campos do registro corretamente formatados conforme arquivo .txt de exemplo */
void print_registro(Registro* A)
{
	cout <<setfill('0') <<setw(8) <<A->inscricao <<" "
		 <<setfill('0') <<setw(5) <<fixed <<setprecision(1) <<A->nota <<" "
		 <<A->estado <<" " 
		 <<A->cidade <<" "
		 <<A->curso <<endl;
}

/* imprime um arquivo de registros com determinada quantidade */
void print_arquivo(FILE* arqb, long QUANTIDADE)
{
	Registro A;
	int cont = 0;

	fseek(arqb, 0, 0);

	/* faz a leitura de registro por registro do arquivo até certa quantidade */
	while(fread(&A, sizeof(Registro), 1, arqb) == 1 && cont < QUANTIDADE)
	{
		print_registro(&A); //imprime registro 
		cont++;
	}
}

/* altera a chave(nota) de um registro */
void set_chave(Registro* A, float x)
{
	A->nota = x;
}