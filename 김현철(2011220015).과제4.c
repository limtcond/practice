/*
2011220015 디자인조형학부  김현철
Delete가 구현이 되지 않아서 어려움을 겪어 늦게 제출합니다.
여전히 되지 않습니다.

 - Null node를 가리키는 문제
 - 함수 안에서 포인터가 가리키는 값을 수정하지 못하는 문제

위 두 문제를 해결하지 못하고 있습니다.
Insert까지는 지난 과제에서 제출하였듯이 구현하였습니다.
다음 과제에서 해결하여 같이 제출하도록 하겠습니다.
*/


#include <stdio.h>
#include <stdlib.h>

// Tree의 Node들을 먼저 정의합니다.
typedef struct Node* NodePtr;
struct Node{
    int val;
    int color; //RBtree
    NodePtr left, right, parent;
};

NodePtr node_alloc(int newval) {
    NodePtr self = (NodePtr)malloc(sizeof(struct Node));
    self->val = newval;
    self->left = NULL;
    self->right = NULL;
    self->parent = NULL;
    self->color = 1; //RBtree 1:Red, 0:Black
    return self;
}

// Tree를 Root를 정하여 구성합니다.
typedef struct RB* RBPtr;
struct RB{
    NodePtr root;
};


RBPtr rb_alloc() {
    RBPtr self = (RBPtr)malloc(sizeof(struct RB));
    self->root = NULL;
    return self;
}




void left_rotate(RBPtr self, NodePtr tree, NodePtr x){
    NodePtr y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        (y->left)->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        self->root = y;
    else if (x == (x->parent)->left)
        (x->parent)->left = y;
    else (x->parent)->right = y;
    y->left = x;
    x->parent = y;
}

void right_rotate(RBPtr self, NodePtr tree, NodePtr x){
    NodePtr y = x->left;
    x->left = y->right;
    if (y->right != NULL)
        (y->right)->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        self->root = y;
    else if (x == (x->parent)->right)
        (x->parent)->right = y;
    else (x->parent)->left = y;
    y->right = x;
    x->parent = y;
}


//RB TREE FIXUP을 구성합니다.
//NodePtr가 NULL인데 color값을 비교하는 경우에 에러가 생겨서
//NULL인 경우 비교를 수행하지 못하도록 앞에 ~ != NULL &&를 추가하였습니다.



void rb_tree_fixup(RBPtr self, NodePtr tree, NodePtr new) {
    
    NodePtr y;
    
    while (new->parent != NULL && (new->parent)->color == 1){
        if ((new->parent) == self->root){
        }
        else if ((new->parent)->parent != NULL && (new->parent)==(((new->parent)->parent)->left)){
            y = ((new->parent)->parent)->right;
            if(y->color == 1){
                (new->parent)->color = 0;
                y->color = 0;
                ((new->parent)->parent)->color = 1;
                new = ((new->parent)->parent);
            }
            
            else if (new == (new->parent)->right){
                new = new ->parent;
                left_rotate(self, tree, new);
            }
            (new->parent)->color = 0;
            ((new->parent)->parent)->color = 1;
            right_rotate(self, tree, (new->parent)->parent);
        }
        else if((new->parent)->parent != NULL && (new->parent)==(((new->parent)->parent)->right)){
            y = ((new->parent)->parent)->left;
            if(y->color == 1){
                (new->parent)->color = 0;
                y->color = 0;
                ((new->parent)->parent)->color = 1;
                new = ((new->parent)->parent);
            }
            
            else if (new == (new->parent)->left){
                new = new ->parent;
                right_rotate(self, tree, new);
            }
            (new->parent)->color = 0;
            ((new->parent)->parent)->color = 1;
            left_rotate(self, tree, (new->parent)->parent);
        }
    }
    (self->root)->color = 0;
}





// RB Tree의 INSERT들을 정의합니다.
void rb_tree_insert(RBPtr self, NodePtr tree, NodePtr new) {
    NodePtr y = NULL; //
    NodePtr x = self->root;
    
    while ( x != NULL){
        y = x;
        if (new->val < x->val)
            x = x->left;
        else x = x->right;
    }
    new->parent = y;
    
    if (y == NULL){
        self->root = new;
        new->color = 0;} //RBtree:Black
    else if (new->val < y->val)
        y->left = new;
    else y->right = new;
    
    new->left = NULL;
    new->right = NULL;
    new->color = 1;
    
    rb_tree_fixup(self, tree, new);
}



void rb_count(RBPtr self, NodePtr tree, int level, int*all, int*black) {
    if (tree->right != NULL)//오른쪽으로
        rb_count(self, tree->right, level + 1, all, black);
    for(int i=0; i<level; i++)
        printf("    ");
    
    printf("%d", tree->val);
    (*all) += 1; // 모든 노드의 갯수를 파악한다.
    if(tree->color == 0){
        printf("(black)\n");
        (*black) += 1; // 검정 노드의 갯수를 파악한다.
    } else printf("(red)\n");
    
    // -> 왼쪽으로
    if (tree->left != NULL){
        rb_count(self, tree->left, level + 1, all, black);
    }
    
}

void rb_bh_count(RBPtr self, NodePtr tree, int level, int* bh) {
    if (tree->right != NULL)//오른쪽으로
        rb_bh_count(self, tree->right, level + 1, bh);
    printf("%d", tree->val);
    if(tree->color == 0)
        (*bh) += 1; // 검정 노드의 갯수를 파악한다.
}

void rb_inorder(RBPtr self, NodePtr tree) {
    if (tree == NULL)
        return;
    else {
        rb_inorder(self, tree->left);
        printf("%d ", tree->val);
        rb_inorder(self, tree->right);
    }
}

void rb_transplant(RBPtr self, NodePtr tree, NodePtr u, NodePtr v){
    if (v == NULL){
        u->parent = NULL;
    }else{
    if (u->parent == NULL){
        self -> root = v;
    }else if(u == (u->parent)->left){
        (u->parent)->left = v;
    }else{
        (u->parent)->right = v;
    }
    v->parent = u->parent;
    }
}

NodePtr tree_minimum(NodePtr x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}

void rb_delete_fixup(RBPtr self, NodePtr tree, NodePtr x){
    NodePtr w;
    while( x!=self->root && x->color == 0){
        if (x == (x->parent)->left){
            w = (x->parent)->right;
            if (w->color == 1){
                w->color = 0;
                (x->parent)->color = 1;
                left_rotate(self, tree, x->parent);
                w = (x->parent)->right;
            }
            if ((w->left)->color == 0 && (w->right)->color == 0){
                w->color = 1;
                x = x->parent;
            }else if((w->right)-> color == 0) {
                (w->left)-> color = 0;
                w->color = 1;
                right_rotate(self, tree, w);
                w = (w->parent)->right;
            }
            w->color = (x->parent)->color;
            (x->parent)->color = 0;
            (w->right)->color = 0;
            left_rotate(self, tree, x->parent);
            x = self->root;
        }else{
            w = (x->parent)->left;
            if (w->color == 1){
                w->color = 0;
                (x->parent)->color = 1;
                right_rotate(self, tree, x->parent);
                w = (x->parent)->left;
            }
            if ((w->right)->color == 0 && (w->left)->color == 0){
                w->color = 1;
                x = x->parent;
            }else if((w->left)-> color == 0) {
                (w->right)-> color = 0;
                w->color = 1;
                left_rotate(self, tree, w);
                w = (w->parent)->left;
            }
            w->color = (x->parent)->color;
            (x->parent)->color = 0;
            (w->left)->color = 0;
            right_rotate(self, tree, x->parent);
            x = self->root;
        }
    }
    x->color = 0;
}


void rb_delete(RBPtr self, NodePtr tree, NodePtr z) {
    NodePtr x = NULL;
    NodePtr y;
    int ycolor;
    y = z;
    ycolor = y->color;
    

    if (z->left == NULL){
        x = z->right;
        rb_transplant(self, tree, z, z->right);
    }else if(z->right == NULL){
        x = z->left;
        rb_transplant(self, tree, z, z->left);
    }
    
    else{
        if(z->right != NULL)
            y = tree_minimum(z->right);
        ycolor = y->color;
        
        if(y->right != NULL)
            x = y->right;
        if (y->parent == z){
            x->parent = y;
        }else {
            if(y->right != NULL)
                rb_transplant(self, tree, y, y->right);
            y->right = z->right;
            if(y->right != NULL)
                (y->right)->parent = y;
        }
        rb_transplant(self, tree, z, y);
        y->left = z->left;
        if(y->left != NULL)
            (y->left)->parent = y;
        y->color = z->color;
    }
    if (ycolor == 0)
        rb_delete_fixup(self, tree, x);
}

void rb_find(RBPtr self, NodePtr tree, NodePtr delData, int data){
    if(tree->val == data)
        delData = tree;
    else
    if (tree->right != NULL)
        rb_find(self, tree->right, delData, data);
    if (tree->left != NULL)
        rb_find(self, tree->left, delData, data);
}



int main() {
    
    RBPtr rb = rb_alloc();
    /* all or black node의 수를 저장 */
    int allNodeNum = 0;
    int blackNodeNum = 0;
    int blackHeightNum = 0;
    int delNumber;
    
    int *all = &allNodeNum;
    int *black = &blackNodeNum;
    int *bh = &blackHeightNum;
    
    NodePtr del;
    
    
    int data;
    
    FILE *input = fopen("/Users/haechal/Desktop/ds4/ds4/input.txt", "r");
    FILE *list = fopen("/Users/haechal/Desktop/ds4/ds4/list.txt", "w");
    
    
    while(fscanf(input, "%d", &data) != EOF){
        printf("%d", data);
        if (data == 0)
            break;
        else if(data > 0){
            rb_tree_insert(rb, rb->root, node_alloc(data));
            printf("더한다\n");}
        else if (data < 0){
            del = NULL;
            rb_find(rb, rb->root, del, ((-1)*data));
            delNumber = data;
            //printf("%d", del->val);
            if(del == NULL){
                printf("%d", data);
                fprintf(list, "%d \n", data);
                printf("없다\n");
            }
        }
    }
    
    //count//
    rb_count(rb, rb->root, 0, all, black);
    rb_bh_count(rb, rb->root, 0, bh);
    printf("\n");
    
    //output//
    rb_inorder(rb, rb->root);
    printf("\ntotal : %d \n", allNodeNum);
    printf("nb : %d \n", blackNodeNum);
    printf("bh : %d \n", blackHeightNum);
    
    fclose(input);
    fclose(list);
    return 0;
}
