#ifndef REGISTRO_H_
#define REGISTRO_H_

/* deixando campos de estrutura públicos */

	typedef struct registro //abaixo todos os campos extraidos do txt sugerido
	{
		long int inscricao; /* inscrição do aluno presente */
		float nota;			/* nota obtida pelo aluno entre 0 e 100 */
		char estado[3];		/* estado natal do aluno */
		char cidade[51];	/* cidade natal do aluno */
		char curso[31];		/* curso do aluno */

	}Registro;

/* metodo cria um registro e aloca dinamicamente */
Registro* cria_registro();

/* metodo deleta espaço alocado dinamicamente para registro utilizado em tempo de execução */
void deleta_registro(Registro* A);

/* imprime todos os campos do registro */
void print_registro(Registro* A);

/* imprime os registros de um arquivo até o valor de quantidade */
void print_arquivo(FILE* arqb, long QUANTIDADE);

/*altera a chave de um registro */
void set_chave(Registro* A, float x);

#endif //REGISTRO_H_
