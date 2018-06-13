#include <stdio.h>
#include <stdlib.h>

typedef struct binary_tree {
  int info;
  struct binary_tree *left;
  struct binary_tree *right;
} BinaryTree;

BinaryTree *createBinaryTree();

void insertBinaryTree(BinaryTree *root, int value);

void freeNode(BinaryTree *node);

void freeBinaryTree(BinaryTree *root);

int getHeight(BinaryTree *root);

void printPreOrder(BinaryTree *root);
// preOrder: visita a raiz, o filho da esquerda e o filho da direita
void printInOrder(BinaryTree *root);
// inOrder: visita o filho da esquerda, a raiz e o filho da direita
void printPostOrder(BinaryTree *root);
// postOrder: visita o filho da esquerda, o filho da direita e a raiz

////////////////////////// main /////////////////////////////
int main () {

  BinaryTree *root = createBinaryTree(); // Aponta para o primeiro elemento da arvore.


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
  BinaryTree *new = (BinaryTree*) malloc(sizeof(BinaryTree));
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
    BinaryTree *current = *root;
    BinaryTree *previous = NULL;

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
void freeNode(BinaryTree *node) {
  if(node == NULL) { // verifica se a alocacao deu certo
    return;
  }
  freeNode(node->left); // vai a esquerda e libera
  freeNode(node->right); // vai a direita e libera
  free(node); // libera o no apos liberar tudo a direita e a esquerda
  node = NULL;
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

///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
