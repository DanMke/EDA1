#include <stdio.h>
#include <stdlib.h>

struct node {
  int info;
  struct node *left;
  struct node *right;
};

typedef struct node *BinaryTree;

BinaryTree *createBinaryTree();

void insertBinaryTree(BinaryTree *root, int value);

void freeNode(struct node *no);

void freeBinaryTree(BinaryTree *root);

int getHeight(BinaryTree *root);

void printPreOrder(BinaryTree *root);
// preOrder: visita a raiz, o filho da esquerda e o filho da direita
void printInOrder(BinaryTree *root);
// inOrder: visita o filho da esquerda, a raiz e o filho da direita
void printPostOrder(BinaryTree *root);
// postOrder: visita o filho da esquerda, o filho da direita e a raiz

struct node *removeCurrent(struct node *current);

void removeValue(BinaryTree *root, int value);

void searchValue(BinaryTree *root, int value);

BinaryTree *loadTreeFromFile();

////////////////////////// main /////////////////////////////
int main () {

  BinaryTree *root = createBinaryTree(); // Aponta para o primeiro elemento da arvore.

  /*
  int selector;
  scanf("%d\n", &selector);
  while(selector != 8) {
    if(selector == 1) {

    } else if(selector == 2) {

    } else {
      printf("Invalid option\n");
    }
  }

  */

  root = loadTreeFromFile();

  // printPreOrder(root);

  // printInOrder(root);

  // printPostOrder(root);

  // int h = getHeight(root);

  // freeBinaryTree(root);

  return 0;
}
///////////////////////// Funcoes /////////////////////////////
BinaryTree *createBinaryTree() {
  BinaryTree *root = (BinaryTree*) malloc(sizeof(BinaryTree));

  if(root != NULL) {
    *root = NULL;
  }

  return root;
}
///////////////////////////////////////////////////////////////
void insertBinaryTree(BinaryTree *root, int value) {
  if(root == NULL) {
    return;
  }
  struct node *new;
  new = (struct node*) malloc(sizeof(struct node));
  if(new == NULL) { // confere a alocacao
    return;
  }
  new->info = value;
  new->right = NULL;
  new->left = NULL;
  // onde inserir:
  if(*root == NULL) {
    *root = new;
  } else {
    struct node *current = *root;
    struct node *previous = NULL;

    while(current != NULL) { // quando tiver achado aonde ira ser inserido, para o while
      previous = current; // salva o atual pois o atual se tornara um dos filhos
      if(value == current->info) {
        free(new);
        printf("Elemento já existe na árvore.\n");
        return; // elemento ja existe
      }
      if(value > current->info) { // vai percorrer ate que um seja nulo e o anterior eh a folha que vai ficar em cima do elemento a ser inserido
        current = current->right;
      } else {
        current = current->left;
      }
    }
    // ve se vai ser inserido na direita ou na esquerda da folha encontrada acima
    if(value > previous->info) {
      previous->right = new;
    } else {
      previous->left = new;
    }
  }
}
///////////////////////////////////////////////////////////////
void freeNode(struct node *no) {
  if(no == NULL) { // verifica se a alocacao deu certo
    return;
  }
  freeNode(no->left); // vai a esquerda e libera
  freeNode(no->right); // vai a direita e libera
  free(no); // libera o no apos liberar tudo a direita e a esquerda
  no = NULL;
}
///////////////////////////////////////////////////////////////
void freeBinaryTree(BinaryTree *root) {
  if(root == NULL) { // verifica se a alocacao deu certo
    return;
  }
  freeNode(*root); // libera cada no
  free(root); // libera root

}
///////////////////////////////////////////////////////////////
int getHeight(BinaryTree *root) {
  if(root == NULL || *root == NULL) {
    return 0;
  }
  int leftHeight = getHeight(&((*root)->left)); // endereco do no da esquerda
  int rightHeight = getHeight(&((*root)->right)); // endereco do no da direita
  if(leftHeight > rightHeight) {
    return (leftHeight + 1);
  } else {
    return (rightHeight + 1);
  }
}
///////////////////////////////////////////////////////////////
void printPreOrder(BinaryTree *root) {
  if(root == NULL) {
    return;
  }
  if(*root != NULL) {
    printf("%d\n", (*root)->info); // imprime a raiz
    printPreOrder(&((*root)->left)); // imprime o da esquerda e seus filhos a esquerda
    printPreOrder(&((*root)->right)); // quando nao tem mais a esquerda imprime o da direita e seus filhos
  }
}
///////////////////////////////////////////////////////////////
void printInOrder(BinaryTree *root) {
  if(root == NULL) {
    return;
  }
  if(*root != NULL) {
    printInOrder(&((*root)->left));
    printf("%d\n", (*root)->info); // imprime o ultimo a esquerda, vai pra raiz imprime e vai pra direita e começa do ultimo a esquerda
    printInOrder(&((*root)->right));
  }
}
///////////////////////////////////////////////////////////////
void printPostOrder(BinaryTree *root) {
  if(root == NULL) {
    return;
  }
  if(*root != NULL) {
    printInOrder(&((*root)->left));
    printInOrder(&((*root)->right));
    printf("%d\n", (*root)->info); // imprime o ultimo a esquerda, depois vai pra direita imprime o mais a esquerda depois imprime o da direita e depois que imprime os filhos imprime a raiz.
  }
}
///////////////////////////////////////////////////////////////
struct node *removeCurrent(struct node *current) {
  struct node *node1, *node2;
  if(current->left == NULL) {
    // sem filho da esquerda, apontar para o filho da direita (trata no folha e com 1 filho)
    node2 = current->right;
    free(current);
    return node2;
  }
  node1 = current;
  node2 = current->left;
  while(node2->right != NULL) {
    // procura filho mais a direita da sub-arvore esquerda
    node1 = node2;
    node2 = node2->right;
  }
  if(node1 != current) {
    // copia o filho mais a direita na sub-arvore esquerda parada o lugar do no removido
    node1->right = node2->left;
    node2->left = current->left;
  }
  node2->right = current->right;
  free(current);
  return node2;
}
///////////////////////////////////////////////////////////////
void removeValue(BinaryTree *root, int value) {
  if(root == NULL) {
    return;
  }
  struct node *previous = NULL;
  struct node *current = *root;
  while(current != NULL) {
    if(value == current->info) { // verifica se achou o valor
      if(current == *root) { // verifica se eh a raiz
        *root = removeCurrent(current);
      } else {
        if(previous->right == current) { // verifica se eh o da direita
          previous->right = removeCurrent(current);
        } else { // verifica se eh da esquerda
          previous->left = removeCurrent(current);
        }
      }
    }
    previous = current;
    if(value > current->info) {
      current = current->right;
    } else {
      current = current->left;
    }
  }
}
///////////////////////////////////////////////////////////////
void searchValue(BinaryTree *root, int value) {
  int level = 1;
  if(root == NULL) {
    printf("O valor não está presente na árvore!\n");
  }
  struct node *current = *root;
  struct node *previous = NULL;
  while(current != NULL) {
    if(value == current->info) {
      printf("Valor encontrado %d\n", value); // se existe na arvore
      printf("Nível do nó: %d\n", level);
      if(previous != NULL) {
        printf("Pai: %d\n", previous->info);
        if(previous->right->info != current->info && previous->right != NULL) {
          printf("Irmão: %d\n", previous->right->info);
        } else if (previous->left->info != current->info && previous->left != NULL){
          printf("Irmão: %d\n", previous->left->info);
        }
      }
    }
    previous = current;
    level++;
    if(value > current->info) {
      current = current->right;
    } else {
      current = current->left;
    }
  }
  printf("O valor não está presente na árvore!\n");
}
///////////////////////////////////////////////////////////////
BinaryTree *loadTreeFromFile() {
  FILE *fp;
  int number;

  BinaryTree *root = createBinaryTree();

  fp = fopen("BSTs/bst1.txt", "r");

  if (fp == NULL) {
    printf("not found\n");
  }

  while(!feof(fp)) {
    fscanf(fp, "%d", &number);
    printf("%d\n", number);
    insertBinaryTree(root, number);
  }

  fclose(fp);

  printf("Endereço da raíz: %d\n", &root); //TODO

  return root;
}
