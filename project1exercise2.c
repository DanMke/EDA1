/*
a) Escreva uma função "recebe_notas" que recebe por referência
(utilizando ponteiro) um vetor NOTAS, contendo 10 notas, e
recebe por valor o número de elementos do vetor, e retorna outro
vetor APR, também com 10 posições, que deve ser preenchido
com 1 quando a NOTA referente a i-ésima posição é maior ou igual
a 6.0 e 0 caso contrário. O vetor APR indica se o aluno foi
aprovado (1) ou não (0).
b) Escreva uma função "conta_notas" que recebe por referência
(utilizando ponteiros) o vetor APR e por valor o número de
elementos do vetor, e retorna também por referência (utilizando
ponteiros) o número de aprovados e o número de reprovados.
c) Escreva uma função "percent_aprov" que recebe o número de
aprovados e o número de reprovados e retorna por referência
(utilizando ponteiros) a percentagem de reprovados e a
percentagem de aprovados. Deve retornar também, via return, o
valor 1 se mais da metade da turma foi aprovada e 0 caso
contrário.
d) Escreva um programa principal que solicita 10 notas ao usuário,
armazena essas notas no vetor NOTAS e, por meio das chamadas
das funções que foram criadas, mostre:
- Quantidade de aprovados;
- Quantidade de reprovados;
- Percentual de aprovados;
- Percentual de reprovados; e
- Se mais da metade da turma aprovada.
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 10

void *recebe_notas(float *notas, int valor_max, int *APR);
void conta_notas(int *APR, int valor_max, int *aprovados, int *reprovados);
int percent_aprov(int *aprovados, int *reprovados, float *porcentagem_aprovados, float *porcentagem_reprovados);

int main (int argc, char *argv[]) {
  int APR[MAX], contador, maioria, aprovados = 0, reprovados = 0;
  float notas[MAX], porcentagem_aprovados = 0, porcentagem_reprovados = 0;

  printf("Insira as 10 notas: \n");

  for (contador = 0; contador < MAX; contador++) {
    scanf("%f", &notas[contador]);
  }

  recebe_notas(notas, MAX, APR);

  conta_notas(APR, MAX, &aprovados, &reprovados);

  maioria = percent_aprov(&aprovados, &reprovados, &porcentagem_aprovados, &porcentagem_reprovados);

  printf("Quantidade de aprovados: %d\n", (aprovados));
  printf("Quantidade de reprovados: %d\n", (reprovados));
  printf("Percentual de aprovados: %.2lf%%\n", (porcentagem_aprovados * 100));
  printf("Percentual de reprovados: %.2lf%%\n", (porcentagem_reprovados * 100));

  if (maioria == 1) {
    printf("Mais da metade da turma foi aprovada!\n");
  } else {
    printf("Menos da metade da turma foi aprovada!\n");
  }

  return 0;
}

void *recebe_notas(float *notas, int valor_max, int *APR) {
  int contador;

  for (contador = 0; contador < valor_max; contador++) {
    if (*(notas + contador) >= 6.0) {
      *(APR + contador) = 1;
    } else {
      *(APR + contador) = 0;
    }
  }
}

void conta_notas(int *APR, int valor_max, int *aprovados, int *reprovados) {
  int contador;

  for (contador = 0; contador < valor_max; contador++) {
    if (*(APR + contador) == 1) {
      (*aprovados)++;
    } else {
      (*reprovados)++;
    }
  }
}

int percent_aprov(int *aprovados, int *reprovados, float *porcentagem_aprovados, float *porcentagem_reprovados) {
  *porcentagem_aprovados = (*aprovados) / ((*aprovados) + (*reprovados));
  *porcentagem_reprovados = (*reprovados) / ((*aprovados) + (*reprovados));

  if (*porcentagem_aprovados > 0.5) {
    return 1;
  } else {
    return 0;
  }
}
