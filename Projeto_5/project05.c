#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  int info;
  struct node *left;
  struct node *right;
};

typedef struct node *BinaryTree;

#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

typedef int ELEMENT;

typedef struct BSTNode_struct BSTNode;

struct BSTNode_struct {
    BSTNode *ptesq, *ptdir;

    //length of the edge from this no to its children
    int edge_length;

    int altura;

    ELEMENT element;

    //-1=I am left, 0=I am root, 1=right
    int parent_dir;

    //max supported unit32 in dec, 10 digits max
    char label[11];
};

//adjust gap between left and right nos
int gap = 3;

//used for printing next no in the same level,
//this is the x coordinate of the next char printed
int print_next;

int MIN(int X, int Y) {
    return ((X) < (Y)) ? (X) : (Y);
}

int MAX(int X, int Y) {
    return ((X) > (Y)) ? (X) : (Y);
}

BSTNode *build_ascii_tree_recursive(struct node *arvore) {
    BSTNode *no;

    if (arvore == NULL) return NULL;

    no = malloc(sizeof(BSTNode));
    no->ptesq = build_ascii_tree_recursive(arvore->left);
    no->ptdir = build_ascii_tree_recursive(arvore->right);

    if (no->ptesq != NULL) {
        no->ptesq->parent_dir = -1;
    }

    if (no->ptdir != NULL) {
        no->ptdir->parent_dir = 1;
    }

    sprintf(no->label, "%d", arvore->info);
    no->element = strlen(no->label);

    return no;
}


//Copy the tree into the ascii no structre
BSTNode *build_ascii_tree(struct node *arvore) {
    BSTNode *no;
    if (arvore == NULL) return NULL;
    no = build_ascii_tree_recursive(arvore);
    no->parent_dir = 0;
    return no;
}

//Free all the nos of the given tree
void free_ascii_tree(BSTNode *no) {
    if (no == NULL) return;
    free_ascii_tree(no->ptesq);
    free_ascii_tree(no->ptdir);
    free(no);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(BSTNode *no, int x, int y) {
    int i, eh_esq;
    if (no == NULL) return;
    eh_esq = (no->parent_dir == -1);
    lprofile[y] = MIN(lprofile[y], x - ((no->element - eh_esq) / 2));
    if (no->ptesq != NULL) {
        for (i = 1; i <= no->edge_length && y + i < MAX_HEIGHT; i++) {
            lprofile[y + i] = MIN(lprofile[y + i], x - i);
        }
    }
    compute_lprofile(no->ptesq, x - no->edge_length - 1, y + no->edge_length + 1);
    compute_lprofile(no->ptdir, x + no->edge_length + 1, y + no->edge_length + 1);
}

void compute_rprofile(BSTNode *no, int x, int y) {
    int i, n_eh_esq;
    if (no == NULL) return;
    n_eh_esq = (no->parent_dir != -1);
    rprofile[y] = MAX(rprofile[y], x + ((no->element - n_eh_esq) / 2));
    if (no->ptdir != NULL) {
        for (i = 1; i <= no->edge_length && y + i < MAX_HEIGHT; i++) {
            rprofile[y + i] = MAX(rprofile[y + i], x + i);
        }
    }
    compute_rprofile(no->ptesq, x - no->edge_length - 1, y + no->edge_length + 1);
    compute_rprofile(no->ptdir, x + no->edge_length + 1, y + no->edge_length + 1);
}

//This function fills in the edge_length and
//altura fields of the specified tree
void filledge(BSTNode *no) {
    int h, hmin, i, delta;
    if (no == NULL) return;
    filledge(no->ptesq);
    filledge(no->ptdir);

   /* first fill in the edge_length of no */
   if (no->ptdir == NULL && no->ptesq == NULL) {
        no->edge_length = 0;
    }
    else {
        if (no->ptesq != NULL) {
            for (i = 0; i < no->ptesq->altura && i < MAX_HEIGHT; i++) {
                rprofile[i] = -INFINITY;
            }
            compute_rprofile(no->ptesq, 0, 0);
            hmin = no->ptesq->altura;
        }
        else {
            hmin = 0;
        }
        if (no->ptdir != NULL) {
            for (i = 0; i < no->ptdir->altura && i < MAX_HEIGHT; i++) {
                lprofile[i] = INFINITY;
            }
            compute_lprofile(no->ptdir, 0, 0);
            hmin = MIN(no->ptdir->altura, hmin);
        }
        else {
            hmin = 0;
        }
        delta = 4;
        for (i = 0; i < hmin; i++) {
            delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
        }

        //If the no has two children of altura 1, then we allow the
        //two leaves to be within 1, instead of 2
        if (((no->ptesq != NULL && no->ptesq->altura == 1) ||
             (no->ptdir != NULL && no->ptdir->altura == 1)) && delta > 4) {
            delta--;
        }

        no->edge_length = ((delta + 1) / 2) - 1;
    }

    //now fill in the altura of no
    h = 1;
    if (no->ptesq != NULL) {
        h = MAX(no->ptesq->altura + no->edge_length + 1, h);
    }
    if (no->ptdir != NULL) {
        h = MAX(no->ptdir->altura + no->edge_length + 1, h);
    }
    no->altura = h;
}

//This function prints the given level of the given tree, assuming
//that the no has the given x cordinate.
void printLevel(BSTNode *no, int x, int level) {
    int i, isleft;
    if (no == NULL) return;
    isleft = (no->parent_dir == -1);
    if (level == 0) {
        for (i = 0; i < (x - print_next - ((no->element - isleft) / 2)); i++) {
            printf(" ");
        }
        print_next += i;
        printf("%s", no->label);
        print_next += no->element;
    }
    else if (no->edge_length >= level) {
        if (no->ptesq != NULL) {
            for (i = 0; i < (x - print_next - (level)); i++) {
                printf(" ");
            }
            print_next += i;
            printf("/");
            print_next++;
        }
        if (no->ptdir != NULL) {
            for (i = 0; i < (x - print_next + (level)); i++) {
                printf(" ");
            }
            print_next += i;
            printf("\\");
            print_next++;
        }
    }
    else {
        printLevel(no->ptesq,
                   x - no->edge_length - 1,
                   level - no->edge_length - 1);
        printLevel(no->ptdir,
                   x + no->edge_length + 1,
                   level - no->edge_length - 1);
    }
}

//prints ascii tree for given Tree structure
void showTree(BinaryTree *raiz) {
    BSTNode *proot;
    int xmin, i;
    if (raiz == NULL) return;
    proot = build_ascii_tree(*raiz);
    filledge(proot);
    for (i = 0; i < proot->altura && i < MAX_HEIGHT; i++) {
        lprofile[i] = INFINITY;
    }
    compute_lprofile(proot, 0, 0);
    xmin = 0;
    for (i = 0; i < proot->altura && i < MAX_HEIGHT; i++) {
        xmin = MIN(xmin, lprofile[i]);
    }
    for (i = 0; i < proot->altura; i++) {
        print_next = 0;
        printLevel(proot, -xmin, i);
        printf("\n");
    }
    if (proot->altura >= MAX_HEIGHT) {
        printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
    }
    free_ascii_tree(proot);
}

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

int isFull(struct node *root);

void searchValue(BinaryTree *root, int value);

BinaryTree *loadTreeFromFile();

////////////////////////// main /////////////////////////////
int main () {

  int full;

  BinaryTree *root = createBinaryTree(); // Aponta para o primeiro elemento da arvore.

  root = loadTreeFromFile();

  /*
  int selector;

  while(selector != 10) {

    printf("\n-------------------------------------------------------------------------------------------\n");
    printf("---\t\t\tSELECIONE A OPÇÃO DESEJADA:\t\t\t\t\t---\n");
    printf("-------------------------------------------------------------------------------------------\n");
    printf("---\t\t\t(1) SHOW TREE\t\t\t\t\t---\n");
    printf("---\t\t\t(2) IS FULL\t\t\t\t\t\t---\n");
    printf("---\t\t\t(3) SEARCH VALUE\t\t\t\t\t\t---\n");
    printf("---\t\t\t(4) GET HEIGHT\t\t---\n");
    printf("---\t\t\t(5) REMOVE VALUE\t\t---\n");
    printf("---\t\t\t(6) PRINT PREORDER\t\t---\n");
    printf("---\t\t\t(7) PRINT INORDER\t\t---\n");
    printf("---\t\t\t(8) PRINT POSTORDER\t\t---\n");
    printf("---\t\t\t(9) BALANCE TREE\t\t---\n");
    printf("---\t\t\t(10) EXIT\t\t\t\t\t\t\t---\n");
    printf("-------------------------------------------------------------------------------------------\n");

    scanf("%d\n", &selector);

    if(selector == 1) {

    } else if(selector == 2) {
      full = isFull(*root);

      if(full == 1) {
        printf("A árvore é cheia!\n");
      } else {
        printf("A árvore não é cheia!\n");
      }
    } else {
      printf("Invalid option\n");
    }
  }

  */



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
int isFull(struct node *root) {
  // se a arvore eh vazia
	if (root == NULL)
		return 1;

	// If leaf node
	if (root->left == NULL && root->right == NULL)
		return 1;

	// If both left and right are not NULL, and left & right subtrees
	// are full
	if ((root->left) && (root->right))
		 if(isFull(root->left) && isFull(root->right)) {
       return 1;
     }

	// We reach here when none of the above if conditions work
	return 0;
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
    printf("not founded\n");
  }

  while(!feof(fp)) {
    fscanf(fp, "%d", &number);
    printf("%d\n", number);
    insertBinaryTree(root, number);
  }

  fclose(fp);

  printf("Endereço da raíz: %d\n", &root); //TODO

  showTree(root);

  return root;
}
