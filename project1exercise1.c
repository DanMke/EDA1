/*
a) Escreva um programa principal que solicita ao usuário 10 valores
reais que são guardados em um vetor ENTRADAS e outros 10
valores reais que são guardados em um vetor PESOS. O programa
também deve solicitar ao usuário o valor do limiar T. Em seguida a
função “fneuronio” descrita abaixo deve ser chamada.
b) Escreva uma função “fneuronio”, que recebe por referência
(utilizando ponteiros) os vetores ENTRADAS e PESOS, preenchidos
pelo usuário, além dos valores do limiar T e do número máximo de
elementos do vetor ENTRADAS, e retorna (utilizando ponteiros) o
valor 1 caso o neurônio esteja excitado ou 0 caso o neurônio esteja
inibido.
c) No programa principal, verificar se o valor de retorno da função
“fneuronio” foi 1 ou 0 e escrever na tela do computador a
mensagem “Neurônio ativado!” ou “Neurôno inibido!”,
respectivamente.
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 10

void fneuronio(float *entradas, float *pesos, float limiarT, int valorMax, int *excitada);

int main (int argc, char *argv[]) {
  int contador, status = 0;
  float entradas[MAX], pesos[MAX], limiarT;

  printf("Insira as 10 entradas: \n");

  for (contador = 0; contador < MAX; contador++) {
    scanf("%f\n", &entradas[contador]);
  }

  printf("Insira os 10 pesos: \n");

  for contador = 0; contador < MAX; contador++) {
    scanf("%f\n", &pesos[contador]);
  }

  fneuronio(entradas, pesos, limiarT, MAX, &status);



  return 0;
}

void fneuronio(float *entradas, float *pesos, float limiarT, int valorMax, int *excitada) {
  int contador = 0;
  float somaP = 0.0;

  for (contador = 0; contador < valorMAX; i++) {
    somaP += *(entradas + contador) * *(pesos + contador);
  }

  if (somaP > limiarT) {
    *excitada = 1;
  } else {
    *excitada = 0;
  }

}
