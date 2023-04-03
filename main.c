#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char data[32];
    char color;
    struct Node* right;
    struct Node* left;
    struct Node* parent;
} Node;

typedef struct
{
    struct Node* root;
    int size;
} RedBlackTree;

Node *initializeNode(char *x){
    Node *newNode=malloc(sizeof(Node));
    strcpy(newNode->data,x);
    newNode->color = 'R';
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

RedBlackTree *initializeRBT(){
    RedBlackTree *rbt = malloc(sizeof(RedBlackTree));
    rbt->root=NULL;
    rbt->size = 0;
    return rbt;
}

int position(Node *node) {
    if(node->parent) {
        if (strcasecmp(node->data,node->parent->data) < 0)
            return -1;
        else
            return 1;
    }
    return 0;
}

void rotateLeft (Node *node) {
    node->right->parent = node->parent;
    if (node->parent) {
        if (position(node) == -1)
            node->parent->left = node->right;
        else
            node->parent->right = node->right;
    }
    node->parent = node->right;
    node->right = node->parent->left;
    node->parent->left = node;
    if (node->right)
        node->right->parent = node;
}

void rotateRight (Node *node) {
    node->left->parent = node->parent;
    if (node->parent) {
        if (position(node) == -1)
            node->parent->left = node->left;
        else
            node->parent->right = node->left;
    }
    node->parent = node->left;
    node->left = node->parent->right;
    node->parent->right = node;
    if (node->left)
        node->left->parent = node;
}

Node * getUncle(Node *node) {
    Node * uncle = NULL;
    if(node->parent && node->parent->parent) {
        if (position(node->parent) == -1)
            uncle = node->parent->parent->right;
        else
            uncle = node->parent->parent->left;
    }
    return uncle;
}

//void changeColour (Node *node) {
//    if((node)->color == 'R')
//        (node)->color = 'B';
//    else
//        (node)->color = 'R';
//}

//void fix1 (Node *node) {
//    if((node)->parent) {
//        Node * uncle = getUncle((node));
//        if (uncle && uncle->color == 'R') {
//            changeColour((node)->parent);
//            changeColour((node)->parent->parent);
//            changeColour(uncle);
//            fix1((node)->parent->parent);
//        } else {
//            if (position((node)->parent) != position((node))) {
//                if (position((node)) == 1) {
//                    rotateLeft((node)->parent);
//                    //fix(node->left);
////                    node = node->left;
////                    goto lineCaseL;
//                }
//                else {
//                    rotateRight((node)->parent);
//                    //fix(node->right);
////                    node = node->right;
////                    goto lineCaseR;
//
//                }
//            } else {
//                if (position((node)) == 1) {
//                    lineCaseR:rotateLeft((node)->parent->parent);
//                    changeColour((node)->parent->left);
//                }
//                else {
//                    lineCaseL: rotateRight((node)->parent->parent);
//                    changeColour((node)->parent->right);
//                }
//                changeColour((node)->parent);
//            }
//        }
//    }
//    else
//        (node)->color = 'B';
//}
//
//void fix2(Node *node) {
//    Node *uncle;
//    Node * temp;
//    if (node->parent) {
//        while (1) {
//            printf("%s Entered Loop\n",node->data);
//            printf("%s Parent = %s\n",node->data,node->parent->data);
//            printf("%s Grand Parent = %s\n",node->data,node->parent->parent->data);
//            uncle = getUncle(node);
//            if (uncle && uncle->color == 'R') {
//                printf("RED UNCLE\n");
//                changeColour(uncle);
//                changeColour(node->parent);
//                changeColour(node->parent->parent);
//                node = node->parent->parent;
//                continue;
//            } else {
//                printf("BLACK UNCLE\n");
//                if (position(node->parent) == position(node)) {
//                    changeColour(node->parent);
//                    changeColour(node->parent->parent);
//                    if (position(node) == -1)
//                        rotateRight(node->parent->parent);
//                    else
//                        rotateLeft(node->parent->parent);
//                } else {
//                    temp = node->parent;
//                    if (position(node) == -1)
//                        rotateRight(node->parent);
//                    else {
//                    rotateLeft(node->parent);
//                    }
//                    node = temp;
//                    continue;
//                }
//            }
//            break;
//        }
//    } else
//        node->color = 'B';
//}
void printNodeDetails(Node node);
void fix3(RedBlackTree *RBT,Node *node) {
    fix_rec:
    if (node->parent) {
        if (node->parent->color == 'R' && node->parent->parent) {
            Node * uncle = getUncle(node);
            if (uncle && uncle->color == 'R') {
                uncle->color = 'B';
                node->parent->color = 'B';
                node->parent->parent->color = 'R';
                node = node->parent->parent;
                goto fix_rec;
            } else {
                if (position(node) == position(node->parent)) { //LINE CASE
                    node->parent->color = 'B';
                    node->parent->parent->color = 'R';
                    if (position(node) == -1)
                        rotateRight(node->parent->parent);
                    else
                        rotateLeft(node->parent->parent);
                    if (!node->parent->parent) {
                        RBT->root = node->parent;
                    }
                } else { //TRIANGLE CASE
                    if (position(node) == -1) {
                        rotateRight(node->parent);
                        node = node->right;
                    }
                    else {
                        rotateLeft(node->parent);
                        node = node->left;
                    }
                    node->parent->color = 'B';
                    node->parent->parent->color = 'R';
                    if (position(node) == -1)
                        rotateRight(node->parent->parent);
                    else
                        rotateLeft(node->parent->parent);
                    if (!node->parent->parent) {
                        RBT->root = node->parent;
                    }
                }
            }
        }
    } else {
        node->color = 'B';
        RBT->root = node;
    }
}

//LAST YEAR BST
void plant(RedBlackTree * tree, Node * * root,char * word,Node * parent)
{
    if (!(*root))
    {
        *root = initializeNode(word);
        (*root)->parent = parent;
        fix3(tree,*root);
    } else
    {
        if (strcasecmp(word,(*root)->data)<0)
        {
            plant(tree,&((*root)->left),word,*root);
        } else
        {
            plant(tree,&((*root)->right),word,*root);
        }
    }
}

void insert(RedBlackTree * tree,char * word)
{
    plant(tree,&(tree->root),word,NULL);
    tree->size++;
}

//void rightrotate(Node * root,Node * temp)
//{
//    Node * left = temp->left;
//    temp->left = left->right;
//    if (temp->left)
//        temp->left->parent = temp;
//    left->parent = temp->parent;
//    if (!temp->parent)
//        root = left;
//    else if (temp == temp->parent->left)
//        temp->parent->left = left;
//    else
//        temp->parent->right = left;
//    left->right = temp;
//    temp->parent = left;
//}
//void leftrotate(Node * root,Node * temp)
//{
//    Node * right = temp->right;
//    temp->right = right->left;
//    if (temp->right)
//        temp->right->parent = temp;
//    right->parent = temp->parent;
//    if (!temp->parent)
//        root = right;
//    else if (temp == temp->parent->left)
//        temp->parent->left = right;
//    else
//        temp->parent->right = right;
//    right->left = temp;
//    temp->parent = right;
//}
//void fix4(Node * root,Node *pt) {
//    Node * parent_pt = NULL;
//    Node * grand_parent_pt = NULL;
//
//    while ((pt != root) && (pt->color != 0)
//           && (pt->parent->color == 1))
//    {
//        parent_pt = pt->parent;
//        grand_parent_pt = pt->parent->parent;
//
//        /*  Case : A
//             Parent of pt is left child
//             of Grand-parent of
//           pt */
//        if (parent_pt == grand_parent_pt->left)
//        {
//
//            Node * uncle_pt = grand_parent_pt->right;
//
//            /* Case : 1
//                The uncle of pt is also red
//                Only Recoloring required */
//            if (uncle_pt != NULL && uncle_pt->color == 1)
//            {
//                grand_parent_pt->color = 1;
//                parent_pt->color = 0;
//                uncle_pt->color = 0;
//                pt = grand_parent_pt;
//            }
//
//            else {
//
//                /* Case : 2
//                     pt is right child of its parent
//                     Left-rotation required */
//                if (pt == parent_pt->right) {
//                    leftrotate(root,parent_pt);
//                    pt = parent_pt;
//                    parent_pt = pt->parent;
//                }
//
//                /* Case : 3
//                     pt is left child of its parent
//                     Right-rotation required */
//                rightrotate(root,grand_parent_pt);
//                int t = parent_pt->color;
//                parent_pt->color = grand_parent_pt->color;
//                grand_parent_pt->color = t;
//                pt = parent_pt;
//            }
//        }
//
//            /* Case : B
//                 Parent of pt is right
//                 child of Grand-parent of
//               pt */
//        else {
//            Node * uncle_pt = grand_parent_pt->left;
//
//            /*  Case : 1
//                The uncle of pt is also red
//                Only Recoloring required */
//            if ((uncle_pt != NULL) && (uncle_pt->color == 1))
//            {
//                grand_parent_pt->color = 1;
//                parent_pt->color = 0;
//                uncle_pt->color = 0;
//                pt = grand_parent_pt;
//            }
//            else {
//                /* Case : 2
//                   pt is left child of its parent
//                   Right-rotation required */
//                if (pt == parent_pt->left) {
//                    rightrotate(root,parent_pt);
//                    pt = parent_pt;
//                    parent_pt = pt->parent;
//                }
//
//                /* Case : 3
//                     pt is right child of its parent
//                     Left-rotation required */
//                leftrotate(root,grand_parent_pt);
//                int t = parent_pt->color;
//                parent_pt->color = grand_parent_pt->color;
//                grand_parent_pt->color = t;
//                pt = parent_pt;
//            }
//        }
//    }
//
//    root->color = 0;
//}

//void LeftLeft (Node * node) {
//    Node * parent = node->parent;
//    Node * grandParent = parent->parent;
//    parent->color = 'B';
//    grandParent->color = 'R';
//    if(parent->right) {
//        grandParent->left = parent->right;
//        grandParent->left->parent = grandParent;
//    } else
//        grandParent->left = NULL;
//
//    if (!grandParent->parent) {
//        this.root = p;
//    } else {
//        if (g == g.parent.left) {
//            g.parent.left = p;
//        } else {
//            g.parent.right = p;
//        }
//
//        p.parent = g.parent;
//    }
//
//    p.right = g;
//    g.parent = p;
//}

//void insert(RedBlackTree *rbt, char *word){
//    Node *newNode=initializeNode(word);
//    Node *current=rbt->root;
//    if(!rbt->root)
//        rbt->root=newNode;
//    else {
//        while (1) {
//            if (strcasecmp(current->data, newNode->data) < 0) {
//                if (current->right) {
//                    current = current->right;
//                } else {
//                    current->right = newNode;
//                    newNode->parent = current;
//                    rbt->size++;
//                    break;
//                }
//            } else if (strcasecmp(current->data, newNode->data) > 0) {
//                if (current->left) {
//                    current = current->left;
//                } else {
//                    current->left = newNode;
//                    newNode->parent = current;
//                    rbt->size++;
//                    break;
//                }
//            } else break;
//        }
//    }
//    fix3(rbt,newNode);
//}

int getHeight(Node* root)
{
    if (root==NULL)
        return -1;
    else
    {
        int a = getHeight(root->left);
        int b = getHeight(root->right);

        if (a > b)
            return(a+1);
        else return(b+1);
    }
}

int Search(Node *root, char *data) {
    Node *current = root;
    while (current != NULL) {
        if (strcmp(current->data, data) == 0) {
            return 1;
        } else if (strcmp(current->data, data) > 0) {
            if (current->left == NULL) break;
            else current = current->left;
        } else {
            if (current->right == NULL) break;
            else current = current->right;
        }
    }
    return 0;
}

void loadDictionary(RedBlackTree *tree) {
    char word[32];
    char filename[32];
    printf("Enter Reference Dictionary: (ex: Dictionary.txt)\n");
    fgets(filename,32,stdin);
    filename[strlen(filename)-1]='\0';
    FILE * file = fopen(filename,"r");
    if (file)
    {
        printf("Loading Dictionary...\n");
        while(fgets(word, 32, file) != NULL)
        {
            word[strlen(word)-1]='\0';
            insert(tree,word);
        }
        printf("> Dictionary Loaded Successfully!\n");
    } else
    {
        printf("> File Not Found\n");
        loadDictionary(tree);
    }
}

void printDetails(RedBlackTree tree)
{
    printf("Data Structure:\tBinary Search Tree\n");
    printf("Size of Tree:\t%d\n",tree.size);
    printf("Height of Tree:\t%d\n", getHeight(tree.root));
}

void printNodeDetails(Node node) {
    printf("Data: %s\n",node.data);
    printf("Colour: %c\n",node.color);
    if(node.parent) {
        printf("PARENT\n");
        printf("Data: %s\n",node.parent->data);
        printf("Colour: %c\n",node.parent->color);
        if (node.parent->parent) {
            printf("GRAND PARENT\n");
            printf("Data: %s\n",node.parent->parent->data);
            printf("Colour: %c\n",node.parent->parent->color);
        }
    }
    if(node.left) {
        printf("LEFT\n");
        printf("Data: %s\n",node.left->data);
        printf("Colour: %c\n",node.left->color);
    }
    if(node.right) {
        printf("RIGHT\n");
        printf("Data: %s\n",node.right->data);
        printf("Colour: %c\n",node.right->color);
    }
}

int main() {

    RedBlackTree * Dictionary = initializeRBT();
    printf("~~~~~~~~~~~~~~~ Sentence Checker ~~~~~~~~~~~~~~~\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~~~~~~~~~~~~~~ Loading Dictionary ~~~~~~~~~~~~~~\n");
    loadDictionary(Dictionary);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~~~~~~~~~~~~~~ Dictionary Details ~~~~~~~~~~~~~~\n");
    printDetails(*Dictionary);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//    insert(Dictionary,"Monaco");
//    printNodeDetails(*Dictionary->root);
//    printf("------------------------------------\n");
//    insert(Dictionary,"Catalonia");
//    printNodeDetails(*Dictionary->root);
//    printf("....................................\n");
//    printNodeDetails(*Dictionary->root->left);
//    printf("------------------------------------\n");
//    insert(Dictionary,"fetish's");
//    printNodeDetails(*Dictionary->root->right);
//    insert(Dictionary,"Karaganda");
//    printNodeDetails(*Dictionary->root);
//    printNodeDetails(*Dictionary->root->left);
//    printNodeDetails(*Dictionary->root->right);
//    insert(Dictionary,"Niagara");
//    printNodeDetails(*Dictionary->root);
//    printNodeDetails(*Dictionary->root->left);
//    printNodeDetails(*Dictionary->root->right);
//    insert(Dictionary,"Claudia's");
//    insert(Dictionary,"embroider");


    return 0;
}

////LAST YEAR BST
//void plant(Node * * root,char * word,Node * parent)
//{
//    if (!(*root))
//    {
//        *root = initializeNode(word);
//
//        (*root)->parent = parent;
//        printf("%s\n",(*root)->parent->data);
//        fix(root);
//    } else
//    {
//        if (strcasecmp(word,(*root)->data)<=0)
//        {
//            plant(&((*root)->left),word,(*root));
//        } else
//        {
//            plant(&((*root)->right),word,(*root));
//        }
//    }
//}
//
//void insert(RedBlackTree * tree,char * word)
//{
//    plant(&(tree->root),word,NULL);
//    tree->size++;
//}
//
//int maximum(int a,int b)
//{
//    if (a>b)
//        return a;
//    else
//        return b;
//}
//
//int paths(Node * root) {
//    if (root->left==NULL && root->right==NULL)
//    {
//        return 0;
//    }else if (root->left!=NULL && root->right!=NULL)
//    {
//        return maximum(paths(root->left),paths(root->right))+1;
//    } else if  (root->right != NULL)
//    {
//        return paths(root->right) + 1;
//    } else
//    {
//        return paths(root->left) + 1;
//    }
//}
//
//int height(RedBlackTree tree)
//{
//    return paths(tree.root);
//}