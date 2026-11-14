/*
 * Programacao I - Teste Pratico 1 (Treino)
 * Gestao dos funcionarios de uma empresa
 *
 * Numero: ________     Nome: ______________________________
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_FUNCIONARIOS 50
#define MAX_NOME 50
#define VENCIMENTO_REF 1000.0
#define FICH_BINARIO "funcionarios.dat"
#define FICH_TEXTO "admitidos.txt"

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
    float vencimento;
    tipoData dataAdmissao;
} tipoFuncionario;

/* Prototipos */
char menu(int quantFunc, float mediaVenc, int quantAdmitidos);
tipoFuncionario lerDadosFuncionario(void);
int procuraFuncionario(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc, int numero);
int acrescentaFuncionario(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc);
void mostraFuncionario(tipoFuncionario func);
void mostraFuncionarios(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc);
float mediaVencimentos(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc);
void percentagemVencimentoSuperior(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc);
int admitidosDesdeNov2020(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc);
void alteraVencimento(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc);
void gravaFicheiroBinario(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc);
int leFicheiroBinario(tipoFuncionario vetor[MAX_FUNCIONARIOS]);

/* Funcoes para leitura de dados (disponibilizadas nas aulas) */
void limpaBufferStdin(void);
int lerInteiro(int min, int max);
float lerFloat(float min, float max);
void lerString(char vetor[], int max);
tipoData lerData(void);

/* a) Estrutura do menu na funcao main() */
int main(void)
{
    tipoFuncionario funcionarios[MAX_FUNCIONARIOS];   /* c) estrutura de dados */
    int quantFunc = 0;
    int quantAdmitidos;
    float media;
    char opcao;

    do
    {
        media = mediaVencimentos(funcionarios, quantFunc);
        quantAdmitidos = admitidosDesdeNov2020(funcionarios, quantFunc);
        opcao = menu(quantFunc, media, quantAdmitidos);

        switch (opcao)
        {
        case 'N':
            quantFunc = acrescentaFuncionario(funcionarios, quantFunc);
            break;
        case 'L':
            mostraFuncionarios(funcionarios, quantFunc);
            break;
        case 'P':
            percentagemVencimentoSuperior(funcionarios, quantFunc);
            break;
        case 'A':
            alteraVencimento(funcionarios, quantFunc);
            break;
        case 'E':
            gravaFicheiroBinario(funcionarios, quantFunc);
            break;
        case 'R':
            quantFunc = leFicheiroBinario(funcionarios);
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
char menu(int quantFunc, float mediaVenc, int quantAdmitidos)
{
    char op;

    printf("\n\n************** Gestao de Funcionarios **************\n");
    printf(" Funcionarios: %d\t Media dos vencimentos: %.2f euros\n", quantFunc, mediaVenc);
    printf(" Admitidos desde novembro de 2020: %d\n", quantAdmitidos);
    printf("\n N - Novo funcionario");
    printf("\n L - Listar dados de funcionarios");
    printf("\n P - Percentagem de funcionarios com vencimento superior a 1000 euros");
    printf("\n A - Alterar vencimento de um funcionario");
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

/* Obtem os dados de um funcionario */
tipoFuncionario lerDadosFuncionario(void)
{
    tipoFuncionario func;

    printf("Numero do funcionario (1 a 9999): ");
    func.numero = lerInteiro(1, 9999);

    printf("Nome: ");
    lerString(func.nome, MAX_NOME);

    printf("Vencimento mensal (euros): ");
    func.vencimento = lerFloat(0, 100000);

    printf("Data de admissao:");
    func.dataAdmissao = lerData();

    return func;
}

/* Devolve a posicao do funcionario com esse numero, ou -1 se nao existir */
int procuraFuncionario(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc, int numero)
{
    int i, pos = -1;

    for (i = 0; i < quantFunc; i++)
    {
        if (vetor[i].numero == numero)
        {
            pos = i;
            i = quantFunc;      /* termina a pesquisa */
        }
    }
    return pos;
}

/* d) Acrescenta um funcionario com numero unico; devolve a nova quantidade */
int acrescentaFuncionario(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc)
{
    tipoFuncionario func;

    if (quantFunc == MAX_FUNCIONARIOS)
    {
        printf("\nImpossivel acrescentar: limite de %d funcionarios atingido\n", MAX_FUNCIONARIOS);
    }
    else
    {
        func = lerDadosFuncionario();
        if (procuraFuncionario(vetor, quantFunc, func.numero) != -1)
        {
            printf("\nJa existe um funcionario com o numero %d - nao foi acrescentado\n", func.numero);
        }
        else
        {
            vetor[quantFunc] = func;
            quantFunc++;
        }
    }
    return quantFunc;
}

/* e) Mostra os dados de um funcionario */
void mostraFuncionario(tipoFuncionario func)
{
    printf("\n %4d  %-25s %10.2f   %02d/%02d/%04d", func.numero, func.nome, func.vencimento,
           func.dataAdmissao.dia, func.dataAdmissao.mes, func.dataAdmissao.ano);
}

/* f) Mostra os dados de todos os funcionarios */
void mostraFuncionarios(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc)
{
    int i;

    if (quantFunc == 0)
    {
        printf("\nNao existem funcionarios registados\n");
    }
    else
    {
        printf("\n Num.  Nome                      Vencimento   Admissao");
        for (i = 0; i < quantFunc; i++)
        {
            mostraFuncionario(vetor[i]);
        }
        printf("\n");
    }
}

/* g) Calcula e devolve a media dos vencimentos mensais */
float mediaVencimentos(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc)
{
    int i;
    float soma = 0, media = 0;

    for (i = 0; i < quantFunc; i++)
    {
        soma += vetor[i].vencimento;
    }
    if (quantFunc > 0)
    {
        media = soma / quantFunc;
    }
    return media;
}

/* h) Calcula e apresenta a percentagem de funcionarios com vencimento > 1000 euros
      e lista os dados desses funcionarios */
void percentagemVencimentoSuperior(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc)
{
    int i, conta = 0;
    float percentagem;

    if (quantFunc == 0)
    {
        printf("\nNao existem funcionarios registados\n");
    }
    else
    {
        printf("\n Num.  Nome                      Vencimento   Admissao");
        for (i = 0; i < quantFunc; i++)
        {
            if (vetor[i].vencimento > VENCIMENTO_REF)
            {
                mostraFuncionario(vetor[i]);
                conta++;
            }
        }
        percentagem = (float) conta * 100 / quantFunc;
        printf("\n\nFuncionarios com vencimento superior a 1000 euros: %d (%.2f%%)\n", conta, percentagem);
    }
}

/* i) Calcula e devolve a quantidade de funcionarios admitidos desde novembro de 2020
      (inclusive) e guarda os dados desses funcionarios num ficheiro de texto */
int admitidosDesdeNov2020(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc)
{
    FILE *ficheiro;
    int i, conta = 0;
    tipoData d;

    ficheiro = fopen(FICH_TEXTO, "w");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_TEXTO);
    }
    else
    {
        fprintf(ficheiro, "Funcionarios admitidos desde novembro de 2020\n");
        fprintf(ficheiro, "Numero\tNome\tVencimento\tData de admissao\n");
    }

    for (i = 0; i < quantFunc; i++)
    {
        d = vetor[i].dataAdmissao;
        if (d.ano > 2020 || (d.ano == 2020 && d.mes >= 11))
        {
            conta++;
            if (ficheiro != NULL)
            {
                fprintf(ficheiro, "%d\t%s\t%.2f\t%02d/%02d/%04d\n", vetor[i].numero, vetor[i].nome,
                        vetor[i].vencimento, d.dia, d.mes, d.ano);
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

/* j) Altera o vencimento do funcionario com o numero indicado pelo utilizador */
void alteraVencimento(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc)
{
    int numero, pos;

    if (quantFunc == 0)
    {
        printf("\nNao existem funcionarios registados\n");
    }
    else
    {
        printf("Numero do funcionario: ");
        numero = lerInteiro(1, 9999);
        pos = procuraFuncionario(vetor, quantFunc, numero);
        if (pos == -1)
        {
            printf("\nNao existe nenhum funcionario com o numero %d\n", numero);
        }
        else
        {
            printf("Vencimento atual: %.2f euros\n", vetor[pos].vencimento);
            printf("Novo vencimento (euros): ");
            vetor[pos].vencimento = lerFloat(0, 100000);
        }
    }
}

/* k) Escreve num ficheiro binario os dados de todos os funcionarios */
void gravaFicheiroBinario(tipoFuncionario vetor[MAX_FUNCIONARIOS], int quantFunc)
{
    FILE *ficheiro;
    int quant;

    ficheiro = fopen(FICH_BINARIO, "wb");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_BINARIO);
    }
    else
    {
        quant = fwrite(&quantFunc, sizeof(int), 1, ficheiro);
        if (quant != 1)
        {
            printf("\nErro ao escrever no ficheiro\n");
        }
        else
        {
            quant = fwrite(vetor, sizeof(tipoFuncionario), quantFunc, ficheiro);
            if (quant != quantFunc)
            {
                printf("\nErro ao escrever no ficheiro (%d de %d)\n", quant, quantFunc);
            }
            else
            {
                printf("\nDados de %d funcionarios gravados em %s\n", quantFunc, FICH_BINARIO);
            }
        }
        fclose(ficheiro);
    }
}

/* l) Obtem do ficheiro binario os dados dos funcionarios; devolve a quantidade lida */
int leFicheiroBinario(tipoFuncionario vetor[MAX_FUNCIONARIOS])
{
    FILE *ficheiro;
    int quantFunc = 0, quant;

    ficheiro = fopen(FICH_BINARIO, "rb");
    if (ficheiro == NULL)
    {
        printf("\nErro ao abrir o ficheiro %s\n", FICH_BINARIO);
    }
    else
    {
        quant = fread(&quantFunc, sizeof(int), 1, ficheiro);
        if (quant != 1 || quantFunc < 0 || quantFunc > MAX_FUNCIONARIOS)
        {
            printf("\nErro ao ler do ficheiro\n");
            quantFunc = 0;
        }
        else
        {
            quant = fread(vetor, sizeof(tipoFuncionario), quantFunc, ficheiro);
            if (quant != quantFunc)
            {
                printf("\nErro ao ler do ficheiro (%d de %d)\n", quant, quantFunc);
                quantFunc = 0;
            }
            else
            {
                printf("\nDados de %d funcionarios lidos de %s\n", quantFunc, FICH_BINARIO);
            }
        }
        fclose(ficheiro);
    }
    return quantFunc;
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
