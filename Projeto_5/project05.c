#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct treeAVL {
  int key;
  struct treeAVL *left;
  struct treeAVL *right;
  int height;
};

int height(struct treeAVL *N);

int max(int a, int b);

struct treeAVL *newNode(int key);

struct treeAVL *rightRotate(struct treeAVL *y);

struct treeAVL *leftRotate(struct treeAVL *x);

int getBalance(struct treeAVL *N);

struct treeAVL *insertTreeAVL(struct treeAVL* node, int key);

struct treeAVL *createTreeAVL();

struct treeAVL *loadTreeAVL(char *nameOfFile);

///////////////////////////////////////////////////////////////////////////

typedef struct node {
  int info;
  struct node *left;
  struct node *right;
} BinaryTree;

int verifyBalancedTree(BinaryTree *root);

BinaryTree *createBinaryTree();

BinaryTree *insertBinaryTree(BinaryTree *root, int value);

void freeBinaryTree(BinaryTree *root);

int getHeight(BinaryTree *root);

void printPreOrder(BinaryTree *root);
// preOrder: visita a raiz, o filho da esquerda e o filho da direita
void printInOrder(BinaryTree *root);
// inOrder: visita o filho da esquerda, a raiz e o filho da direita
void printPostOrder(BinaryTree *root);
// postOrder: visita o filho da esquerda, o filho da direita e a raiz

void removeValue(BinaryTree *root, int value);

int isFull(struct node *root);

void searchValue(BinaryTree *root, int value);

BinaryTree *loadTreeFromFile();

int saveTree(BinaryTree *root, int is_left, int offset, int depth, char **s);

void showTree(BinaryTree *root);

int getHeightAVL(struct treeAVL *root);

int saveTreeAVL(struct treeAVL *root, int is_left, int offset, int depth, char **s);

void showTreeAVL(struct treeAVL *root);

void freeTreeAVL(struct treeAVL *node);


////////////////////////// main /////////////////////////////
int main () {

  int full;

  BinaryTree *root;
  root = createBinaryTree(); // Aponta para o primeiro elemento da arvore.

  int selector, fileSelector = 0;
  char getName[8];
  char fileName[13];

  while(selector != 10) {

    if(fileSelector == 0) {
      while(fileSelector != 1) {
        printf("\n---------------------------------------------------\n");
        printf("-\tBINARY TREE HOUSE:\t\t\t---\n");
        printf("-\tType the name of the file to be opened: ---\n");
        printf("---------------------------------------------------\n");
        printf("---\t\t- bst1.txt\t\t\t---\n");
        printf("---\t\t- bst2.txt\t\t\t---\n");
        printf("---\t\t- bst3.txt\t\t\t---\n");
        printf("---\t\t- bst4.txt\t\t\t---\n");
        printf("---\t\t- bst5.txt\t\t\t---\n");
        printf("---\t\t- bst6.txt\t\t\t---\n\n");
        printf("---------------------------------------------------\n");
        printf("---\t'E' for exit the program \t\t---\n");
        printf("---------------------------------------------------\n");


        scanf(" %s", getName);

        if(strlen(getName) == 1 && getName[0] == 'E') {
          fileSelector = 0;
          freeBinaryTree(root);
          break;
        }

        if(strlen(getName) <= 8) {
          if(getName[0] == 'b' && getName[1] == 's' && getName[2] == 't'
              && getName[4] == '.' && getName[5] == 't' && getName[6] == 'x' && getName[7] == 't') {
            if(getName[3] - '0' > 0 && getName[3] - '0' < 7){

              fileSelector = 1;
              fileName[0] = 'B';
              fileName[1] = 'S';
              fileName[2] = 'T';
              fileName[3] = 's';
              fileName[4] = '/';
              fileName[5] = getName[0];
              fileName[6] = getName[1];
              fileName[7] = getName[2];
              fileName[8] = getName[3];
              fileName[9] = getName[4];
              fileName[10] = getName[5];
              fileName[11] = getName[6];
              fileName[12] = getName[7];

              printf(" %s\n", fileName);
              root = loadTreeFromFile(fileName);

            }
            else {
              printf("File name is invalid: file does not exist\n");
              fileSelector = 0;
            }
          }
          else {
            printf("File name is invalid: Typo on name\n");
            fileSelector = 0;
          }
        }

        else {
          printf("File name is invalid: Too Long\n");
          fileSelector = 0;
        }

      }
    }

    if(fileSelector == 0){
      break;
    }

    printf("\n-------------------------------------------------------------------------------------------\n");
    printf("---\t\t\tSELECIONE A OPÇÃO DESEJADA:\t\t\t\t\t---\n");
    printf("-------------------------------------------------------------------------------------------\n");
    printf("---\t\t\t(1) SHOW TREE\t\t\t\t\t\t\t---\n");
    printf("---\t\t\t(2) IS FULL\t\t\t\t\t\t\t---\n");
    printf("---\t\t\t(3) SEARCH VALUE\t\t\t\t\t\t---\n");
    printf("---\t\t\t(4) GET HEIGHT\t\t\t\t\t\t\t---\n");
    printf("---\t\t\t(5) REMOVE VALUE\t\t\t\t\t\t---\n");
    printf("---\t\t\t(6) PRINT PREORDER\t\t\t\t\t\t---\n");
    printf("---\t\t\t(7) PRINT INORDER\t\t\t\t\t\t---\n");
    printf("---\t\t\t(8) PRINT POSTORDER\t\t\t\t\t\t---\n");
    printf("---\t\t\t(9) BALANCE TREE\t\t\t\t\t\t---\n");
    printf("---\t\t\t(10) EXIT\t\t\t\t\t\t\t---\n");
    printf("-------------------------------------------------------------------------------------------\n");

    scanf("%d", &selector);

    if(selector == 1) {
      showTree(root);
    }

    else if(selector == 2) {
      full = isFull(root);
      if(full == 1) {
        printf("The tree is full!\n");
      } else {
        printf("The tree is not full!\n");
      }
    }
    else if(selector == 3) {
      printf("Enter the number to be searched in the tree: \n");
      int num;
      scanf("%d", &num);
      searchValue(root, num);
    }
    else if(selector == 4) {
      int h = getHeight(root);
      printf("The height of the tree is: %d\n", h);
    }
    else if(selector == 5) {
      printf("Enter the number to be removed from the tree: \n");
      int num;
      scanf("%d", &num);
      removeValue(root, num);
    }
    else if(selector == 6) {
      printf("\n\nPre order:\n\n");
      printPreOrder(root);
    }
    else if(selector == 7) {
      printf("\n\nIn order:\n\n");
      printInOrder(root);
    }
    else if(selector == 8) {
      printf("\n\nPost order:\n\n");
      printPostOrder(root);
    }
    else if(selector == 9) {
      int verifyBalance = verifyBalancedTree(root);
      if(verifyBalance == 1) {
        printf("The tree is balanced!\n");
      } else {
        printf("The tree is not balanced!\n");
        printf("The tree balanced is:\n");
        struct treeAVL *rootAVL;
        rootAVL = createTreeAVL();
        rootAVL = loadTreeAVL(fileName);
        showTreeAVL(rootAVL);
        freeTreeAVL(rootAVL);
      }
    }
    else if(selector == 10) {
      freeBinaryTree(root);
      break;
    }
    else {
      printf("Invalid option\n");
    }
  }

  return 0;
}

///////////////////////// Funcoes /////////////////////////////
BinaryTree *createBinaryTree() {
  BinaryTree *root;
  root = (BinaryTree *) malloc(sizeof(BinaryTree));

  if(root != NULL) {
    root = NULL;
  }

  return root;
}

///////////////////////////////////////////////////////////////
BinaryTree *loadTreeFromFile(char *nameOfFile) {
  FILE *fp;
  int number;

  BinaryTree *root = createBinaryTree();

  fp = fopen(nameOfFile, "r");

  if (fp == NULL) {
    printf("Was not found!\n");
  }

  while(!feof(fp)) {
    fscanf(fp, "%d", &number);
    root = insertBinaryTree(root, number);
  }

  fclose(fp);

  return root;
}

///////////////////////////////////////////////////////////////
BinaryTree *insertBinaryTree(BinaryTree *root, int value) {

  BinaryTree *new;
  new = (BinaryTree *) malloc(sizeof(BinaryTree));
  if(new == NULL) { // confere a alocacao
    exit(1);
  }

  new->info = value;
  new->right = NULL;
  new->left = NULL;

  // onde inserir:
  if(root == NULL) {
    root = new;
  }
  else {
    BinaryTree *current = root;
    BinaryTree *previous = NULL;

    while(current != NULL) { // quando tiver achado aonde ira ser inserido, para o while
      previous = current; // salva o atual pois o atual se tornara um dos filhos
      if(value == current->info) {
        free(new);
        printf("Element already exists in the tree.\n");
        exit(2);// elemento ja existe
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
  return root;
}

///////////////////////////////////////////////////////////////
void freeBinaryTree(BinaryTree *node) {
  if(node == NULL) { // verifica se a alocacao deu certo
    return;
  }
  freeBinaryTree(node->left); // vai a esquerda e libera
  freeBinaryTree(node->right); // vai a direita e libera
  free(node); // libera o node apos liberar tudo a direita e a esquerda
  node = NULL;
}

///////////////////////////////////////////////////////////////
int getHeight(BinaryTree *root) {
  if(root == NULL) {
    return 0;
  }
  int leftHeight = getHeight(root->left); // endereco do no da esquerda
  int rightHeight = getHeight(root->right); // endereco do no da direita
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
  if(root != NULL) {
    printf("%d\n", root->info); // imprime a raiz
    printPreOrder(root->left); // imprime o da esquerda e seus filhos a esquerda
    printPreOrder(root->right); // quando nao tem mais a esquerda imprime o da direita e seus filhos
  }
}

///////////////////////////////////////////////////////////////
void printInOrder(BinaryTree *root) {
  if(root == NULL) {
    return;
  }
  if(root != NULL) {
    printInOrder(root->left);
    printf("%d\n", root->info); // imprime o ultimo a esquerda, vai pra raiz imprime e vai pra direita e começa do ultimo a esquerda
    printInOrder(root->right);
  }
}

///////////////////////////////////////////////////////////////
void printPostOrder(BinaryTree *root) {
  if(root == NULL) {
    return;
  }
  if(root != NULL) {
    printPostOrder(root->left);
    printPostOrder(root->right);
    printf("%d\n", root->info); // imprime o ultimo a esquerda, depois vai pra direita imprime o mais a esquerda depois imprime o da direita e depois que imprime os filhos imprime a raiz.
  }
}

//////////////////////////////////////////////////////////////////
void removeValue(BinaryTree *root, int value) {
  if(root == NULL) {
    return;
  }

  int verifier = 0;
  BinaryTree *previous = NULL;
  BinaryTree *current = root;
  while(current != NULL) {
    if(value == current->info) { // verifica se achou o valor
      verifier = 1;
      if(current->left != NULL && current->right != NULL) { // verifica se tem 2 filhos
        previous = current;
        BinaryTree *currentFather;
        currentFather = current;
        current = current->right;

        if(current->left == NULL) {
          printf("Value was removed: %d\n", previous->info);
          previous->info = current->info;
          if(current->right != NULL) {
            previous->right = current->right;
          }
          else{
            previous->right = NULL;
          }
        }
        else {
          while(current->left != NULL) {
            currentFather = current;
            current = current->left;
          }
          printf("Value was removed: %d\n", previous->info);
          previous->info = current->info;
          currentFather->left = NULL;
        }
        free(current);
        break;

      }
      else if(current->left == NULL && current->right == NULL){//verifica se é uma folha

        if(previous->right == current) { // verifica se eh o da direita
          previous->right = NULL;
        }
         else { // verifica se eh da esquerda
          previous->left = NULL;
        }
        printf("Value was removed: %d\n", current->info);
        free(current);
        break;
      }
      else { //verifica se tem só 1 filho
        if(current->right != NULL) {
          if(previous->right == current) { // verifica se eh o da direita
            previous->right = current->right;
          } else { // verifica se eh da esquerda
            previous->left = current->right;
          }
        }
        else if(current->left != NULL) {
          if(previous->right == current) { // verifica se eh o da direita
            previous->right = current->left;
          } else { // verifica se eh da esquerda
            previous->left = current->left;
          }
        }
        printf("Value was removed: %d\n", current->info);
        free(current);
        break;
      }
    }
    previous = current;
    if(value > current->info) {
      current = current->right;
    } else {
      current = current->left;
    }
  }
  if(verifier == 0){
    printf("Value not found. Impossible removal.");
  }
}

// ///////////////////////////////////////////////////////////////
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

// ///////////////////////////////////////////////////////////////
void searchValue(BinaryTree *root, int value) {
  int level = 1, verifier = 0;
  if(root == NULL) {
    printf("Value is not present in tree!\n");
  }
  BinaryTree *current = root;
  BinaryTree *previous = NULL;

  while(current != NULL ) {
    if(value == current->info) {
      printf("Value found: %d\n", value); // se existe na arvore
      printf("Node level: %d\n", level);
      if(previous != NULL) {
        printf("Father: %d\n", previous->info);
        if(previous->right != NULL) {
          if(previous->right->info != current->info) {
            printf("Sibling: %d\n", previous->right->info);
          }
        }
        else if(previous->left != NULL) {
          if (previous->left->info != current->info && previous->left != NULL) {
            printf("Sibling: %d\n", previous->left->info);
          }
        } else {
          printf("It is a leaf element. Does not have siblings.");
        }
      }
      verifier = 1;
    }
    previous = current;
    level++;
    if(value > current->info) {
      current = current->right;
    } else {
      current = current->left;
    }
  }
  if(verifier == 0) {
    printf("Value is not present in tree!\n");
  }
}

int verifyBalancedTree(BinaryTree *root) {

    if((root != NULL && root->left == NULL && root->right == NULL) || root == NULL) {
        return 1;
    }

    int hLeftTree = getHeight(root->left);
    int hRightTree = getHeight(root->right);

    if (abs(hRightTree - hLeftTree) > 1) {
        return 0;
    }

    return verifyBalancedTree(root->right) && verifyBalancedTree(root->left);
}

int saveTree(BinaryTree *root, int is_left, int offset, int depth, char **s) {
    char b[20];
    int width = 5;

    if (!root) return 0;

    sprintf(b, " %3d ", root->info);

    int left  = saveTree(root->left,  1, offset, depth + 1, s);
    int right = saveTree(root->right, 0, offset + left + width, depth + 1, s);

    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '*';

        s[2 * depth - 1][offset + left + width/2] = '/';
        s[2 * depth - 1][offset + left + width + right + width/2] = ':';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '*';

        s[2 * depth - 1][offset + left + width/2] = 92;
        s[2 * depth - 1][offset - width/2 - 1] = ':';
    }

    return left + width + right;
}

void showTree(BinaryTree *root) {
  int height = getHeight(root);
  int tam = ((height * 2) + 1);

  char **show = (char **)malloc( tam * sizeof(char *));
	for (int i = 0; i < tam; i++) {
		show[i] = (char *)malloc(255 * sizeof(char));
		sprintf(show[i], "%80s", " ");
	}

	saveTree(root, 0, 0, 0, show);

	for (int i = 0; i < tam; i++) {
    int tamStr = strlen(show[i]);
    for(int j = 0; j < tamStr; j++){
        printf("%c", show[i][j]);
    }
    printf("\n");
  }


    for (int i = 0; i < tam; i++)
        free(show[i]);
    free(show);
}

int getHeightAVL(struct treeAVL *root) {
  if(root == NULL) {
    return 0;
  }
  int leftHeight = getHeightAVL(root->left); // endereco do no da esquerda
  int rightHeight = getHeightAVL(root->right); // endereco do no da direita
  if(leftHeight > rightHeight) {
    return (leftHeight + 1);
  } else {
    return (rightHeight + 1);
  }
}

int saveTreeAVL(struct treeAVL *root, int is_left, int offset, int depth, char **s) {
    char b[20];
    int width = 5;

    if (!root) return 0;

    sprintf(b, " %3d ", root->key);

    int left  = saveTreeAVL(root->left,  1, offset, depth + 1, s);
    int right = saveTreeAVL(root->right, 0, offset + left + width, depth + 1, s);

    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '*';

        s[2 * depth - 1][offset + left + width/2] = '/';
        s[2 * depth - 1][offset + left + width + right + width/2] = ':';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '*';

        s[2 * depth - 1][offset + left + width/2] = 92;
        s[2 * depth - 1][offset - width/2 - 1] = ':';
    }

    return left + width + right;
}

void showTreeAVL(struct treeAVL *root) {
  int height = getHeightAVL(root);
  int tam = ((height * 2) + 1);

  char **show = (char **)malloc( tam * sizeof(char *));
	for (int i = 0; i < tam; i++) {
		show[i] = (char *)malloc(255 * sizeof(char));
		sprintf(show[i], "%80s", " ");
	}

	saveTreeAVL(root, 0, 0, 0, show);

	for (int i = 0; i < tam; i++) {
    int tamStr = strlen(show[i]);
    for(int j = 0; j < tamStr; j++){
        printf("%c", show[i][j]);
    }
    printf("\n");
  }


    for (int i = 0; i < tam; i++)
        free(show[i]);
    free(show);
}

void freeTreeAVL(struct treeAVL *node) {
  if(node == NULL) { // verifica se a alocacao deu certo
    return;
  }
  freeTreeAVL(node->left); // vai a esquerda e libera
  freeTreeAVL(node->right); // vai a direita e libera
  free(node); // libera o node apos liberar tudo a direita e a esquerda
  node = NULL;
}

int height(struct treeAVL *N) {
  if (N == NULL) {
    return 0;
  }
  return N->height;
}

int max(int a, int b) {
  return (a > b)? a : b;
}

struct treeAVL *newNode(int key) {
  struct treeAVL *node = (struct treeAVL*) malloc(sizeof(struct treeAVL));
  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  return(node);
}
struct treeAVL *rightRotate(struct treeAVL *y) {
  struct treeAVL *x = y->left;
  struct treeAVL *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right))+1;
  x->height = max(height(x->left), height(x->right))+1;

  return x;
}

struct treeAVL *leftRotate(struct treeAVL *x) {
  struct treeAVL *y = x->right;
  struct treeAVL *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right))+1;
  y->height = max(height(y->left), height(y->right))+1;

  return y;
}

int getBalance(struct treeAVL *N) {
  if (N == NULL) {
    return 0;
  }
  return height(N->left) - height(N->right);
}

struct treeAVL *insertTreeAVL(struct treeAVL* node, int key) {
  if (node == NULL) {
    return(newNode(key));
  }
  if (key < node->key) {
    node->left  = insertTreeAVL(node->left, key);
  }
  else if (key > node->key) {
    node->right = insertTreeAVL(node->right, key);
  }
  else {
    return node;
  }

  node->height = 1 + max(height(node->left), height(node->right));

  int balance = getBalance(node);

  if (balance > 1 && key < node->left->key) {
    return rightRotate(node);
  }
  if (balance < -1 && key > node->right->key) {
    return leftRotate(node);
  }
  if (balance > 1 && key > node->left->key) {
    node->left =  leftRotate(node->left);
    return rightRotate(node);
  }

  if (balance < -1 && key < node->right->key) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

struct treeAVL *createTreeAVL() {
  struct treeAVL *root;
  root = (struct treeAVL *) malloc(sizeof(struct treeAVL));

  if(root != NULL) {
    root = NULL;
  }
  return root;
}

struct treeAVL *loadTreeAVL(char *nameOfFile) {
  FILE *fp;
  int number;

  struct treeAVL *root = createTreeAVL();

  fp = fopen(nameOfFile, "r");

  if (fp == NULL) {
    printf("Was not found!\n");
  }

  while(!feof(fp)) {
    fscanf(fp, "%d", &number);
    root = insertTreeAVL(root, number);
  }

  fclose(fp);

  return root;
}
