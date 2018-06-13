#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct voo {
  char codigo[7];
  char modo;
  int gas;
  struct voo *prox;
} Voo;

typedef struct fila {
  Voo *inicio;
  Voo *final;
} Fila;

Voo *cria_voo(char *codigo, char modo, int gas);
Voo *insere_voo_na_lista(Voo *lista, Voo *voo_inserido);
Fila *cria_fila();
void libera_fila(Fila *fi);
void insere_fila(Fila *fi, Voo *voo);
void remove_fila(Fila *fi);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

  int nDecol = 0, nAprox = 0, nVoos = 0, cont = 0;
  char *nomeVoos[64] = {"VG3001",
  "JJ4404", "LN7001", "TG1501", "GL7602", "TT1010", "AZ1009", "AZ1008",
  "AZ1010", "TG1506", "VG3002", "JJ4402", "GL7603", "RL7880", "AL0012",
  "TT4544", "TG1505", "VG3003", "JJ4403", "JJ4401", "LN7002", "AZ1002",
  "AZ1007", "GL7604", "AZ1006", "TG1503", "AZ1003", "JJ4403", "AZ1001",
  "LN7003", "AZ1004", "TG1504", "AZ1005", "TG1502", "GL7601",
  "TT4500", "RL7801", "JJ4410", "GL7607", "AL0029", "VV3390", "VV3392",
  "GF4681", "GF4690", "AZ1020", "JJ4435", "VG3010", "LF0920", "AZ1065",
  "LF0978", "RL7867", "TT4502", "GL7645", "LF0932", "JJ4434", "TG1510",
  "TT1020", "AZ1098", "BA2312", "VG3030", "BA2304", "KL5609",
  "KL5610", "KL5611"};


  int seed = time(NULL);
  srand(seed);
  nDecol = 10 + (rand() % 22);
  srand(seed+4);
  nAprox = 10 + (rand() % 22);
  nVoos = nAprox + nDecol;


  int *voos_selecionados, randomNumb = 0;
  voos_selecionados = (int *) calloc(64, sizeof(int));

  Voo *lista_inicial = NULL;
  Voo *voo_criado;

  int decolagens = 0, aproximacoes = 0;
  srand(seed+1);
  //Aleatoriza as posições de voos para decolagens
  for(cont = 0; cont < nDecol; cont++) {

    randomNumb = (rand() % 64);

    if( *(voos_selecionados + randomNumb) == 1) {
      cont--;
    }
    else if( *(voos_selecionados + randomNumb) == 0) {
      *(voos_selecionados + randomNumb) = 1;
      voo_criado = cria_voo( nomeVoos[randomNumb], 'D', -1);
      lista_inicial = insere_voo_na_lista(lista_inicial, voo_criado);

    }

  }

  srand(seed+2);
  //Aleatoriza as posições de voos para aproximações
  for(cont = 0; cont < nAprox; cont++) {
    randomNumb = (rand() % 64);

    if( *(voos_selecionados + randomNumb) == 1 ) {
      cont--;
    }
    else if( *(voos_selecionados + randomNumb) == 0) {
      *(voos_selecionados + randomNumb) = 1;
      int gas = (rand() % 13);
      voo_criado = cria_voo( nomeVoos[randomNumb], 'A', gas);
      lista_inicial = insere_voo_na_lista(lista_inicial, voo_criado);
    }

  }

  //libera vetor de 64 posições para seleção de voos
  free(voos_selecionados);



  Fila *fila_aprox = cria_fila();

  for (int i = 0; i < 13; i++) {

    Voo *elemento_atual = lista_inicial;
    Voo *elemento_anterior = NULL;

    while (elemento_atual != NULL) {

      if (elemento_atual->gas == i) {
        if (elemento_atual == lista_inicial) {
          lista_inicial = lista_inicial->prox;
        }
        else {
          elemento_anterior->prox = elemento_atual->prox;
        }

        Voo *aux = elemento_atual->prox;
        insere_fila(fila_aprox, elemento_atual);
        elemento_atual = aux;
      }

      else {
        elemento_anterior = elemento_atual;
        elemento_atual = elemento_atual->prox;
      }
    }
  }

  Fila *fila_decol = cria_fila();

  Voo *elemento_atual = lista_inicial;
  Voo *elemento_proximo = NULL;

  while(elemento_atual != NULL) {
    elemento_proximo = elemento_atual->prox;
    insere_fila(fila_decol, elemento_atual);
    elemento_atual = elemento_proximo;
  }

  printf("---------------------------------------------------------------------------------\n");
  printf("Aeroporto Internacional de Brasília\n");
  printf("Hora Inicial: 16:00\n");
  printf("Fila de Pedidos: [código do voo – P/D – prioridade]\n");
  printf("NVoos: %d\n", nVoos);
  printf("Naproximações: %d\n", nAprox);
  printf("NDecolagens: %d\n", nDecol);
  printf("---------------------------------------------------------------------------------\n\n\n");

  Voo *imprime_fila_aprox = fila_aprox->inicio;

  while (imprime_fila_aprox != NULL) {

    printf("[%s-%c-%d]\n", imprime_fila_aprox->codigo, imprime_fila_aprox->modo, imprime_fila_aprox->gas);

    imprime_fila_aprox = imprime_fila_aprox->prox;
  }

  Voo *imprime_fila_decol = fila_decol->inicio;

  while (imprime_fila_decol != NULL) {

    printf("[%s-%c]\n", imprime_fila_decol->codigo, imprime_fila_decol->modo);

    imprime_fila_decol = imprime_fila_decol->prox;
  }

  printf("\n\n\nListagem de eventos:\n\n\n");


  //~~~~~~~~~~~~~~~~/LÓGICA DO VOO/~~~~~~~~~~~~~~~~/

  int hora = 16, minuto = 0, verifica_gas = 1;
  int pista_1 = 0, pista_2 = 0, pista_3 = 0;

  while(fila_aprox->inicio != NULL || fila_decol->inicio != NULL) {

    //Verificação de Emergência//
    int gas_zero = 0;
    Voo *confere_gas = fila_aprox->inicio;

    while(confere_gas != NULL) {
      if(confere_gas->gas == 0) {
        gas_zero++;
      }
      confere_gas = confere_gas->prox;
    }

    if(gas_zero >= 3) {
      printf("\n\nALERTA GERAL DE DESVIO DE AERONAVE\n\n");
      //printf("Tentativa de pouso na pista 3\n\n");
    }

    //Pista 1//
    if(pista_1 == 0) {
      if(fila_aprox->inicio != NULL) {
        printf("Código do voo: %s - %c - %d\n", fila_aprox->inicio->codigo, fila_aprox->inicio->modo, fila_aprox->inicio->gas);
        printf("Status: [Aeronave Pousou]\n");
        printf("Horário do ínicio do procedimento: %02d:%02d\n", hora, minuto);
        printf("Número da pista: 1\n\n");
        pista_1 = 4;
        remove_fila(fila_aprox);
      }
      else if(fila_decol->inicio != NULL){
        printf("Código do voo: %s - %c\n", fila_decol->inicio->codigo, fila_decol->inicio->modo);
        printf("Status: [Aeronave Decolou]\n");
        printf("Horário do ínicio do procedimento: %02d:%02d\n", hora, minuto);
        printf("Número da pista: 1\n\n");
        pista_1 = 2;
        remove_fila(fila_decol);
      }
    }


    //Pista 2//
    if(pista_2 == 0) {
      if(fila_aprox->inicio != NULL) {
        printf("Código do voo: %s - %c - %d\n", fila_aprox->inicio->codigo, fila_aprox->inicio->modo, fila_aprox->inicio->gas);
        printf("Status: [Aeronave Pousou]\n");
        printf("Horário do ínicio do procedimento: %02d:%02d\n", hora, minuto);
        printf("Número da pista: 2\n\n");
        pista_2 = 4;
        remove_fila(fila_aprox);
      }
      else if(fila_decol->inicio != NULL) {
        printf("Código do voo: %s - %c\n", fila_decol->inicio->codigo, fila_decol->inicio->modo);
        printf("Status: [Aeronave Decolou]\n");
        printf("Horário do ínicio do procedimento: %02d:%02d\n", hora, minuto);
        printf("Número da pista: 2\n\n");
        pista_2 = 2;
        remove_fila(fila_decol);
      }
    }


    //Pista 3//
    if(gas_zero >= 3) {
      if(pista_3 == 0) {
        //printf("Pouso na pista 3 foi um sucesso!\n\n");
        printf("Código do voo: %s - %c - %d\n", fila_aprox->inicio->codigo, fila_aprox->inicio->modo, fila_aprox->inicio->gas);
        printf("Status: [Aeronave Pousou]\n");
        printf("Horário do ínicio do procedimento: %02d:%02d\n", hora, minuto);
        printf("Número da pista: 3\n\n");
        pista_3 = 4;
        remove_fila(fila_aprox);
      }// else {
        //printf("Pouso na pista 3 não deu certo, pista ocupada!\n\n");
      //}
    }

    if(pista_3 == 0) {
      if(fila_decol->inicio != NULL) {
        printf("Código do voo: %s - %c\n", fila_decol->inicio->codigo, fila_decol->inicio->modo);
        printf("Status: [Aeronave Decolou]\n");
        printf("Horário do ínicio do procedimento: %02d:%02d\n", hora, minuto);
        printf("Número da pista: 3\n\n");
        pista_3 = 2;
        remove_fila(fila_decol);
      }
    }

    //Verificação de Rotina//
    if(pista_1 > 0){
      pista_1--;
    }

    if(pista_2 > 0){
      pista_2--;
    }

    if(pista_3 > 0){
      pista_3--;
    }

    minuto+= 5;
    if(minuto >= 60) {
      hora++;
      minuto = 0;
      if(hora >= 24) {
        hora = 0;
      }
    }

    if(verifica_gas%10 == 0) {
      Voo *gasta_gas = fila_aprox->inicio;

      while(gasta_gas != NULL) {
        gasta_gas->gas = (gasta_gas->gas) - 1;
        gasta_gas = gasta_gas->prox;
      }
      gasta_gas = fila_aprox->inicio;
    }

    // Derruba os que estão com combustível menos de 0
    Voo *avioes_que_caem = fila_aprox->inicio;

    while(avioes_que_caem != NULL) {
      if(avioes_que_caem->gas < 0) {
        printf("ALERTA CRÍTICO: Aeronave irá cair\n\n");
        printf("Código do voo: %s - %c - %d\n", avioes_que_caem->codigo, avioes_que_caem->modo, avioes_que_caem->gas);
        printf("Status: [Aeronave Caiu]\n\n");
        remove_fila(fila_aprox);
      }
      avioes_que_caem = avioes_que_caem->prox;
    }

    verifica_gas++;

  }

  libera_fila(fila_aprox);
  libera_fila(fila_decol);

  printf("---------------------------PROGRAMA FINALIZADO---------------------------\n");

  return 0;
}

///////////////////////////////////////Funções////////////////////////////////////////

Voo *cria_voo(char *codigo, char modo, int gas) {

  Voo *voo_criado = (Voo *)malloc(sizeof(Voo));
  // Iguala os elementos da struct aos elementos recebidos como parâmetros.
  strcpy(voo_criado->codigo, codigo);
  voo_criado->modo = modo;
  voo_criado->gas = gas;
  voo_criado->prox = NULL;
  // Retorna o contato.
  return voo_criado;
}

///////////////////////////////////////////////////////////////////////////////

Voo *insere_voo_na_lista(Voo *lista, Voo *voo_inserido) {

  if(lista != NULL) {
    voo_inserido->prox = lista;
  }

  return voo_inserido;

}

///////////////////////////////////////////////////////////////////////////////

Fila *cria_fila() {
  Fila *fi = (Fila *) malloc(sizeof(Fila));

  if (fi != NULL) {
    fi->inicio = NULL;
    fi->final = NULL;
  }

  return fi;
}

///////////////////////////////////////////////////////////////////////////////

void libera_fila(Fila *fi) {
  if (fi != NULL) {
    Voo *no;
    while (fi->inicio != NULL) {
      no = fi->inicio;
      fi->inicio = fi->inicio->prox;
      free(no);
    }
    free(fi);
  }
}

///////////////////////////////////////////////////////////////////////////////

void insere_fila(Fila *fi, Voo *voo) {

  if (fi == NULL) {
    printf("Alocação falhou!\n");
    exit(1);
  }

  if (voo == NULL) {
    printf("Alocação falhou!\n");
    exit(1);
  }

  voo->prox = NULL;

  if (fi->final == NULL) { // inicio da fila
    fi->inicio = voo;
  } else {
    fi->final->prox = voo;
  }
  fi->final = voo;
}

///////////////////////////////////////////////////////////////////////////////

void remove_fila(Fila *fi) {
  if (fi == NULL) {
    printf("Alocação falhou!\n");
    exit(1);
  }
  if (fi->inicio == NULL) {
    printf("Fila está vazia\n");
    exit(1);
  }
  Voo *no = fi->inicio;
  fi->inicio = fi->inicio->prox;
  if (fi->inicio == NULL) {
    fi->final = NULL;
  }
  free(no);
}
