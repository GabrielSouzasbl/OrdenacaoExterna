#include <iostream>
#include <iomanip>

	typedef struct registro
	{
		long int inscricao;
		float nota;
		char estado[3];
		char cidade[51];
		char curso[31];
		
	}Registro;

using namespace std;

int main()
{
	FILE* arqt;
	FILE* arqb;

	Registro A;

	if((arqt = fopen("PROVAO.txt", "r+")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		return 0;
	}

	if((arqb = fopen("PROVAO.bin", "w+b")) == NULL)
	{
		cout <<"\n\nFALHA NA CRIACAO DO ARQUIVO\n";
		return 0;
	}

	while(fscanf(arqt, "%ld %f %2s %50[A-Z '-] %30[A-Z ] '\n'", &(A.inscricao), &(A.nota), A.estado, A.cidade, A.curso) != EOF)
		fwrite(&A, sizeof(Registro), 1, arqb);


	return 0;
}