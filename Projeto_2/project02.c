#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void aleatorizar_nome(int *vet);
/*
Aleatoriza para ler os arquivos em ordem aleatória.
*/

void monta_vetor_binario(int **mat, int lin, int col, char *vetorbin);
/*
A função pega como parâmento a linha e a coluna do elemento central, a partir dele
forma uma submatriz [3][3] e pega os nove elementos, soma todos e calcula a média,
após isso cria-se uma matriz de char [3][3] em que os elementos serão 1 ou 0
iguala-se a '1' os elementos da submatriz que são maiores ou iguais que a média
e iguala-se a '0' os elementos menores que a média, após isso na ordem definida
pelo ILBP monta-se o vetor de binários.
*/
int converte_binario_calcula_menor_decimal(char *bin, int *dec);
/*
A função pega o vetor de binários, rotaciona bit-a-bit e calcula o decimal de
cada rotação, com isso pega-se os decimais, salva em um vetor e pega o menor dos
decimais e retorna.
*/
void GLCM(int **mat, int lin, int col, float *metricas);
/*
A função recebe a matriz do arquivo, o tanto de linhas e de colunas que ela possui,
ela calcula a ocorrência de vizinhos nas 8 direções e calcula o contraste, a energia e
a homogeneidade de cada direção e vai salvando na respectiva ordem em um vetor
metricas que é modificado e na main irá se juntar com o vetor ilbp para formar o vetor de 536 posições.
*/
float contraste(int **mat, int lin_col);
/*
Calcula o contraste de cada direção da GLCM.
*/

float energia(int **mat, int lin_col);
/*
Calcula o energia de cada direção da GLCM.
*/

float homogeneidade(int **mat, int lin_col);
/*
Calcula o homogeneidade de cada direção da GLCM.
*/

void normalizar_vetor_dados(float *ilbp_glcm, float *ilbp_glcm_normalizado);
/*
A função pega o vetor de dados ilbp + glcm e normaliza (faz o cálculo de normalização).
*/
float distancia_euclidiana(float *referencia, float **comparador, int posicao);
/*
A função faz a distância euclidiana entre o vetor de referencia e o vetor que está sendo comparado.
Para poder ver se está mais próximo de grama ou de asfalto.
*/

int main (int argc, char *argv[]) {

  // Declaração de variáveis.
  char marcador, pv, vetorbin[9], name_grass[30], name_asphalt[30];
  int asfalto[50], grama[50];
  int **mat, **mat2, *ilbp, decimal[9], i, j, z, cont, menor, lin, col;
  float *metricas, *ilbp_glcm, *ilbp_glcm_normalizado, distancia_grama, distancia_asfalto;
  float **resultado_grama, **resultado_asfalto, **resultado_asfalto_teste, **resultado_grama_teste;
  float **resultado_grama_treinamento, *media_grama, **resultado_asfalto_treinamento, *media_asfalto;
  float acerto = 0.0, falsa_aceitacao = 0.0, falsa_rejeicao = 0.0;

  // Aloca dinamicamente a matriz dos resultado_grama_treinamento.
  resultado_grama = (float**)malloc(50*sizeof(float *));
  for (i = 0; i < 50; i++) {
    *(resultado_grama+i) = (float*)malloc(536*sizeof(float));
  }

  resultado_asfalto = (float**)malloc(50*sizeof(float *));
  for (i = 0; i < 50; i++) {
    *(resultado_asfalto+i) = (float*)malloc(536*sizeof(float));
  }

  resultado_grama_treinamento = (float**)malloc(25*sizeof(float *));
  resultado_grama_teste = (float**)malloc(25*sizeof(float *));
  for (i = 0; i < 25; i++) {
    *(resultado_grama_treinamento+i) = (float*)malloc(536*sizeof(float));
    *(resultado_grama_teste+i) = (float*)malloc(536*sizeof(float));
  }

  resultado_asfalto_treinamento = (float**)malloc(25*sizeof(float *));
  resultado_asfalto_teste = (float**)malloc(25*sizeof(float *));
  for (i = 0; i < 25; i++) {
    *(resultado_asfalto_treinamento+i) = (float*)malloc(536*sizeof(float));
    *(resultado_asfalto_teste+i) = (float*)malloc(536*sizeof(float));
  }

  media_grama = (float *) calloc(536, sizeof (float));

  media_asfalto = (float *) calloc(536, sizeof (float));

  aleatorizar_nome(grama);
  printf("\nOrdem Grama: \n");
  for (i = 1; i < 51; i++) {
    printf("%d ", grama[i]);
  }
  printf("\n\n");

  // cont pega as posições do vetor de resultados da grama.
  cont = 0;

  for (z = 1; z < 51; z++) {
    printf("Arquivo: %d\n", z);
    // Cria um ponteiro para arquivo.
    FILE *file_grass;

    if (grama[z] < 10) {
      sprintf(name_grass, "DataSet/grass/grass_0%d.txt", grama[z]);
    } else {
      sprintf(name_grass, "DataSet/grass/grass_%d.txt", grama[z]);
    }
    printf("Arquivo: %s\n", name_grass);

    // Abre o arquivo para leitura.
    file_grass = fopen(name_grass, "r");

    // Verifica a existência do arquivo, se não existir retorna "Falha".
    if (file_grass == NULL) {
      printf("Falha\n");
      exit(1);
    }

    lin = 0, col = 1;

    // Verifica quantas linha e colunas tem a matriz do arquivo.
     while ((marcador = fgetc(file_grass)) != EOF) {
      if (marcador == '\n') {
        lin++;
      }
      else if (lin == 1 && marcador == ';') {
        col++;
      }
    }

    // Volta para o ínicio do arquivo.
    rewind(file_grass);

    // Aloca dinamicamente a matriz do arquivo.
    mat = (int**)malloc(lin*sizeof(int *));
    for (i = 0; i < lin; i++) {
      *(mat+i) = (int*)malloc(col*sizeof(int));
    }

    // Aloca dinamicamente o vetor ILBP de 512 elementos tudo preenchido com 0.
    ilbp = (int *) calloc(512, sizeof (int));

    // Aloca dinamicamente o vetor metricas de 24 elementos tudo preenchido com 0.
    metricas = (float *) calloc(24, sizeof (float));

    // Aloca dinamicamente o vetor ilbp+glcm de 536 elementos tudo preenchido com 0.
    ilbp_glcm = (float *) calloc(536, sizeof (float));

    // Aloca dinamicamente o vetor ilbp+glcm normalizado de 536 elementos tudo preenchido com 0.
    ilbp_glcm_normalizado = (float *) calloc(536, sizeof (float));

    // Passa os elementos da matriz do arquivo para a matriz alocada dinamicamente.
    for (i = 0; i < lin; i++) {
      for (j = 0; j < col; j++) {
          if (!feof(file_grass)) {
            fscanf(file_grass, "%d%c", *(mat+i)+j, &pv);
          }
      }
    }

    // Faz o vetor ILBP.
    for (i = 1; i < lin - 1; i++) {
      for (j = 1; j < col - 1; j++) {
        monta_vetor_binario(mat, i, j, vetorbin);
        menor = converte_binario_calcula_menor_decimal(vetorbin, decimal);
        ilbp[menor]++;
      }
    }

    // GLCM.
    GLCM(mat, lin, col, metricas);

    // Concatenar ilbp + glcm,
    for (i = 0; i < 512; i++) {
      *(ilbp_glcm + i) = *(ilbp + i);
    }
    for (i = 512; i < 536; i++) {
      *(ilbp_glcm + i) = *(metricas + (i - 512));
    }

    // Libera a memória utilizada pelo vetor ilbp e metricas.
    free(ilbp);
    free(metricas);

    // NORMALIZAR VETOR CONCATENADO
    normalizar_vetor_dados(ilbp_glcm, ilbp_glcm_normalizado);

    // Libera a memória utilizada pelo vetor ilbp_glcm.
    free(ilbp_glcm);

    // Pega o vetor normalizado da matriz do arquivo.
    for (i = 0; i < 536; i++) {
      *(*(resultado_grama+cont)+i) = *(ilbp_glcm_normalizado + i);
    }
    cont++;

    // Libera a memória alocada dinamicamente.
    for (i = 0; i < lin; i++) {
      free(*(mat+i));
    }
    free(mat);

    free(ilbp_glcm_normalizado);

    // Fecha o arquivo.
    fclose(file_grass);

  }

  aleatorizar_nome(asfalto);
  printf("\nOrdem Asfalto: \n");
  for (i = 1; i < 51; i++) {
    printf("%d ", asfalto[i]);
  }
  printf("\n\n");

  // cont pega as posições do vetor de resultados do asfalto.
  cont = 0;

  for (z = 1; z < 51; z++) {
    printf("Arquivo: %d\n", z);
    // Cria um ponteiro para arquivo.
    FILE *file_asphalt;

    if (asfalto[z] < 10) {
      sprintf(name_asphalt, "DataSet/asphalt/asphalt_0%d.txt", asfalto[z]);
    } else {
      sprintf(name_asphalt, "DataSet/asphalt/asphalt_%d.txt", asfalto[z]);
    }
    printf("Arquivo: %s\n", name_asphalt);
    // Abre o arquivo para leitura.
    file_asphalt = fopen(name_asphalt, "r");

    // Verifica a existência do arquivo, se não existir retorna "Falha".
    if (file_asphalt == NULL) {
      printf("Falha\n");
      exit(1);
    }

    lin = 0, col = 1;

    // Verifica quantas linha e colunas tem a matriz do arquivo.
     while ((marcador = fgetc(file_asphalt)) != EOF) {
      if (marcador == '\n') {
        lin++;
      }
      else if (lin == 1 && marcador == ';') {
        col++;
      }
    }

    // Volta para o ínicio do arquivo.
    rewind(file_asphalt);

    // Aloca dinamicamente a matriz do arquivo.
    mat = (int**)malloc(lin*sizeof(int *));
    for (i = 0; i < lin; i++) {
      *(mat+i) = (int*)malloc(col*sizeof(int));
    }

    // Aloca dinamicamente o vetor ILBP de 512 elementos tudo preenchido com 0.
    ilbp = (int *) calloc(512, sizeof (int));

    // Aloca dinamicamente o vetor metricas de 24 elementos tudo preenchido com 0.
    metricas = (float *) calloc(24, sizeof (float));

    // Aloca dinamicamente o vetor ilbp+glcm de 536 elementos tudo preenchido com 0.
    ilbp_glcm = (float *) calloc(536, sizeof (float));

    // Aloca dinamicamente o vetor ilbp+glcm normalizado de 536 elementos tudo preenchido com 0.
    ilbp_glcm_normalizado = (float *) calloc(536, sizeof (float));

    // Passa os elementos da matriz do arquivo para a matriz alocada dinamicamente.
    for (i = 0; i < lin; i++) {
      for (j = 0; j < col; j++) {
          if (!feof(file_asphalt)) {
            fscanf(file_asphalt, "%d%c", *(mat+i)+j, &pv);
          }
      }
    }

    // Faz o vetor ILBP.
    for (i = 1; i < lin - 1; i++) {
      for (j = 1; j < col - 1; j++) {
        monta_vetor_binario(mat, i, j, vetorbin);
        menor = converte_binario_calcula_menor_decimal(vetorbin, decimal);
        ilbp[menor]++;
      }
    }

    // GLCM.
    GLCM(mat, lin, col, metricas);

    // Concatenar ilbp + glcm,
    for (i = 0; i < 512; i++) {
      *(ilbp_glcm + i) = *(ilbp + i);
    }
    for (i = 512; i < 536; i++) {
      *(ilbp_glcm + i) = *(metricas + (i - 512));
    }

    // Libera a memória utilizada pelo vetor ilbp e metricas.
    free(ilbp);
    free(metricas);

    // NORMALIZAR VETOR CONCATENADO
    normalizar_vetor_dados(ilbp_glcm, ilbp_glcm_normalizado);

    // Libera a memória utilizada pelo vetor ilbp_glcm.
    free(ilbp_glcm);

    // Pega o vetor normalizado da matriz do arquivo.
    for (i = 0; i < 536; i++) {
      *(*(resultado_asfalto+cont)+i) = *(ilbp_glcm_normalizado + i);
    }
    cont++;

    // Libera a memória alocada dinamicamente.
    for (i = 0; i < lin; i++) {
      free(*(mat+i));
    }
    free(mat);

    free(ilbp_glcm_normalizado);

    // Fecha o arquivo.
    fclose(file_asphalt);

  }

  // Faz o treinamento para pegar o vetor base (media) de grama e de asfalto.
  for (i = 0; i < 25; i++) {
    for (j = 0; j < 536; j++) {
      *(*(resultado_grama_treinamento+i)+j) = *(*(resultado_grama+i)+j);
    }
  }
  for (i = 0; i < 25; i++) {
    for (j = 0; j < 536; j++) {
      *(*(resultado_asfalto_treinamento+i)+j) = *(*(resultado_asfalto+i)+j);
    }
  }
  // MEDIA DO VETOR RESULTADOS
  for (j = 0; j < 536; j++) {
    for (i = 0; i < 25; i++) {
      media_grama[j] += resultado_grama_treinamento[i][j];
    }
    media_grama[j] = media_grama[j] / 25.0;
  }

  for (j = 0; j < 536; j++) {
    for (i = 0; i < 25; i++) {
      media_asfalto[j] += resultado_asfalto_treinamento[i][j];
    }
    media_asfalto[j] = media_asfalto[j] / 25.0;
  }

  // Monta os testes.
  for (i = 0; i < 25; i++) {
    for (j = 0; j < 536; j++) {
      *(*(resultado_grama_teste+i)+j) = *(*(resultado_grama+i+25)+j);
    }
  }

  for (i = 0; i < 25; i++) {
    for (j = 0; j < 536; j++) {
      *(*(resultado_asfalto_teste+i)+j) = *(*(resultado_asfalto+i+25)+j);
    }
  }
  // Pega as taxas para grama.
  for (i = 0; i < 25; i++) {
    distancia_grama = distancia_euclidiana(media_grama, resultado_grama_teste, i);
    distancia_asfalto = distancia_euclidiana(media_asfalto, resultado_grama_teste, i);

    if (distancia_grama > distancia_asfalto) {
     falsa_rejeicao++;
    }
    else if (distancia_grama <= distancia_asfalto) {
     acerto++;
   }
  }

  // Pega as taxas para asfalto.
  for (i = 0; i < 25; i++) {
    distancia_grama = distancia_euclidiana(media_grama, resultado_asfalto_teste, i);
    distancia_asfalto = distancia_euclidiana(media_asfalto, resultado_asfalto_teste, i);

    if(distancia_grama < distancia_asfalto) {
      falsa_aceitacao++;
    }
    else if(distancia_grama >= distancia_asfalto) {
      acerto++;
    }
  }

  printf("\n\nResultados:\n\n");

   acerto = (acerto / 50) * 100;
   falsa_aceitacao = (falsa_aceitacao / 50) * 100;
   falsa_rejeicao = (falsa_rejeicao / 50) * 100;

   printf("Taxa de Acerto: %.2f%%\n", acerto);
   printf("Taxa de Falsa Aceitação: %.2f%%\n", falsa_aceitacao);
   printf("Taxa de Falsa Rejeição: %.2f%%\n", falsa_rejeicao);

  // Libera a memória alocada dinamicamente.
  for (i = 0; i < 50; i++) {
    free(*(resultado_grama+i));
    free(*(resultado_asfalto+i));
  }
  free(resultado_grama);
  free(resultado_asfalto);
  for (i = 0; i < 25; i++) {
    free(*(resultado_grama_treinamento+i));
    free(*(resultado_asfalto_treinamento+i));
    free(*(resultado_grama_teste+i));
    free(*(resultado_asfalto_teste+i));
  }
  free(resultado_grama_treinamento);
  free(resultado_grama_teste);
  free(resultado_asfalto_teste);
  free(resultado_asfalto_treinamento);
  free(media_asfalto);
  free(media_grama);
  printf("\n\n");
  return 0;
}

void aleatorizar_nome(int *vet){

  int i;

  // Para o vetor ser diferente a cada vez que rode o programa.
  srand((unsigned)time(0)*100);

  for (i = 1; i < 51; i++) {
      vet[i] = i;
  }

  for (i = 1; i < 51; i++) {
    int temp = vet[i];
    int random = rand() % 50;
      if (random != 0) {
        vet[i] = vet[random];
        vet[random] = temp;
      }
    }
}

void monta_vetor_binario(int **mat, int lin, int col, char *vetorbin) {
  // Declaração de variáveis locais.
  float soma = 0, media;
  int i, j, x = 0, y = 0;
  char **bin;

  // Aloca dinamicamente a submatriz [3][3] de binários.
  bin = (char**)malloc(3*sizeof(char *));
  for (i = 0; i < 3; i++) {
    *(bin + i) = (char*)malloc(3*sizeof(char));
  }

  // Calcula a soma de todos os elementos da submatriz[3][3].
  for (i = lin - 1; i <= lin + 1; i++) {
    for (j = col - 1; j <= col + 1; j++) {
      soma += *(*(mat+i)+j);
    }
  }

  // Calcula a média da submatriz [3][3].
  media = soma / 9.0;

  // Verifica cada elemento da submatriz se é maior ou igual a média e preenche com 1 onde é maior e com 0 onde é menor em uma matriz.
  for (i = lin - 1; i <= lin + 1; i++) {
    for (j = col - 1; j <= col + 1; j++) {
      if (*(*(mat+i)+j) < media) {
        *(*(bin+x)+y) = '0';
        y++;
      } else if (*(*(mat+i)+j) >= media) {
        *(*(bin+x)+y) = '1';
        y++;
      }
    }
    y = 0;
    x++;
  }

  // Passa os elementos da matriz criada para o vetorbin na ordem solicitada pelo ILBP.
  vetorbin[0] = *(*(bin+0)+0);
  vetorbin[1] = *(*(bin+0)+1);
  vetorbin[2] = *(*(bin+0)+2);
  vetorbin[3] = *(*(bin+1)+2);
  vetorbin[4] = *(*(bin+2)+2);
  vetorbin[5] = *(*(bin+2)+1);
  vetorbin[6] = *(*(bin+2)+0);
  vetorbin[7] = *(*(bin+1)+0);
  vetorbin[8] = *(*(bin+1)+1);

  // Libera a memória alocada dinamicamente para a matriz de binários.
  for (i = 0; i < 3; i++) {
    free(*(bin+i));
  }
  free(bin);
}

int converte_binario_calcula_menor_decimal(char *bin, int *dec) {

  // Declaração de variáveis locais.
  int decimal, i, k, c, j, n = 9, m = 0, menor = 512;
  char temp;

  for (k = 9; k > 0; k--) {

    // Calcula o decimal.
    decimal = 0;
    j = 0;
    for (i = 8; i >= 0; i--) {
      if (*(bin + i) == '1') {
        decimal += pow(2, j);
      }
      j++;
    }

    // Rotaciona o vetor em 1 bit.
    temp = bin[n - 1];
    for (c = n-1; c > 0; c--) {
      bin[c] = bin[c - 1];
    }
    bin[0] = temp;

    // Salva decimal em um vetor.
    dec[m] = decimal;
    m++;
  }

  // Confere qual é o menor decimal dentre os convertidos.
  for (int i = 0; i < 9; i++) {
    if (menor > dec[i]) {
      menor = dec[i];
    }
  }

  // Retorna o menor elemento.
  return menor;
}

float contraste(int **mat, int lin_col) {

  int i, j;
  float contraste = 0;

  for (i = 0; i < lin_col; i++) {
    for (j = 0; j < lin_col; j++) {
      contraste += pow(i-j,2) * mat[i][j];
    }
  }

  return contraste;
}

float energia(int **mat, int lin_col) {

  int i, j;
  float energia = 0;

  for (i = 0; i < lin_col; i++) {
    for (j = 0; j < lin_col; j++) {
      energia += pow(mat[i][j],2);
    }
  }

  return energia;
}

float homogeneidade(int **mat, int lin_col) {

  int i, j, z;
  float homogeneidade = 0;

  for (i = 0; i < lin_col; i++) {
    for (j = 0; j < lin_col; j++) {
      z = i - j;
      if (z < 0) {
        z = z * (-1);
      }
      homogeneidade += mat[i][j] / (1 + z);
    }
  }

  return homogeneidade;
}


void GLCM(int **mat, int lin, int col, float *metricas) {

  // Declaração de variáveis locais.
  int **glcm, i, j, m, glcm_lin_col = 512;

  for (m = 0; m < 8; m++) {

    // Aloca uma matriz de zeros que irá pegar o GLCM de cada direção.
    glcm = (int**)calloc(glcm_lin_col, sizeof(int *));
    for (i = 0; i < glcm_lin_col; i++) {
      *(glcm+i) = (int*)calloc(glcm_lin_col, sizeof(int));
    }
    if (m == 0) {
      // Incrementa a posição [elemento][elemento vizinho da direita] da matriz glcm.
      for (i = 0; i < lin; i++) {
        for (j = 0; j < col - 1; j++) {
          glcm[mat[i][j]][mat[i][j+1]]++;
        }
      }
      *(metricas + 0) = contraste(glcm, glcm_lin_col);
      *(metricas + 1) = energia(glcm, glcm_lin_col);
      *(metricas + 2) = homogeneidade(glcm, glcm_lin_col);
    } else if (m == 1) {

      // Incrementa a posição [elemento][elemento vizinho da esquerda] da matriz glcm.
      for (i = 0; i < lin; i++) {
        for (j = 1; j < col; j++) {
          glcm[mat[i][j]][mat[i][j-1]]++;
        }
      }
      *(metricas + 3) = contraste(glcm, glcm_lin_col);;
      *(metricas + 4) = energia(glcm, glcm_lin_col);
      *(metricas + 5) = homogeneidade(glcm, glcm_lin_col);
    } else if (m == 2) {

      // Incrementa a posição [elemento][elemento vizinho de cima] da matriz glcm.
      for (i = 1; i < lin; i++) {
        for (j = 0; j < col; j++) {
          glcm[mat[i][j]][mat[i-1][j]]++;
        }
      }
      *(metricas + 6) = contraste(glcm, glcm_lin_col);;
      *(metricas + 7) = energia(glcm, glcm_lin_col);
      *(metricas + 8) = homogeneidade(glcm, glcm_lin_col);
    } else if (m == 3) {

      // Incrementa a posição [elemento][elemento vizinho de baixo] da matriz glcm.
      for (i = 0; i < lin - 1; i++) {
        for (j = 0; j < col; j++) {
          glcm[mat[i][j]][mat[i+1][j]]++;
        }
      }
      *(metricas + 9) = contraste(glcm, glcm_lin_col);;
      *(metricas + 10) = energia(glcm, glcm_lin_col);
      *(metricas + 11) = homogeneidade(glcm, glcm_lin_col);
    } else if (m == 4) {

      // Incrementa a posição [elemento][elemento vizinho da diagonal de cima da esquerda] da matriz glcm.
      for (i = 1; i < lin; i++) {
        for (j = 1; j < col; j++) {
          glcm[mat[i][j]][mat[i-1][j-1]]++;
        }
      }
      *(metricas + 12) = contraste(glcm, glcm_lin_col);;
      *(metricas + 13) = energia(glcm, glcm_lin_col);
      *(metricas + 14) = homogeneidade(glcm, glcm_lin_col);
    } else if (m == 5) {

      // Incrementa a posição [elemento][elemento vizinho da diagonal de cima da direita] da matriz glcm.
      for (i = 1; i < lin; i++) {
        for (j = 0; j < col - 1; j++) {
          glcm[mat[i][j]][mat[i-1][j+1]]++;
        }
      }
      *(metricas + 15) = contraste(glcm, glcm_lin_col);;
      *(metricas + 16) = energia(glcm, glcm_lin_col);
      *(metricas + 17) = homogeneidade(glcm, glcm_lin_col);
    } else if (m == 6) {

      // Incrementa a posição [elemento][elemento vizinho da diagonal de baixo da esquerda] da matriz glcm.
      for (i = 0; i < lin - 1; i++) {
        for (j = 1; j < col; j++) {
          glcm[mat[i][j]][mat[i+1][j-1]]++;
        }
      }
      *(metricas + 18) = contraste(glcm, glcm_lin_col);;
      *(metricas + 19) = energia(glcm, glcm_lin_col);
      *(metricas + 20) = homogeneidade(glcm, glcm_lin_col);
    } else if (m == 7) {

      // Incrementa a posição [elemento][elemento vizinho da diagonal de baixo da direita] da matriz glcm.
      for (i = 0; i < lin - 1; i++) {
        for (j = 0; j < col - 1; j++) {
          glcm[mat[i][j]][mat[i+1][j+1]]++;
        }
      }
      *(metricas + 21) = contraste(glcm, glcm_lin_col);;
      *(metricas + 22) = energia(glcm, glcm_lin_col);
      *(metricas + 23) = homogeneidade(glcm, glcm_lin_col);
    }
    // Libera a memória utilizada pela matriz GLCM.
    for (i = 0; i < glcm_lin_col; i++) {
      free(*(glcm+i));
    }
    free(glcm);
  }
}

void normalizar_vetor_dados(float *ilbp_glcm, float *ilbp_glcm_normalizado) {

  // Declaração de variáveis locais.
  int i;
  float maior = 0, menor = 9999999;

  // Pega o menor e maior elemento do vetor ilbp_glcm.
  for (i = 0; i < 536; i++) {
    if (*(ilbp_glcm + i) > maior) {
      maior = *(ilbp_glcm + i);
    }
    if (menor > *(ilbp_glcm + i)) {
      menor = *(ilbp_glcm + i);
    }
  }

  // Monta o vetor normalizado de acordo com o cálculo.
  for (i = 0; i < 536; i++) {
    *(ilbp_glcm_normalizado + i) = (*(ilbp_glcm + i) - menor) / (maior - menor);
  }
}

float distancia_euclidiana(float *referencia, float **comparador, int posicao) {

  // Declaração de variáveis locais.
  int i, j;
  float resultado = 0.0, elemento_vetor_referencia, elemento_vetor_comparador, diferenca;

  // faz a distância euclidiana entre os vetores.
  for (j = 0; j < 536; j++) {
    elemento_vetor_referencia = *(referencia + j);
    elemento_vetor_comparador = *(*(comparador+posicao)+j);
    diferenca = elemento_vetor_referencia - elemento_vetor_comparador;
    resultado += pow(diferenca, 2);
  }
  resultado = sqrt(resultado);

  return resultado;
}
