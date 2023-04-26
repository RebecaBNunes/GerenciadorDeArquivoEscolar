#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "utils.h"

void menu();
void cadastrar();
void consultar();
void alterar();
void remover();

int main(){
    menu();

    return 0;
}

void menu(){
    int escolha;

    while(escolha != 5){
        printf("-----------------------------\n");
        printf("             Menu            \n");
        printf("-----------------------------\n");
        printf("%d: %10s\n", 1, "Cadastrar Aluno\n");
        printf("%d: %10s\n", 2, "Consultar Aluno\n");
        printf("%d: %10s\n", 3, "Remover aluno\n");
        printf("%d: %10s\n", 4, "Alterar aluno\n");
        printf("%d: %10s\n", 5, "Sair do programa\n");
        printf("-----------------------------\n");
        printf("Escolha uma opção: (1 a 5): \n");
        scanf("%d", &escolha);

        switch(escolha){
            case 1: cadastrar(); break;
            case 2: consultar(); break;
            case 3: remover(); break;
            case 4: alterar(); break;
            case 5: printf("Encerrando programa..."); break;
            default: printf("Opção inválida, escolha novamente!\n"); break;
        }
    }
}

void cadastrar(){
    FILE *arquivo = tentarAbrirArquivo("a+");

    printf("-----------------------------\n");
    printf("      CADASTRO DE ALUNO:     \n");
    printf("-----------------------------\n");
    printf("Preencha os campos solicitados\n");
    printf("-----------------------------\n");
    
    Aluno aluno;
    criarAluno(&aluno);

    fprintf(
        arquivo, 
        "%-49s %s %-19s %.2d %.2d %.4d %.2f %.2f\n", 
        aluno.nome,
        aluno.cpf, 
        aluno.matricula,
        aluno.dataNascimento.tm_mday,
        aluno.dataNascimento.tm_mon,
        aluno.dataNascimento.tm_year,
        aluno.notas[0],
        aluno.notas[1]
    );

    fclose(arquivo);
    esperarInput();
}

void consultar(){
    FILE *arquivo = tentarAbrirArquivo("r");

    printf("\n-----------------------------\n");
    printf("      CONSULTA DE ALUNOS:    \n");
    printf("-----------------------------\n");

    char ch;
    long arquivoTamanho = pegarArquivoTamanho(arquivo);
    long linhas = arquivoTamanho / LINHA_TAMANHO;
    long datas[linhas];

    while((ch = getc(arquivo)) != EOF){
        long posicaoAtual = ftell(arquivo);
        long linhaAtual = posicaoAtual / LINHA_TAMANHO;

        bool ehInicioDaData = posicaoAtual == linhaAtual * LINHA_TAMANHO + DATA_COMECO;
        
        if(ehInicioDaData){
            char data[DATA_TAMANHO];            
            for (size_t i = 0; i < DATA_TAMANHO; i++)
                data[i] = getc(arquivo);
            
            int dia, mes, ano; 
            sscanf(data, "%d %d %d", &dia, &mes, &ano);

            bool ehValido = data[0] != ' ';
            if(!ehValido) {
                printf(" ");
                fseek(arquivo, - DATA_TAMANHO + 1, SEEK_CUR);
                continue;
            }

            struct tm dataFormatada = {
                .tm_year = ano - 1900,
                .tm_mon = mes - 1,
                .tm_mday = dia 
            };        

            time_t hoje = time(NULL);
            struct tm *hojeFormatado = localtime(&hoje);
            
            printf("%d", pegarIdade(*hojeFormatado, dataFormatada));
            
            fseek(arquivo, -DATA_TAMANHO + 1, SEEK_CUR);
        } else if (
            posicaoAtual > linhaAtual * LINHA_TAMANHO + DATA_COMECO &&
            posicaoAtual <= linhaAtual * LINHA_TAMANHO + (DATA_COMECO + DATA_TAMANHO)
        ){
            printf(" ");
        } else {
            printf("%c", ch);
        }
    }

    printf("\n");

    fclose(arquivo);
    esperarInput();
}

void alterar(){
    FILE *arquivo = tentarAbrirArquivo("r+");

    printf("\n-----------------------------\n");
    printf("     ALTERAÇÃO DE ALUNO:     \n");
    printf("-----------------------------\n");

    long arquivoTamanho = pegarArquivoTamanho(arquivo);
    long linhas = arquivoTamanho / LINHA_TAMANHO;

    long indice = -1;
    tentarCriarIndice(arquivo, &indice, linhas);

    Aluno aluno;
    criarAluno(&aluno);

    char alunoFormatado[LINHA_TAMANHO];
    formatarAluno(aluno, alunoFormatado);

    fseek(arquivo, indice, SEEK_SET);

    for (size_t i = 0; i < LINHA_TAMANHO - 1; i++)        
        fputc(alunoFormatado[i], arquivo);

    printf("\n");
    printf("Aluno alterado com sucesso!\n");

    fclose(arquivo);
    esperarInput();
}

void remover(){
    FILE *arquivo = tentarAbrirArquivo("r+");

    printf("\n-----------------------------\n");
    printf("     REMOVEÇÃO DE ALUNOS:    \n");
    printf("-----------------------------\n");

    long arquivoTamanho = pegarArquivoTamanho(arquivo);
    long linhas = arquivoTamanho / LINHA_TAMANHO;

    long indice = -1;
    tentarCriarIndice(arquivo, &indice, linhas);

    fseek(arquivo, indice, SEEK_SET);

    for (size_t i = 0; i < LINHA_TAMANHO - 1; i++)
        fputc(' ', arquivo);
    
    printf("\n");
    printf("Aluno excluído com sucesso!\n");

    fclose(arquivo);
    esperarInput();
}