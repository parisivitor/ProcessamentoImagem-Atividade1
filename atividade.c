/*+-------------------------------------------------------------------------+
  | PROCESSAMENTO DE IMAGEM -   Prof Luiz Eduardo da Silva.                 |
  | -----------------------                                                 |
  |                                                                         |
  | Vitor Risso Parisi                                                      |
  +-------------------------------------------------------------------------+*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define TRUE  1
#define FALSE 0
#define CREATOR "# CREATOR: Vitor Parisi V:1.00 RA:2016.1.08.037\n"

typedef int * imagem;

/*+-------------------------------------------------------------------------+
  | Rotinas para ALOCAR e DESALOCAR dinamicamente espaco de memoria para um |
  | vetor monodimensional que armazenara' a imagem.                         |
  | PARAMETROS:                                                             |
  | I  = Endereco do vetor (ponteiro de inteiros).                          |
  | nl = Numero de linhas.                                                  |
  | nc = Numero de colunas.                                                 |
  +-------------------------------------------------------------------------+*/
int aloca_memo(imagem *I, int nl, int nc) {
    *I = (int *) malloc(nl * nc * sizeof (int));
    if (*I) return TRUE;
    else return FALSE;
}

int desaloca_memo(imagem *I) {
    free(*I);
}

/*+-------------------------------------------------------------------------+
  | Apresenta informacoes sobre um arquivo de imagem.                       |
  | Parametros:                                                             |
  |   nome = nome fisico do arquivo de imagem.                              |
  |   nl   = numero de linhas da imagem.                                    |
  |   nc   = numero de colunas da imagem.                                   |
  |   mn   = maximo nivel de cinza da imagem.                               |
  +-------------------------------------------------------------------------+*/
void info_imagem(char *nome, int nl, int nc, int mn) {
    printf("\nINFORMACOES SOBRE A IMAGEM:");
    printf("\n--------------------------\n");
    printf("Nome do arquivo.............: %s \n", nome);
    printf("Numero de linhas............: %d \n", nl);
    printf("Numero de colunas...........: %d \n", nc);
    printf("Maximo nivel-de-cinza/cores.: %d \n\n", mn);
}

/*+-------------------------------------------------------------------------+
  | Rotina que faz a leitura de uma imagem em formato .PGM ASCII e armazena |
  | num vetor monodimensional. Um exemplo de imagem .PGM ASCII gravada neste|
  | formato:                                                                |
  |                                                                         |
  | P2                                                                      |
  | # CREATOR: XV Version 3.10a  Rev: 12/29/94                              |
  | 124 122                                                                 |
  | 255                                                                     |
  | 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255     |
  | 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255     |
  | (...)                                                                   |
  |                                                                         |
  | Lin 1: contem P2, o que identifica este arquivo como PGM ASCII.         |
  | Lin 2: contem um comentario qualquer iniciado com #.                    |
  | Lin 3: numero de colunas e numero de linhas da imagem.                  |
  | Lin 4: maximo nivel de cinza na imagem (255 normalmente).               |
  | Lin 5 em diante: valores de cinza da imagem.                            |
  |                                                                         |
  | PARAMETROS:                                                             |
  | nome = nome do arquivo (entra).                                         |
  | I    = ponteiro para o vetor imagem (retorna).                          |
  | nl   = numero de linhas da imagem (retorna).                            |
  | nc   = numero de colunas da imagem (retorna).                           |
  | mn   = maximo nivel de cinza (retorna).                                 |
  +-------------------------------------------------------------------------+*/
int le_imagem_pgm(char *nome, imagem *I, int *nl, int *nc, int *mn) {
    int i, j, k, LIMX, LIMY, MAX_NIVEL;
    char c, LINHA[100];
    FILE *arq;
    if ((arq = fopen(nome, "r")) == NULL) {
        printf("Erro na ABERTURA do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- PGM = "P2" -----------*/
    fgets(LINHA, 80, arq);
    if (!strstr(LINHA, "P2")) {
        printf("Erro no FORMATO do arquivo <%s>\n\n", nome);
        return FALSE;
    }
    /*-- Dimensoes da imagem --*/
    fgets(LINHA, 80, arq);
    do {
        fgets(LINHA, 80, arq);
    } while (strchr(LINHA, '#'));
    sscanf(LINHA, "%d %d", &LIMX, &LIMY);
    fscanf(arq, "%d", &MAX_NIVEL);

    if (LIMX == 0 || LIMY == 0 || MAX_NIVEL == 0) {
        printf("Erro nas DIMENSOES do arquivo <%s>\n\n", nome);
        return FALSE;
    }

    if (aloca_memo(I, LIMY, LIMX)) {
        for (i = 0; i < LIMY; i++) {
            for (j = 0; j < LIMX; j++) {
                fscanf(arq, "%d", &k);
                if (k > MAX_NIVEL) {
                    printf("Erro nos DADOS do arquivo <%s>\n", nome);
                    printf("Valor lido: %d   Max Nivel: %d\n\n", k, MAX_NIVEL);
                    return FALSE;
                }
                *(*(I) + i * LIMX + j) = k;
            }
        }
        fclose(arq);
    } else {
        printf("Erro na ALOCACAO DE MEMORIA para o arquivo <%s>\n\n", nome);
        printf("Rotina: le_imagem_pgm\n\n");
        fclose(arq);
        return FALSE;
    }
    *nc = LIMX;
    *nl = LIMY;
    *mn = MAX_NIVEL;
    return TRUE;
}

/*+-------------------------------------------------------------------------+
  | Rotina que grava o arquivo da imagem em formato PGM ASCII.              |
  | PARAMETROS:                                                             |
  | I    = ponteiro para o vetor imagem (entra).                            |
  | nome = nome do arquivo (entra).                                         |
  | nl   = numero de linhas (entra).                                        |
  | nc   = numero de colunas (entra).                                       |
  | mn   = maximo nivel de cinza (entra).                                   |
  +-------------------------------------------------------------------------+*/
void grava_imagem_pgm(imagem I, char *nome, int nl, int nc, int mn) {
    int i, j, x, k, valores_por_linha;
    FILE *arq;
    if ((arq = fopen(nome, "wt")) == NULL) {
        printf("Erro na CRIACAO do arquivo <%s>\n\n", nome);
    } else {
        fputs("P2\n", arq);
        fputs(CREATOR, arq);
        fprintf(arq, "%d  %d\n", nc, nl);
        fprintf(arq, "%d\n", mn);
        valores_por_linha = 16;
        for (i = 0, k = 0; i < nl; i++)
            for (j = 0; j < nc; j++) {
                x = *(I + i * nc + j);
                fprintf(arq, "%3d ", x);
                k++;
                if (k > valores_por_linha) {
                    fprintf(arq, "\n");
                    k = 0;
                }
            }
    }
    fclose(arq);
}

int tetoMn(int valor, int mn) {
    if (valor >= mn) {
        //valor = mn;
        return valor;
    } else {
        return valor;
    }
}

void prolongar(imagem In, imagem Out, int nl, int nc, int mn, int x1, int y1, int x2, int y2) {
    int i, valor, a, b;
    for (i = 0; i < nl * nc; i++) {
        if (In[i] <= x1) {
            valor = floor(In[i] * y1 / x1);
            Out[i] = tetoMn(valor, mn);
        } else if ((In[i] > x1) && (In[i] < x2)) {
            a = (y2 - y1) / (x2 - x1);
            b = y2 - (a * x2);
            valor = floor(a * In[i] + b);
            Out[i] = tetoMn(valor, mn);
        } else {
            a = (mn - y2) / (mn - x2);
            b = y2 - (a * mn);
            valor = floor(a * In[i] + b);
            Out[i] = tetoMn(valor, mn);
        }
        /*} else if ((In[i] > x1) && (In[i] < x2)) {
            valor = floor(In[i] * (y2 - y1) / (x2 - x1)) + y1;
            Out[i] = tetoMn(valor,mn);
        } else {
            valor = floor(In[i] * (mn - y2) / (mn - x1)) + y2;
            Out[i] =  tetoMn(valor,mn);
        }*/
    }
}

void limiar(imagem Out, int nl, int nc, int mn, int k) {
    int i;
    for (i = 0; i < nl * nc; i++) {
        Out[i] = Out[i] < k ? 0 : 255;

    }
}

void negativo(imagem In, imagem Out, int nl, int nc, int mn) {
    int i, j;
    for (i = 0; i < nl; i++)
        for (j = 0; j < nc; j++) {

            Out[i * nc + j] = 255 - In[i * nc + j];
        }
}

void convoluciona(imagem In, imagem Out, int nl, int nc, int mn){
    int i, j, y, x;
    int w[3][3]={0, 1, 0,
                 1,-4, 1,
                 0, 1, 0};
    for(i=1; i < nl-1 ;i++){
        for(j=1;j<nc - 1; j++){
            int soma=0;
            for(y = 0; y < 3; y++){
                for(x = 0; x<3; x++){
                    int vy = i+y -1;
                    int vx = j+x -1;
                    soma += In[vy * nc + vx] + w[y][x];
                }
            }
            soma = soma < 0 ? 0 : soma;
            soma = soma < mn ? mn : soma;
            Out[i * nc + j] = mn - soma; //destaca as bordas
            //Out[i * nc + j] = soma /9; // borra a imagem
        }
    }
}

void msg(char *s) {
    printf("\nNEGATIVO de uma imagem qualquer");
    printf("\n-------------------------------");
    printf("\nUSO.:  %s  <IMG.PGM> <x1> <y1> <x2> <y2> <k>", s);
    printf("\nONDE:\n");
    printf("    <IMG.PGM>  Arquivo da imagem em formato PGM\n\n");
}

/*+------------------------------------------------------+
  |        P R O G R A M A    P R I N C I P A L          |
  +------------------------------------------------------+*/
int main(int argc, char *argv[]) {
    int OK, nc, nl, mn, x1, y1, x2, y2, k;
    char nome[100];
    char comando[100],img;
    imagem In, Out;
    /*argv[1] = "feijao.pgm";
    argv[2] = "96";
    argv[3] = "32";
    argv[4] = "160";
    argv[5] = "224";
    argv[6] = "96";*/
            
    if (argc < 2)
        msg(argv[0]);
    else {
        OK = le_imagem_pgm(argv[1], &In, &nl, &nc, &mn);
        if (OK) {
             x1 = atoi(argv[2]);
             y1 = atoi(argv[3]);
             x2 = atoi(argv[4]);
             y2 = atoi(argv[5]);
             k = atoi(argv[6]);

            printf("\nNEGATIVO DE IMAGEM");
            info_imagem(argv[1], nl, nc, mn);
            aloca_memo(&Out, nl, nc);

            prolongar(In, Out, nl, nc, mn, x1, y1, x2, y2);
            strcpy(nome, argv[1]);
            strcat(nome, "-alongar.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);

            limiar(Out, nl, nc, mn, k);
            strcpy(nome, argv[1]);
            strcat(nome, "-limiar.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);
            
           
            //convoluciona(In, Out, nl, nc, mn);
            strcpy(nome, argv[1]);
            strcat(nome, "-convoluciona.pgm");
            grava_imagem_pgm(Out, nome, nl, nc, mn);

            //strcpy(comando, "i_view32.exe "); // executar no WINDOWS
            strcpy(comando, "./i_view32 "); // executar no LINUX

            strcat(comando, nome);
            strcat(comando, " /hide=7");
            system(comando);
            desaloca_memo(&In);
            desaloca_memo(&Out);
        }
    }
    return 0;
}
