#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50
#define TAM_CPF 12
#define TAM_LISTA 20
#define BUFFER_SIZE 1000

struct Data{
    unsigned short dia;
    unsigned short mes;
    unsigned short ano;
};

typedef struct Data Data;

struct Aluno{
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    Data dataNascimento;
    long matricula;
    float notas[4];
};

typedef struct Aluno Aluno;

void menu();
void cadastrar();
int validarCpf(char *cpf);
void consultar();
void remover();

int main(){
    menu();

    return 0;
}

void menu(){
    int escolha;

    while(escolha != 5){
        printf("-----------------------------\n");
        printf("             Menu            ");
        printf("-----------------------------\n");
        printf("%d: %10s\n", 1, "Cadastrar Aluno\n");
        printf("%d: %10s\n", 2, "Consultar Aluno\n");
        printf("%d: %10s\n", 3, "Remover aluno\n");
        printf("%d: %10s\n", 4, "Alterar aluno\n");
        printf("%d: %10s\n", 5, "Sair do programa\n");
        printf("-----------------------------\n");
        printf("Escolha uma opção: (1 a 5): ");
        scanf("%d", &escolha);

        switch(escolha){
            case 1: cadastrar(); break;
            case 2: consultar(); break;
            case 3: remover(); break;
            // case 4: alterar(); break;
            case 5: printf("Encerrando programa..."); break;
            default: printf("Opção inválida, escolha novamente!\n"); break;
        }
    }
}

void cadastrar(){
    FILE *arquivo = NULL;
    Aluno alunos[TAM_LISTA];

    int index = 0;
    char opcao = 'n';

    arquivo = fopen("arquivo-alunos.txt", "a+");
    
    if(arquivo == NULL){
        printf("O arquivo não foi aberto\n");
        exit(EXIT_FAILURE);
    }

    printf("-----------------------------\n");
    printf("      CADASTRO DE ALUNO:     ");
    printf("-----------------------------\n");
    printf("Preencha os campos solicitados\n");
    printf("-----------------------------\n");

    do{
        Aluno aluno = alunos[index];
        
        printf("Nome: ");
        scanf(" %49[^\n]s", aluno.nome);
        printf("cpf: FORMATO(xxxxxxxxxxx) ");
        scanf(" %11[^\n]s", aluno.cpf);

        // validacao do CPF:
        int ehValido = validarCpf(aluno.cpf);
        while(ehValido == 0){
            printf("CPF inválido! Digite novamente: ");
            scanf(" %11[^\n]s", aluno.cpf);
            ehValido = validarCpf(aluno.cpf);
        }

        printf("Data de Nascimento: FORMATO(dd MM yyyy)\n");
        printf("Dia: ");
        scanf("%hd", &aluno.dataNascimento.dia);
        printf("Mês: ");
        scanf("%hd", &aluno.dataNascimento.mes);
        printf("Ano: ");
        scanf("%hd", &aluno.dataNascimento.ano);

        printf("Matricula: ");
        scanf("%ld", &aluno.matricula);

        printf("Notas: \n");
        printf("Primeira nota: ");
        scanf("%f", &aluno.notas[0]);
        printf("Segunda nota: ");
        scanf("%f", &aluno.notas[1]);
        printf("Terceira nota: ");
        scanf("%f", &aluno.notas[2]);
        printf("Quarta nota: ");
        scanf("%f", &aluno.notas[3]);

        fprintf(
            arquivo, 
            "%s %s %ld %hd %hd %hd %.2f %.2f %.2f %.2f\n", 
            aluno.nome, 
            aluno.cpf, 
            aluno.matricula,
            aluno.dataNascimento.dia,
            aluno.dataNascimento.mes,
            aluno.dataNascimento.ano,
            aluno.notas[0],
            aluno.notas[1],
            aluno.notas[2],
            aluno.notas[3]
        );

        index++;

        printf("continuar cadastro de alunos? FORMATO(s / n)");
        scanf(" %c", &opcao);
    } while(opcao != 'n');

    fclose(arquivo);
}

int validarCpf(char *cpf){
    short digito1 = 0, digito2 = 0, ascii = 0;

    for(int i = 0, j = 10; i < strlen(cpf) - 2; i++, j--){
        ascii = (short)(cpf[i]);
        digito1 += ( ascii - 48 ) * j;
    }
    
    digito1 %= 11;
    
    digito1 = (digito1 < 2) ? 0 : 11 - digito1;

    if((cpf[9] - 48) != digito1) return 0;

    for(int i = 0, j = 11; i < strlen(cpf)-1; i++, j--){
        ascii = (short)(cpf[i]);
        digito2 += (ascii - 48) * j;
    }

    digito2 %= 11;

    digito2 = (digito2 < 2) ? 0 : 11 - digito2;
    
    ascii = (short)(cpf[10]);

    if((ascii - 48) != digito2) return 0;
    
    return 1;
}

void consultar(){
    FILE *arquivo = NULL;
    Aluno alunos[TAM_LISTA];
    char ch;

    arquivo = fopen("arquivo-alunos.txt", "r");

    if(arquivo == NULL){
        printf("O arquivo não foi aberto\n");
        exit(EXIT_FAILURE);
    }

    printf("\n-----------------------------\n");
    printf("      LISTAGEM DE ALUNOS:      \n");
    printf("-----------------------------");

    while((ch = getc(arquivo)) != EOF){
        printf("%c", ch);
    }

    printf("\n");

    fclose(arquivo);
}

void remover(){
    FILE *arquivo = NULL;
    // Aluno alunos[TAM_LISTA];
    char ch = 'a';
    long indice = -1;
    long quantidadeDeLinhas = 0;
    long quantidadeParaApagar = 0;
    long indiceReal = 0;

    printf("Digite a linha que você deseja excluir: (começando de 0) ");
    scanf("%ld", &indice);

    arquivo = fopen("arquivo-alunos.txt", "r+");

    if(arquivo == NULL){
        printf("Arquivo inacessível ou não existe.\n");
        exit(EXIT_FAILURE);
    }

    do {
        ch = fgetc(arquivo);
        if(ch == '\n') quantidadeDeLinhas++;
        if(quantidadeDeLinhas < indice) indiceReal++;
    } while(ch != EOF);
    
    indiceReal++;

    if(quantidadeDeLinhas < indice && indice < 0){
        printf("Índice maior que quantidade de linhas ou não válido.\n");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    fseek(arquivo, indiceReal, SEEK_SET);
    
    do {
        ch = fgetc(arquivo);
        quantidadeParaApagar++;
    } while(ch != '\n');

    quantidadeParaApagar--;

    fseek(arquivo, indiceReal, SEEK_SET);

    indice = 0;
    
    do{
        fputc(' ', arquivo);
        indice++;
    } while (indice < quantidadeParaApagar);

    fclose(arquivo);
}