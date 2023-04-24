#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define NOME_TAMANHO 50
#define CPF_TAMANHO 12
#define MATRICULA_TAMANHO 20
#define DATA_TAMANHO 10
#define DATA_COMECO 86
#define BUFFER_SIZE 1000
#define LINHA_TAMANHO 107

struct Aluno{
    char nome[NOME_TAMANHO];
    char cpf[CPF_TAMANHO];
    struct tm dataNascimento;
    char matricula[MATRICULA_TAMANHO];
    float notas[2];
};

typedef struct Aluno Aluno;

FILE *tentarAbrirArquivo(const char *modo);
void esperarInput();
void tentarCriarIndice(FILE *arquivo, long *indice, long linhas);
void criarAluno(Aluno *aluno);
void formatarAluno(Aluno aluno, char *alunoFormatado);
char *pegarResto(char *nome, short tamanhoTotal); 
long pegarArquivoTamanho(FILE *arquivo);
int pegarIdade(struct tm hoje, struct tm aniversario);
bool validarCpf(char *cpf);

FILE *tentarAbrirArquivo(const char *modo) {
    FILE *arquivo = NULL; 
    arquivo = fopen("alunos.txt", *(&modo));

    if(arquivo == NULL){
        printf("Arquivo inacessível ou não existe.\n");
        exit(EXIT_FAILURE);
    }

    return arquivo;
}

void esperarInput() {
	printf("Digite qualquer tecla para voltar ao menu.\n");
	getchar();
	getchar();
	printf("\n");
}

void tentarCriarIndice(FILE *arquivo, long *indice, long linhas) {
    printf("Digite o indice do aluno: (começando de 0) ");
    scanf("%ld", indice);

    if(linhas <= *indice && *indice < 0){
        printf("Índice maior que quantidade de linhas ou não válido.\n");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    *indice = (*indice) * LINHA_TAMANHO;
}

void criarAluno(Aluno *aluno) {
    printf("Nome: ");
    scanf(" %49[^\n]s", aluno->nome);

    printf("cpf: FORMATO(xxxxxxxxxxx) ");
    scanf(" %11[^\n]s", aluno->cpf);

    // validacao do CPF:
    bool ehValido = validarCpf(aluno->cpf);
    while(ehValido){
        printf("CPF inválido! Digite novamente: ");
        scanf(" %11[^\n]s", aluno->cpf);
        ehValido = validarCpf(aluno->cpf);
    }

    printf("Data de Nascimento: FORMATO(dd MM yyyy)\n");
    printf("Dia: ");
    scanf("%d", &aluno->dataNascimento.tm_mday);
    printf("Mês: ");
    scanf("%d", &aluno->dataNascimento.tm_mon);
    printf("Ano: ");
    scanf("%d", &aluno->dataNascimento.tm_year);

    printf("Matricula: ");
    scanf(" %19[^\n]s", aluno->matricula);

    printf("Notas: \n");
    printf("Primeira nota: ");
    scanf("%f", &aluno->notas[0]);
    printf("Segunda nota: ");
    scanf("%f", &aluno->notas[1]);
}

void formatarAluno(Aluno aluno, char *alunoFormatado) {
    char *nomeResto = pegarResto(aluno.nome, NOME_TAMANHO);
    char *matriculaResto = pegarResto(aluno.matricula, MATRICULA_TAMANHO);

    snprintf(
        alunoFormatado, 
        LINHA_TAMANHO, 
        "%s%s %s %s%s %.2d %.2d %.4d %.2f %.2f\n",
        aluno.nome,
        nomeResto,
        aluno.cpf, 
        aluno.matricula,
        matriculaResto,
        aluno.dataNascimento.tm_mday,
        aluno.dataNascimento.tm_mon,
        aluno.dataNascimento.tm_year,
        aluno.notas[0],
        aluno.notas[1]
    );

    free(nomeResto);
    free(matriculaResto);
}

char* pegarResto(char *nome, short tamanhoTotal) {
	short tamanhoUsado = strlen(nome);
    short tamanhoResto = tamanhoTotal - tamanhoUsado + 1;

    char *resto = malloc(tamanhoResto);

    for (size_t i = 0; i < tamanhoResto; i++)
        resto[i] = ' ';

	return resto;
}

long pegarArquivoTamanho(FILE *arquivo) {
	long arquivoTamanho;

	fseek(arquivo, 0, SEEK_END);
    arquivoTamanho = ftell(arquivo);
    rewind(arquivo);

	return arquivoTamanho;
}

int pegarIdade(struct tm hoje, struct tm aniversario) {
    int idade = hoje.tm_year - aniversario.tm_year;

    bool antesDoMesDoAniversario = hoje.tm_mon < aniversario.tm_mon;
    bool antesDoDiaDoAniversario = (
        hoje.tm_mon == aniversario.tm_mon && 
        hoje.tm_mday < aniversario.tm_mday
    );

    if (antesDoMesDoAniversario || antesDoDiaDoAniversario) idade--;

    return idade;
}

bool validarCpf(char *cpf){
    short digito1 = 0, digito2 = 0, ascii = 0;

    for(int i = 0, j = 10; i < strlen(cpf) - 2; i++, j--){
        ascii = (short)(cpf[i]);
        digito1 += ( ascii - 48 ) * j;
    }
    
    digito1 %= 11;
    
    digito1 = (digito1 < 2) ? 0 : 11 - digito1;

    if((cpf[9] - 48) != digito1) return true;

    for(int i = 0, j = 11; i < strlen(cpf)-1; i++, j--){
        ascii = (short)(cpf[i]);
        digito2 += (ascii - 48) * j;
    }

    digito2 %= 11;

    digito2 = (digito2 < 2) ? 0 : 11 - digito2;
    
    ascii = (short)(cpf[10]);

    if((ascii - 48) != digito2) return true;
    
    return false;
}