#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char name[21];
    char phone[11];
    struct node *left, *right;
    int height;
} node_t;

int print_count = 0, search_count = 0;

node_t *newNode(char name[], char phone[])
{
    node_t *new_Node = malloc(sizeof(node_t));
    strcpy(new_Node->name, name);
    strcpy(new_Node->phone, phone);
    new_Node->left = NULL;
    new_Node->right = NULL;
    new_Node->height = 1;
    return new_Node;
}

int nodeHeight(int height_a, int height_b)  //計算node的高度(比較左右subtree)
{
    if (height_a > height_b)
        return height_a;
    else
        return height_b;
}

int height(node_t *root)    //判斷高度
{
    if (root == NULL)       //一定要有這行，不然會segmentation fault
        return 0;
    return root->height;
}

node_t *rightRotate(node_t *root)       //右旋
{
    node_t *l_sub = root->left;     //左子樹
    node_t *lr_sub = l_sub->right;      //左子樹的右子樹
    root->left = lr_sub;        //移動樹(旋轉)
    l_sub->right = root;
    root->height = nodeHeight(height(root->left), height(root->right)) + 1;
    l_sub->height = nodeHeight(height(l_sub->left), height(l_sub->right)) + 1;      //重新計算樹高
    return l_sub;
}

node_t *leftRotate(node_t *root)        //左旋
{
    node_t *r_sub = root->right;        //右子樹
    node_t *rl_sub = r_sub->left;       //右子樹的左子樹
    root->right = rl_sub;       //移動樹(旋轉)
    r_sub->left = root;
    root->height = nodeHeight(height(root->left), height(root->right)) + 1;
    r_sub->height = nodeHeight(height(r_sub->left), height(r_sub->right)) + 1;      //重新計算樹高
    return r_sub;
}

int getBF(node_t *root)     //取得balance factor
{
    int bf;
    if (root == NULL)
        return 0;
    bf = height(root->left) - height(root->right);      //left_tree_height - right_tree_height
    return bf;
}

node_t *insert_node(node_t *node, char name[], char phone[])
{
    int balanceFactor = 0;
    if (node == NULL)       //node是空的 ==> 新增node
        return newNode(name, phone);
    
    if (strcmp(name, node->name) < 0)       //name 比 node 小 ==> 往左子樹
        node->left = insert_node(node->left, name, phone);
    else if (strcmp(name, node->name) > 0)      //name 比 node 大 ==> 往右子樹
        node->right = insert_node(node->right, name, phone);
    else if (strcmp(name, node->name) == 0)     //name == node ==> 不存在，AVL 是 BST
        return node;

    node->height = nodeHeight(height(node->left), height(node->right)) + 1;     //重算樹高
    balanceFactor = getBF(node);

    if (balanceFactor > 1 && strcmp(name, node->left->name) < 0)        //LL
        return rightRotate(node);
    else if (balanceFactor < -1 && strcmp(name, node->right->name) > 0)     //RR
        return leftRotate(node);
    else if (balanceFactor > 1 && strcmp(name, node->left->name) > 0) {     //LR
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    else if (balanceFactor < -1 && strcmp(name, node->right->name) < 0) {   //RL
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;    //沒做任何改變的node
}

void preorder(node_t *node)     //preorder 印出
{
    if (node != NULL) {
        if (print_count == 0)
                printf("%s", node->name);
        else
            printf(" %s", node->name);
        print_count++;

        preorder(node->left);
        preorder(node->right);
    }
}

void search(node_t *node, char name[])
{
    if (node == NULL) {
        if (search_count == 0)
            printf("%s null", name);
        else
            printf("\n%s null", name);
        search_count++;
        return;
    }
    if (strcmp(node->name, name) == 0) {
        if (search_count == 0)
            printf("%s %s", node->name, node->phone);
        else
            printf("\n%s %s", node->name, node->phone);
        search_count++;
    }
    else if (strcmp(node->name, name) < 0)
        search(node->right, name);
    else if (strcmp(node->name, name) > 0)
        search(node->left, name);
}

int main()
{
    char operation[2];
    char name[21];
    char phone[11];
    node_t *root = NULL;
    scanf("%s", operation);     //scanf "D"
    strcpy(operation, "S");
    while(scanf("%s", name) != EOF) {
        if (strcmp(name, operation) == 0)
            break;
        scanf("%s", phone);
        root = insert_node(root, name, phone);
        strcpy(name, "\0");
        strcpy(phone, "\0");
    }
    preorder(root);
    printf("\n");
    strcpy(operation, "E");
    while (scanf("%s", name) != EOF) {
        if (strcmp(name, operation) == 0)
            break;
        search(root, name);
        strcpy(name, "\0");
    }
    return 0;
}
