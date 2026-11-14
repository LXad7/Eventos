/*
 * Programacao I - Teste Pratico Simulado B
 * Gestao dos socios de um ginasio
 *
 * Numero: ________     Nome: ______________________________
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_SOCIOS 80
#define MAX_NOME 50
#define AULAS_REF 10
#define FICH_BINARIO "socios.dat"
#define FICH_TEXTO "socios_antigos.txt"

/* b) Tipos de dados */
typedef struct
{
    int dia;
    int mes;
    int ano;
} tipoData;

typedef struct
{
    int numero;
    char nome[MAX_NOME];
    float mensalidade;
    tipoData dataInscricao;
    int aulas;
} tipoSocio;

/* Prototipos */
char menu(int quantSocios, float totalMensal, tipoSocio vetor[MAX_SOCIOS], int posMaisAulas);
tipoSocio lerDadosSocio(void);
int procuraSocio(tipoSocio vetor[MAX_SOCIOS], int quantSocios, int numero);
int acrescentaSocio(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
void mostraSocio(tipoSocio socio);
void mostraSocios(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
float totalMensalidades(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
int socioMaisAulas(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
void percentagemMaisAulas(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
int inscritosAntes2023(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
void registaAula(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
int eliminaSocio(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
void gravaFicheiroBinario(tipoSocio vetor[MAX_SOCIOS], int quantSocios);
int leFicheiroBinario(tipoSocio vetor[MAX_SOCIOS]);

/* Funcoes para leitura de dados (disponibilizadas nas aulas) */
void limpaBufferStdin(void);
int lerInteiro(int min, int max);
float lerFloat(float min, float max);
void lerString(char vetor[], int max);
tipoData lerData(void);

/* a) Estrutura do menu na funcao main() */
int main(void)
{
    tipoSocio socios[MAX_SOCIOS];      /* c) estrutura de dados */
    int quantSocios = 0;
    char opcao;

    do
    {
        opcao = menu(quantSocios, totalMensalidades(socios, quantSocios), socios,
                     socioMaisAulas(socios, quantSocios));

        switch (opcao)
        {
        case 'N':
            quantSocios = acrescentaSocio(socios, quantSocios);
            break;
        case 'L':
            mostraSocios(socios, quantSocios);
            break;
        case 'P':
            percentagemMaisAulas(socios, quantSocios);
            break;
        case 'I':
            printf("\nSocios inscritos antes de 2023: %d (ver %s)\n",
                   inscritosAntes2023(socios, quantSocios), FICH_TEXTO);
            break;
        case 'A':
            registaAula(socios, quantSocios);
            break;
        case 'X':
            quantSocios = eliminaSocio(socios, quantSocios);
            break;
        case 'E':
            gravaFicheiroBinario(socios, quantSocios);
            break;
        case 'R':
            quantSocios = leFicheiroBinario(socios);
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
char menu(int quantSocios, float totalMensal, tipoSocio vetor[MAX_SOCIOS], int posMaisAulas)
{
    char op;

    printf("\n\n************** Ginasio **************\n");
    printf(" Socios: %d\t Total das mensalidades: %.2f euros\n", quantSocios, totalMensal);
    if (posMaisAulas == -1)
    {
        printf(" Socio com mais aulas: ---\n");
    }
    else
    {
        printf(" Socio com mais aulas: %s (%d aulas)\n", vetor[posMaisAulas].nome, vetor[posMaisAulas].aulas);
    }
    printf("\n N - Novo socio");
    printf("\n L - Listar socios");
    printf("\n P - Percentagem de socios com mais de 10 aulas");
    printf("\n I - Socios inscritos antes de 2023");
    printf("\n A - Registar aula de um socio");
    printf("\n X - Eliminar socio");
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

tipoSocio lerDadosSocio(void)
{
    tipoSocio socio;

    printf("Numero de socio (1 a 9999): ");
    socio.numero = lerInteiro(1, 9999);
    printf("Nome: ");
    lerString(socio.nome, MAX_NOME);
    printf("Mensalidade (euros): ");
    socio.mensalidade = lerFloat(0, 500);
    printf("Data de inscricao:");
    socio.dataInscricao = lerData();
    socio.aulas = 0;                 /* novo socio ainda nao tem aulas */

    return socio;
}

int procuraSocio(tipoSocio vetor[MAX_SOCIOS], int quantSocios, int numero)
{
    int i, pos = -1;

    for (i = 0; i < quantSocios; i++)
    {
        if (vetor[i].numero == numero)
        {
            pos = i;
            i = quantSocios;
        }
    }
    return pos;
}

/* d) Novo socio com numero unico */
int acrescentaSocio(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    tipoSocio socio;

    if (quantSocios == MAX_SOCIOS)
    {
        printf("\nImpossivel acrescentar: limite de %d socios atingido\n", MAX_SOCIOS);
    }
    else
    {
        socio = lerDadosSocio();
        if (procuraSocio(vetor, quantSocios, socio.numero) != -1)
        {
            printf("\nJa existe um socio com o numero %d - nao foi acrescentado\n", socio.numero);
        }
        else
        {
            vetor[quantSocios] = socio;
            quantSocios++;
        }
    }
    return quantSocios;
}

/* e) Mostra um socio */
void mostraSocio(tipoSocio socio)
{
    printf("\n %4d  %-25s %8.2f   %02d/%02d/%04d  %4d", socio.numero, socio.nome, socio.mensalidade,
           socio.dataInscricao.dia, socio.dataInscricao.mes, socio.dataInscricao.ano, socio.aulas);
}

/* f) Mostra todos os socios */
void mostraSocios(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    int i;

    if (quantSocios == 0)
    {
        printf("\nNao existem socios registados\n");
    }
    else
    {
        printf("\n Num.  Nome                      Mensal.   Inscricao   Aulas");
        for (i = 0; i < quantSocios; i++)
        {
            mostraSocio(vetor[i]);
        }
        printf("\n");
    }
}

/* g) Total das mensalidades */
float totalMensalidades(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    int i;
    float total = 0;

    for (i = 0; i < quantSocios; i++)
    {
        total += vetor[i].mensalidade;
    }
    return total;
}

/* g) Posicao do socio com mais aulas (-1 se nao houver socios) */
int socioMaisAulas(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    int i, pos = -1;

    if (quantSocios > 0)
    {
        pos = 0;
        for (i = 1; i < quantSocios; i++)
        {
            if (vetor[i].aulas > vetor[pos].aulas)
            {
                pos = i;
            }
        }
    }
    return pos;
}

/* h) Percentagem de socios com mais de 10 aulas + listagem */
void percentagemMaisAulas(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    int i, conta = 0;
    float percentagem;

    if (quantSocios == 0)
    {
        printf("\nNao existem socios registados\n");
    }
    else
    {
        for (i = 0; i < quantSocios; i++)
        {
            if (vetor[i].aulas > AULAS_REF)
            {
                mostraSocio(vetor[i]);
                conta++;
            }
        }
        percentagem = (float) conta * 100 / quantSocios;
        printf("\n\nSocios com mais de 10 aulas: %d (%.2f%%)\n", conta, percentagem);
    }
}

/* i) Inscritos antes de 1 de janeiro de 2023 + ficheiro de texto */
int inscritosAntes2023(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    FILE *ficheiro;
    int i, conta = 0;

    ficheiro = fopen(FICH_TEXTO, "w");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_TEXTO);
    }
    for (i = 0; i < quantSocios; i++)
    {
        if (vetor[i].dataInscricao.ano < 2023)
        {
            conta++;
            if (ficheiro != NULL)
            {
                fprintf(ficheiro, "%d\t%s\t%.2f\t%02d/%02d/%04d\t%d\n", vetor[i].numero, vetor[i].nome,
                        vetor[i].mensalidade, vetor[i].dataInscricao.dia, vetor[i].dataInscricao.mes,
                        vetor[i].dataInscricao.ano, vetor[i].aulas);
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

/* j) Registar uma aula do socio com o numero indicado */
void registaAula(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    int numero, pos;

    printf("Numero de socio: ");
    numero = lerInteiro(1, 9999);
    pos = procuraSocio(vetor, quantSocios, numero);
    if (pos == -1)
    {
        printf("\nNao existe nenhum socio com o numero %d\n", numero);
    }
    else
    {
        vetor[pos].aulas++;
        printf("\n%s tem agora %d aulas\n", vetor[pos].nome, vetor[pos].aulas);
    }
}

/* k) Eliminar o socio com o numero indicado */
int eliminaSocio(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    int numero, pos, i;

    printf("Numero do socio a eliminar: ");
    numero = lerInteiro(1, 9999);
    pos = procuraSocio(vetor, quantSocios, numero);
    if (pos == -1)
    {
        printf("\nNao existe nenhum socio com o numero %d\n", numero);
    }
    else
    {
        for (i = pos; i < quantSocios - 1; i++)
        {
            vetor[i] = vetor[i + 1];
        }
        quantSocios--;
        printf("\nSocio %d eliminado\n", numero);
    }
    return quantSocios;
}

/* l) Escrever ficheiro binario */
void gravaFicheiroBinario(tipoSocio vetor[MAX_SOCIOS], int quantSocios)
{
    FILE *ficheiro;

    ficheiro = fopen(FICH_BINARIO, "wb");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_BINARIO);
    }
    else
    {
        fwrite(&quantSocios, sizeof(int), 1, ficheiro);
        fwrite(vetor, sizeof(tipoSocio), quantSocios, ficheiro);
        fclose(ficheiro);
        printf("\nDados de %d socios gravados\n", quantSocios);
    }
}

/* m) Ler ficheiro binario */
int leFicheiroBinario(tipoSocio vetor[MAX_SOCIOS])
{
    FILE *ficheiro;
    int quantSocios = 0;

    ficheiro = fopen(FICH_BINARIO, "rb");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_BINARIO);
    }
    else
    {
        if (fread(&quantSocios, sizeof(int), 1, ficheiro) != 1 || quantSocios < 0 || quantSocios > MAX_SOCIOS)
        {
            quantSocios = 0;
        }
        else if (fread(vetor, sizeof(tipoSocio), quantSocios, ficheiro) != (size_t) quantSocios)
        {
            quantSocios = 0;
        }
        fclose(ficheiro);
        printf("\nDados de %d socios lidos\n", quantSocios);
    }
    return quantSocios;
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
