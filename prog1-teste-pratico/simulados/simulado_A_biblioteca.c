/*
 * Programacao I - Teste Pratico Simulado A
 * Gestao dos livros de uma biblioteca
 *
 * Numero: ________     Nome: ______________________________
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LIVROS 100
#define MAX_TITULO 80
#define PRECO_REF 15.0
#define FICH_BINARIO "livros.dat"
#define FICH_TEXTO "aquisicoes.txt"

/* b) Tipos de dados */
typedef struct
{
    int dia;
    int mes;
    int ano;
} tipoData;

typedef struct
{
    int codigo;
    char titulo[MAX_TITULO];
    float preco;
    int exemplares;
    tipoData dataAquisicao;
} tipoLivro;

/* Prototipos */
char menu(int quantLivros, int totalEx, float precoMed, int quantRecentes);
tipoLivro lerDadosLivro(void);
int procuraLivro(tipoLivro vetor[MAX_LIVROS], int quantLivros, int codigo);
int acrescentaLivro(tipoLivro vetor[MAX_LIVROS], int quantLivros);
void mostraLivro(tipoLivro livro);
void mostraLivros(tipoLivro vetor[MAX_LIVROS], int quantLivros);
int totalExemplares(tipoLivro vetor[MAX_LIVROS], int quantLivros);
float precoMedio(tipoLivro vetor[MAX_LIVROS], int quantLivros);
void percentagemPrecoInferior(tipoLivro vetor[MAX_LIVROS], int quantLivros);
int adquiridosDesdeMarco2024(tipoLivro vetor[MAX_LIVROS], int quantLivros);
void alteraExemplares(tipoLivro vetor[MAX_LIVROS], int quantLivros);
void gravaFicheiroBinario(tipoLivro vetor[MAX_LIVROS], int quantLivros);
int leFicheiroBinario(tipoLivro vetor[MAX_LIVROS]);

/* Funcoes para leitura de dados (disponibilizadas nas aulas) */
void limpaBufferStdin(void);
int lerInteiro(int min, int max);
float lerFloat(float min, float max);
void lerString(char vetor[], int max);
tipoData lerData(void);

/* a) Estrutura do menu na funcao main() */
int main(void)
{
    tipoLivro livros[MAX_LIVROS];      /* c) estrutura de dados */
    int quantLivros = 0;
    char opcao;

    do
    {
        opcao = menu(quantLivros, totalExemplares(livros, quantLivros), precoMedio(livros, quantLivros),
                     adquiridosDesdeMarco2024(livros, quantLivros));

        switch (opcao)
        {
        case 'N':
            quantLivros = acrescentaLivro(livros, quantLivros);
            break;
        case 'L':
            mostraLivros(livros, quantLivros);
            break;
        case 'P':
            percentagemPrecoInferior(livros, quantLivros);
            break;
        case 'A':
            alteraExemplares(livros, quantLivros);
            break;
        case 'E':
            gravaFicheiroBinario(livros, quantLivros);
            break;
        case 'R':
            quantLivros = leFicheiroBinario(livros);
            break;
        case 'S':
            printf("\nFim do programa\n");
            break;
        default:
            printf("\nOpcao invalida\n");
        }
    }
    while (opcao != 'S');

    return 0;
}

/* a) Menu de opcoes */
char menu(int quantLivros, int totalEx, float precoMed, int quantRecentes)
{
    char op;

    printf("\n\n************** Biblioteca **************\n");
    printf(" Livros: %d\t Exemplares: %d\t Preco medio: %.2f euros\n", quantLivros, totalEx, precoMed);
    printf(" Adquiridos desde 1 de marco de 2024: %d\n", quantRecentes);
    printf("\n N - Novo livro");
    printf("\n L - Listar livros");
    printf("\n P - Percentagem de livros com preco inferior a 15 euros");
    printf("\n A - Alterar numero de exemplares");
    printf("\n E - Escrever dados num ficheiro binario");
    printf("\n R - Ler dados do ficheiro binario");
    printf("\n S - Sair");
    printf("\n\t Opcao --> ");

    op = getchar();
    op = toupper(op);
    if (op != '\n' && op != EOF)
    {
        limpaBufferStdin();
    }
    return op;
}

tipoLivro lerDadosLivro(void)
{
    tipoLivro livro;

    printf("Codigo do livro (1 a 99999): ");
    livro.codigo = lerInteiro(1, 99999);
    printf("Titulo: ");
    lerString(livro.titulo, MAX_TITULO);
    printf("Preco (euros): ");
    livro.preco = lerFloat(0, 1000);
    printf("Numero de exemplares: ");
    livro.exemplares = lerInteiro(1, 1000);
    printf("Data de aquisicao:");
    livro.dataAquisicao = lerData();

    return livro;
}

int procuraLivro(tipoLivro vetor[MAX_LIVROS], int quantLivros, int codigo)
{
    int i, pos = -1;

    for (i = 0; i < quantLivros; i++)
    {
        if (vetor[i].codigo == codigo)
        {
            pos = i;
            i = quantLivros;
        }
    }
    return pos;
}

/* d) Novo livro com codigo unico */
int acrescentaLivro(tipoLivro vetor[MAX_LIVROS], int quantLivros)
{
    tipoLivro livro;

    if (quantLivros == MAX_LIVROS)
    {
        printf("\nImpossivel acrescentar: limite de %d livros atingido\n", MAX_LIVROS);
    }
    else
    {
        livro = lerDadosLivro();
        if (procuraLivro(vetor, quantLivros, livro.codigo) != -1)
        {
            printf("\nJa existe um livro com o codigo %d - nao foi acrescentado\n", livro.codigo);
        }
        else
        {
            vetor[quantLivros] = livro;
            quantLivros++;
        }
    }
    return quantLivros;
}

/* e) Mostra um livro */
void mostraLivro(tipoLivro livro)
{
    printf("\n %5d  %-30s %8.2f %5d   %02d/%02d/%04d", livro.codigo, livro.titulo, livro.preco,
           livro.exemplares, livro.dataAquisicao.dia, livro.dataAquisicao.mes, livro.dataAquisicao.ano);
}

/* f) Mostra todos os livros */
void mostraLivros(tipoLivro vetor[MAX_LIVROS], int quantLivros)
{
    int i;

    if (quantLivros == 0)
    {
        printf("\nNao existem livros registados\n");
    }
    else
    {
        printf("\n Cod.   Titulo                            Preco   Ex.   Aquisicao");
        for (i = 0; i < quantLivros; i++)
        {
            mostraLivro(vetor[i]);
        }
        printf("\n");
    }
}

/* g) Total de exemplares */
int totalExemplares(tipoLivro vetor[MAX_LIVROS], int quantLivros)
{
    int i, total = 0;

    for (i = 0; i < quantLivros; i++)
    {
        total += vetor[i].exemplares;
    }
    return total;
}

/* g) Preco medio dos livros */
float precoMedio(tipoLivro vetor[MAX_LIVROS], int quantLivros)
{
    int i;
    float soma = 0, media = 0;

    for (i = 0; i < quantLivros; i++)
    {
        soma += vetor[i].preco;
    }
    if (quantLivros > 0)
    {
        media = soma / quantLivros;
    }
    return media;
}

/* h) Percentagem de livros com preco inferior a 15 euros + listagem */
void percentagemPrecoInferior(tipoLivro vetor[MAX_LIVROS], int quantLivros)
{
    int i, conta = 0;
    float percentagem;

    if (quantLivros == 0)
    {
        printf("\nNao existem livros registados\n");
    }
    else
    {
        for (i = 0; i < quantLivros; i++)
        {
            if (vetor[i].preco < PRECO_REF)
            {
                mostraLivro(vetor[i]);
                conta++;
            }
        }
        percentagem = (float) conta * 100 / quantLivros;
        printf("\n\nLivros com preco inferior a 15 euros: %d (%.2f%%)\n", conta, percentagem);
    }
}

/* i) Livros adquiridos desde 1 de marco de 2024 (inclusive) + ficheiro de texto */
int adquiridosDesdeMarco2024(tipoLivro vetor[MAX_LIVROS], int quantLivros)
{
    FILE *ficheiro;
    int i, conta = 0;
    tipoData d;

    ficheiro = fopen(FICH_TEXTO, "w");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_TEXTO);
    }
    for (i = 0; i < quantLivros; i++)
    {
        d = vetor[i].dataAquisicao;
        if (d.ano > 2024 || (d.ano == 2024 && d.mes >= 3))
        {
            conta++;
            if (ficheiro != NULL)
            {
                fprintf(ficheiro, "%d\t%s\t%.2f\t%d\t%02d/%02d/%04d\n", vetor[i].codigo, vetor[i].titulo,
                        vetor[i].preco, vetor[i].exemplares, d.dia, d.mes, d.ano);
            }
        }
    }
    if (ficheiro != NULL)
    {
        fprintf(ficheiro, "Total: %d\n", conta);
        fclose(ficheiro);
    }
    return conta;
}

/* j) Alterar o numero de exemplares */
void alteraExemplares(tipoLivro vetor[MAX_LIVROS], int quantLivros)
{
    int codigo, pos;

    printf("Codigo do livro: ");
    codigo = lerInteiro(1, 99999);
    pos = procuraLivro(vetor, quantLivros, codigo);
    if (pos == -1)
    {
        printf("\nNao existe nenhum livro com o codigo %d\n", codigo);
    }
    else
    {
        printf("Exemplares atuais: %d\nNovo numero de exemplares: ", vetor[pos].exemplares);
        vetor[pos].exemplares = lerInteiro(0, 1000);
    }
}

/* k) Escrever ficheiro binario */
void gravaFicheiroBinario(tipoLivro vetor[MAX_LIVROS], int quantLivros)
{
    FILE *ficheiro;

    ficheiro = fopen(FICH_BINARIO, "wb");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_BINARIO);
    }
    else
    {
        fwrite(&quantLivros, sizeof(int), 1, ficheiro);
        fwrite(vetor, sizeof(tipoLivro), quantLivros, ficheiro);
        fclose(ficheiro);
        printf("\nDados de %d livros gravados\n", quantLivros);
    }
}

/* l) Ler ficheiro binario */
int leFicheiroBinario(tipoLivro vetor[MAX_LIVROS])
{
    FILE *ficheiro;
    int quantLivros = 0;

    ficheiro = fopen(FICH_BINARIO, "rb");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_BINARIO);
    }
    else
    {
        if (fread(&quantLivros, sizeof(int), 1, ficheiro) != 1 || quantLivros < 0 || quantLivros > MAX_LIVROS)
        {
            quantLivros = 0;
        }
        else if (fread(vetor, sizeof(tipoLivro), quantLivros, ficheiro) != (size_t) quantLivros)
        {
            quantLivros = 0;
        }
        fclose(ficheiro);
        printf("\nDados de %d livros lidos\n", quantLivros);
    }
    return quantLivros;
}

/************ Funcoes para leitura de dados ************/

void limpaBufferStdin(void)
{
    int lixo;

    do
    {
        lixo = getchar();
    }
    while (lixo != '\n' && lixo != EOF);
}

int lerInteiro(int min, int max)
{
    int numero = 0, controlo;

    do
    {
        controlo = scanf("%d", &numero);
        limpaBufferStdin();
    }
    while (numero < min || numero > max || controlo == 0);

    return numero;
}

float lerFloat(float min, float max)
{
    float numero = 0;
    int controlo;

    do
    {
        controlo = scanf("%f", &numero);
        limpaBufferStdin();
    }
    while (numero < min || numero > max || controlo == 0);

    return numero;
}

void lerString(char vetor[], int max)
{
    int tamanhoString;

    do
    {
        fgets(vetor, max, stdin);
        tamanhoString = strlen(vetor);
    }
    while (tamanhoString == 1);    /* repete se so foi introduzido o \n */

    if (vetor[tamanhoString - 1] != '\n')
    {
        limpaBufferStdin();
    }
    else
    {
        vetor[tamanhoString - 1] = '\0';
    }
}

tipoData lerData(void)
{
    tipoData data;
    int maxDiaMes;

    printf("\n Ano (%d a %d): ", 1950, 2026);
    data.ano = lerInteiro(1950, 2026);

    printf(" Mes (%d a %d): ", 1, 12);
    data.mes = lerInteiro(1, 12);

    switch (data.mes)
    {
    case 2:
        if ((data.ano % 400 == 0) || (data.ano % 4 == 0 && data.ano % 100 != 0))
        {
            maxDiaMes = 29;
        }
        else
        {
            maxDiaMes = 28;
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        maxDiaMes = 30;
        break;
    default:
        maxDiaMes = 31;
    }
    printf(" Dia (%d a %d): ", 1, maxDiaMes);
    data.dia = lerInteiro(1, maxDiaMes);

    return data;
}
