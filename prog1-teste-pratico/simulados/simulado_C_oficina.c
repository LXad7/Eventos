/*
 * Programacao I - Teste Pratico Simulado C
 * Gestao das reparacoes de uma oficina
 *
 * Numero: ________     Nome: ______________________________
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_REPARACOES 60
#define MAX_MATRICULA 10
#define MAX_DESCRICAO 60
#define PENDENTE 0
#define CONCLUIDA 1
#define FICH_BINARIO "reparacoes.dat"
#define FICH_TEXTO "viatura.txt"

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
    char matricula[MAX_MATRICULA];
    char descricao[MAX_DESCRICAO];
    float valor;
    tipoData dataEntrada;
    int estado;                      /* PENDENTE ou CONCLUIDA */
} tipoReparacao;

/* Prototipos */
char menu(int quantRep, int quantPendentes, float totalConcluidas);
tipoReparacao lerDadosReparacao(void);
int procuraReparacao(tipoReparacao vetor[MAX_REPARACOES], int quantRep, int codigo);
int acrescentaReparacao(tipoReparacao vetor[MAX_REPARACOES], int quantRep);
void mostraReparacao(tipoReparacao rep);
void mostraReparacoes(tipoReparacao vetor[MAX_REPARACOES], int quantRep);
float valorConcluidas(tipoReparacao vetor[MAX_REPARACOES], int quantRep, int *quantPendentes);
void percentagemPendentes(tipoReparacao vetor[MAX_REPARACOES], int quantRep);
int reparacoesViatura(tipoReparacao vetor[MAX_REPARACOES], int quantRep);
void concluiReparacao(tipoReparacao vetor[MAX_REPARACOES], int quantRep);
void listaMesAno(tipoReparacao vetor[MAX_REPARACOES], int quantRep);
void gravaFicheiroBinario(tipoReparacao vetor[MAX_REPARACOES], int quantRep);
int leFicheiroBinario(tipoReparacao vetor[MAX_REPARACOES]);

/* Funcoes para leitura de dados (disponibilizadas nas aulas) */
void limpaBufferStdin(void);
int lerInteiro(int min, int max);
float lerFloat(float min, float max);
void lerString(char vetor[], int max);
tipoData lerData(void);

/* a) Estrutura do menu na funcao main() */
int main(void)
{
    tipoReparacao reparacoes[MAX_REPARACOES];   /* c) estrutura de dados */
    int quantRep = 0, quantPendentes = 0, quant;
    float totalConcluidas;
    char opcao;

    do
    {
        totalConcluidas = valorConcluidas(reparacoes, quantRep, &quantPendentes);
        opcao = menu(quantRep, quantPendentes, totalConcluidas);

        switch (opcao)
        {
        case 'N':
            quantRep = acrescentaReparacao(reparacoes, quantRep);
            break;
        case 'L':
            mostraReparacoes(reparacoes, quantRep);
            break;
        case 'P':
            percentagemPendentes(reparacoes, quantRep);
            break;
        case 'V':
            quant = reparacoesViatura(reparacoes, quantRep);
            printf("\nReparacoes dessa viatura: %d (ver %s)\n", quant, FICH_TEXTO);
            break;
        case 'C':
            concluiReparacao(reparacoes, quantRep);
            break;
        case 'M':
            listaMesAno(reparacoes, quantRep);
            break;
        case 'E':
            gravaFicheiroBinario(reparacoes, quantRep);
            break;
        case 'R':
            quantRep = leFicheiroBinario(reparacoes);
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
char menu(int quantRep, int quantPendentes, float totalConcluidas)
{
    char op;

    printf("\n\n************** Oficina **************\n");
    printf(" Reparacoes: %d\t Pendentes: %d\t Valor das concluidas: %.2f euros\n",
           quantRep, quantPendentes, totalConcluidas);
    printf("\n N - Nova reparacao");
    printf("\n L - Listar reparacoes");
    printf("\n P - Percentagem de reparacoes pendentes");
    printf("\n V - Reparacoes de uma viatura");
    printf("\n C - Concluir reparacao");
    printf("\n M - Reparacoes de um mes/ano");
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

tipoReparacao lerDadosReparacao(void)
{
    tipoReparacao rep;

    printf("Codigo da reparacao (1 a 99999): ");
    rep.codigo = lerInteiro(1, 99999);
    printf("Matricula (ex: AA-00-BB): ");
    lerString(rep.matricula, MAX_MATRICULA);
    printf("Descricao: ");
    lerString(rep.descricao, MAX_DESCRICAO);
    printf("Valor (euros): ");
    rep.valor = lerFloat(0, 50000);
    printf("Data de entrada:");
    rep.dataEntrada = lerData();
    rep.estado = PENDENTE;           /* uma reparacao nova fica pendente */

    return rep;
}

int procuraReparacao(tipoReparacao vetor[MAX_REPARACOES], int quantRep, int codigo)
{
    int i, pos = -1;

    for (i = 0; i < quantRep; i++)
    {
        if (vetor[i].codigo == codigo)
        {
            pos = i;
            i = quantRep;
        }
    }
    return pos;
}

/* d) Nova reparacao com codigo unico */
int acrescentaReparacao(tipoReparacao vetor[MAX_REPARACOES], int quantRep)
{
    tipoReparacao rep;

    if (quantRep == MAX_REPARACOES)
    {
        printf("\nImpossivel acrescentar: limite de %d reparacoes atingido\n", MAX_REPARACOES);
    }
    else
    {
        rep = lerDadosReparacao();
        if (procuraReparacao(vetor, quantRep, rep.codigo) != -1)
        {
            printf("\nJa existe uma reparacao com o codigo %d - nao foi acrescentada\n", rep.codigo);
        }
        else
        {
            vetor[quantRep] = rep;
            quantRep++;
        }
    }
    return quantRep;
}

/* e) Mostra uma reparacao */
void mostraReparacao(tipoReparacao rep)
{
    printf("\n %5d  %-9s %-25s %9.2f  %02d/%02d/%04d  %s", rep.codigo, rep.matricula, rep.descricao,
           rep.valor, rep.dataEntrada.dia, rep.dataEntrada.mes, rep.dataEntrada.ano,
           rep.estado == PENDENTE ? "Pendente" : "Concluida");
}

/* f) Mostra todas as reparacoes */
void mostraReparacoes(tipoReparacao vetor[MAX_REPARACOES], int quantRep)
{
    int i;

    if (quantRep == 0)
    {
        printf("\nNao existem reparacoes registadas\n");
    }
    else
    {
        printf("\n Cod.   Matricula Descricao                     Valor  Entrada     Estado");
        for (i = 0; i < quantRep; i++)
        {
            mostraReparacao(vetor[i]);
        }
        printf("\n");
    }
}

/* g) Devolve o valor total das concluidas e, por ponteiro, a quantidade de pendentes */
float valorConcluidas(tipoReparacao vetor[MAX_REPARACOES], int quantRep, int *quantPendentes)
{
    int i;
    float total = 0;

    *quantPendentes = 0;
    for (i = 0; i < quantRep; i++)
    {
        if (vetor[i].estado == CONCLUIDA)
        {
            total += vetor[i].valor;
        }
        else
        {
            (*quantPendentes)++;
        }
    }
    return total;
}

/* h) Percentagem de reparacoes pendentes + listagem */
void percentagemPendentes(tipoReparacao vetor[MAX_REPARACOES], int quantRep)
{
    int i, conta = 0;
    float percentagem;

    if (quantRep == 0)
    {
        printf("\nNao existem reparacoes registadas\n");
    }
    else
    {
        for (i = 0; i < quantRep; i++)
        {
            if (vetor[i].estado == PENDENTE)
            {
                mostraReparacao(vetor[i]);
                conta++;
            }
        }
        percentagem = (float) conta * 100 / quantRep;
        printf("\n\nReparacoes pendentes: %d (%.2f%%)\n", conta, percentagem);
    }
}

/* i) Quantidade de reparacoes de uma matricula indicada + ficheiro de texto */
int reparacoesViatura(tipoReparacao vetor[MAX_REPARACOES], int quantRep)
{
    FILE *ficheiro;
    char matricula[MAX_MATRICULA];
    int i, conta = 0;

    printf("Matricula: ");
    lerString(matricula, MAX_MATRICULA);

    ficheiro = fopen(FICH_TEXTO, "w");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_TEXTO);
    }
    else
    {
        fprintf(ficheiro, "Reparacoes da viatura %s\n", matricula);
    }
    for (i = 0; i < quantRep; i++)
    {
        if (strcmp(vetor[i].matricula, matricula) == 0)
        {
            conta++;
            if (ficheiro != NULL)
            {
                fprintf(ficheiro, "%d\t%s\t%.2f\t%02d/%02d/%04d\t%s\n", vetor[i].codigo, vetor[i].descricao,
                        vetor[i].valor, vetor[i].dataEntrada.dia, vetor[i].dataEntrada.mes,
                        vetor[i].dataEntrada.ano, vetor[i].estado == PENDENTE ? "Pendente" : "Concluida");
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

/* j) Concluir a reparacao com o codigo indicado (so se estiver pendente) */
void concluiReparacao(tipoReparacao vetor[MAX_REPARACOES], int quantRep)
{
    int codigo, pos;

    printf("Codigo da reparacao: ");
    codigo = lerInteiro(1, 99999);
    pos = procuraReparacao(vetor, quantRep, codigo);
    if (pos == -1)
    {
        printf("\nNao existe nenhuma reparacao com o codigo %d\n", codigo);
    }
    else if (vetor[pos].estado == CONCLUIDA)
    {
        printf("\nA reparacao %d ja estava concluida\n", codigo);
    }
    else
    {
        vetor[pos].estado = CONCLUIDA;
        printf("\nReparacao %d concluida\n", codigo);
    }
}

/* k) Listar as reparacoes com entrada num mes/ano indicado */
void listaMesAno(tipoReparacao vetor[MAX_REPARACOES], int quantRep)
{
    int i, mes, ano, conta = 0;

    printf("Mes (1 a 12): ");
    mes = lerInteiro(1, 12);
    printf("Ano (1950 a 2026): ");
    ano = lerInteiro(1950, 2026);

    for (i = 0; i < quantRep; i++)
    {
        if (vetor[i].dataEntrada.mes == mes && vetor[i].dataEntrada.ano == ano)
        {
            mostraReparacao(vetor[i]);
            conta++;
        }
    }
    printf("\n\n%d reparacao(oes) em %02d/%04d\n", conta, mes, ano);
}

/* l) Escrever ficheiro binario */
void gravaFicheiroBinario(tipoReparacao vetor[MAX_REPARACOES], int quantRep)
{
    FILE *ficheiro;

    ficheiro = fopen(FICH_BINARIO, "wb");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_BINARIO);
    }
    else
    {
        fwrite(&quantRep, sizeof(int), 1, ficheiro);
        fwrite(vetor, sizeof(tipoReparacao), quantRep, ficheiro);
        fclose(ficheiro);
        printf("\nDados de %d reparacoes gravados\n", quantRep);
    }
}

/* m) Ler ficheiro binario */
int leFicheiroBinario(tipoReparacao vetor[MAX_REPARACOES])
{
    FILE *ficheiro;
    int quantRep = 0;

    ficheiro = fopen(FICH_BINARIO, "rb");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_BINARIO);
    }
    else
    {
        if (fread(&quantRep, sizeof(int), 1, ficheiro) != 1 || quantRep < 0 || quantRep > MAX_REPARACOES)
        {
            quantRep = 0;
        }
        else if (fread(vetor, sizeof(tipoReparacao), quantRep, ficheiro) != (size_t) quantRep)
        {
            quantRep = 0;
        }
        fclose(ficheiro);
        printf("\nDados de %d reparacoes lidos\n", quantRep);
    }
    return quantRep;
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
