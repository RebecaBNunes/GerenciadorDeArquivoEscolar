#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define TAM_NOME 50
#define TAM_CPF 12
#define TAM_LISTA 20

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

int main()
{
    setlocale(LC_ALL, "Portuguese");

    menu();

    return 0;
}

void menu(){
    int escolha;
    while(escolha != 5){
        printf("-----------------------------\n"
           "%15s\n", "MENU");
        printf("-----------------------------\n");
        printf("%d: %10s\n", 1, "Cadastrar Aluno\n");
        printf("%d: %10s\n", 2, "Consultar Aluno\n");
        printf("%d: %10s\n", 3, "Remover aluno\n");
        printf("%d: %10s\n", 4, "Alterar aluno\n");
        printf("%d: %10s\n", 5, "Sair do programa\n");
        printf("-----------------------------\n");
        printf("Escolha uma op��o: (1 a 5): ");
        scanf("%d", &escolha);

        switch(escolha){
        case 1:
            cadastrar();
            break;
        case 2:
            consultar();
            break;
        case 3:
            // remover();
            break;
        case 4:
            //alterar();
            break;
        case 5:
            printf("Encerrando programa...");
            break;
        default:
            printf("Op��o inv�lida, escolha novamente!\n");
            break;
        }
    }
}

void cadastrar(){
    FILE *registro;
    Aluno alunos[TAM_LISTA];
    int index = 0;
    char opcao = 'n';
    if((registro = fopen("registro-alunos.txt", "a+")) == NULL){
        printf("! Ocorreu algum problema ao tentar abrir o arquivo registro-aluno.txt\n");
    }
    else{
        /*
            /// precisa cadastrar notas.
            /// fazer valida��o do cpf
        */
        printf("\n-----------------------------------------------------------------------\n");
        printf("                            CADASTRO DE ALUNO:                           \n");
        printf("-----------------------------------------------------------------------\n");
        printf("Cadastre os alunos preenchendo os campos solicitados\n");
        printf("-----------------------------------------------------------------------\n");

        do{
            printf("Nome: ");
            scanf(" %49[^\n]s", alunos[index].nome);
            printf("cpf: FORMATO(xxxxxxxxxxx) ");
            scanf(" %11[^\n]s", alunos[index].cpf);

            // validacao do CPF:
            int ehValido = validarCpf(alunos[index].cpf);
            while(ehValido == 0){
                printf("CPF inv�lido! Digite novamente: ");
                scanf(" %11[^\n]s", alunos[index].cpf);
                ehValido = validarCpf(alunos[index].cpf);
            }

            printf("Data de Nascimento: FORMATO(dd MM yyyy)\n");
            printf("Dia: ");
            scanf("%hd", &alunos[index].dataNascimento.dia);
            printf("Mês: ");
            scanf("%hd", &alunos[index].dataNascimento.mes);
            printf("Ano: ");
            scanf("%hd", &alunos[index].dataNascimento.ano);

            printf("Matricula: ");
            scanf("%ld", &alunos[index].matricula);

            printf("Notas: \n");
            printf("Primeira nota: ");
            scanf("%f", &alunos[index].notas[0]);
            printf("Segunda nota: ");
            scanf("%f", &alunos[index].notas[1]);
            printf("Terceira nota: ");
            scanf("%f", &alunos[index].notas[2]);
            printf("Quarta nota: ");
            scanf("%f", &alunos[index].notas[3]);

            fprintf(
                registro, 
                "%s %s %ld %hd %hd %hd %.2f %.2f %.2f %.2f\n", 
                alunos[index].nome, 
                alunos[index].cpf, 
                alunos[index].matricula,
                alunos[index].dataNascimento.dia,
                alunos[index].dataNascimento.mes,
                alunos[index].dataNascimento.ano,
                alunos[index].notas[0],
                alunos[index].notas[1],
                alunos[index].notas[2],
                alunos[index].notas[3]
            );

            index++;

            printf("continuar cadastro de alunos? FORMATO(s / n)");
            scanf(" %c", &opcao);
        } while((opcao != 'n'));

        fclose(registro);
    }
}

// FUNCAO PARA VALIDAR O CPF:
int validarCpf(char *cpf){
    int digito1 = 0;
    int digito2 = 0;
    for(int i = 0, j = 10; i < strlen(cpf)-2; i++, j--){
        digito1 += (cpf[i]-48) * j;
    }
    digito1 %= 11;
    if(digito1 < 2){
        digito1 = 0;
    }
    else {
       digito1 = 11 - digito1;
    }

    if((cpf[9]-48) != digito1){
        return 0;
    }
    for(int i = 0, j =11; i < strlen(cpf)-1; i++, j--){
        digito2 += (cpf[i]-48) * j;
    }
    digito2 %= 11;
    if(digito2 < 2){
        digito2 = 0;
    }
    else{
        digito2 = 11 - digito2;
    }
    if((cpf[10]-48) != digito2){
        return 0;
    }
    return 1;
}

void consultar(){
    FILE *arquivoRead;
    Aluno alunos[TAM_LISTA];
    //int index = 0;
    if((arquivoRead = fopen("registro-alunos.txt", "r")) == NULL){
        printf("! Ocorreu algum problema ao tentar abrir o arquivo registro-aluno.txt\n");
    }
    else{
        printf("\n-----------------------------------------------------------------------\n");
        printf("                            LISTAGEM DE ALUNOS:                           \n");
        printf("-----------------------------------------------------------------------\n");

        while(!feof(arquivoRead)){
            //c�digo referente � leitura do arquivo com os registros dos alunos

        }

        fclose(arquivoRead);
    }
}
