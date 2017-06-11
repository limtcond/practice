// 김현철 2011220015
// 디자인조형학부
// 과제 5: RED-BLACK TREE


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

#define RED 1
#define BLACK 0
#define NIL 0

// Tree의 Node들을 먼저 정의합니다.
typedef struct Node* NodePtr;
struct Node{
    int val;
    int color;
    NodePtr left, right, parent;
};

// Node는 기본적으로 BLACK으로 할당된다.
// NIL의 색이 모두 BLACK이어야하기 때문입니다.
NodePtr node_alloc(int newval) {
    NodePtr self = (NodePtr)malloc(sizeof(struct Node));
    self->val = newval;
    self->left = NULL;
    self->right = NULL;
    self->parent = NULL;
    self->color = BLACK; //RBtree 1:RED, 0:BLACK
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
    NodePtr y = NULL;
    while (new->parent != NULL && (new->parent)->color == RED){
        if ((new->parent)==(((new->parent)->parent)->left)){
            y = ((new->parent)->parent)->right;
            if(y != NULL && y->color == RED){
                (new->parent)->color = BLACK;
                y->color = BLACK;
                ((new->parent)->parent)->color = RED;
                new = ((new->parent)->parent);
            } else {
                if (new == (new->parent)->right){
                    new = new ->parent;
                    left_rotate(self, tree, new);
                }
                if(new->parent != NULL){
                    (new->parent)->color = BLACK;
                    if((new->parent)->parent != NULL){
                        ((new->parent)->parent)->color = RED;
                        right_rotate(self, tree, (new->parent)->parent);
                    }
                }
            }
        }
        else{
            y = ((new->parent)->parent)->left;
            if(y != NULL && y->color == RED){
                (new->parent)->color = BLACK;
                y->color = BLACK;
                ((new->parent)->parent)->color = RED;
                new = ((new->parent)->parent);
            } else {
                if (new == (new->parent)->left){
                    new = new ->parent;
                    right_rotate(self, tree, new);
                }
                if(new->parent != NULL){
                    (new->parent)->color = BLACK;
                    if((new->parent)->parent != NULL){
                        ((new->parent)->parent)->color = RED;
                        left_rotate(self, tree, (new->parent)->parent);
                    }
                }
            }
        }
    }
    (self->root)->color = BLACK;
}

// RB Tree의 INSERT들을 정의합니다.
void rb_tree_insert(RBPtr self, NodePtr tree, NodePtr new, NodePtr nil) {
    NodePtr y = NULL; //
    NodePtr x = self->root;


    while ( x != NULL){
        y = x;
        if (new->val < x->val)
            x = x->left;
        else x = x->right;

        if ((x->val) == NIL) // 말단에 오면 멈춥니다.
            break;
    }
    new->parent = y;

    if (y == NULL){
        self->root = new;
        new->color = BLACK;} //RBtree:Black

    else if (new->val < y->val)
        y->left = new;
    else y->right = new;

    //말단의 BLACK node들을 연결해줍니다.
    new->left = nil;
    new->right = nil;
    new->color = RED;

    rb_tree_fixup(self, tree, new);
}

//트리 모양 출력을 위한 함수입니다.
void rb_print(RBPtr self, NodePtr tree, int level) {
    if ((tree->right)->val != NIL)
        rb_print(self, tree->right, level + 1);
    for(int i=0; i<level; i++)
        printf("   ");
    if(tree->val != NIL){
        printf("%d", tree->val);
        if(tree->color == BLACK)
            printf("B\n");
        else printf("R\n");}
    if ((tree->left)->val != NIL)
        rb_print(self, tree->left, level+1);
}

//모든 노드, 검정 노드를 셈하기 위한 함수입니다.
void rb_count(RBPtr self, NodePtr tree, int level, int*all, int*black) {
    if ((tree->right)->val != NIL)
        rb_count(self, tree->right, level + 1, all, black);
    for(int i=0; i<level; i++)
        //printf("    "); //(확인)

        if(tree->val != NIL){
            //printf("%d", tree->val); // (확인)
            (*all) += 1; // 모든 노드의 갯수를 파악한다.
            if(tree->color == BLACK){
                //printf("B\n"); // (확인)
                (*black) += 1; // 검정 노드의 갯수를 파악한다.
            }//else printf("R\n"); // (확인)
        }

    if ((tree->left)->val != NIL)
        rb_count(self, tree->left, level + 1, all, black);

}

void rb_bh_count(RBPtr self, NodePtr tree, int level, int* bh) {
    if ((tree->right)->val != 0)//오른쪽으로
        rb_bh_count(self, tree->right, level + 1, bh);
    // printf("%d  ", tree->val); // 가장 오른쪽 말단으로 가는 경로(확인용)
    if(tree->color == 0)
        (*bh) += 1; // 검정 노드의 갯수를 파악한다.
}

void rb_inorder(RBPtr self, NodePtr tree) {
    if (tree->val == NIL)
        return;
    else {
        rb_inorder(self, tree->left);
        printf("%d ", tree->val);
        if (tree->color == BLACK) printf("B \n");
        else printf("R \n");
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
    while ((x->left)->val != 0){
        x = x->left;
    }
    return x;
}

void rb_delete_fixup(RBPtr self, NodePtr tree, NodePtr x){
    NodePtr w;
    while( x!=self->root && x->color == BLACK){
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
    NodePtr x;
    NodePtr y = z;
    int ycolor;
    ycolor = y->color;

    if ((z->left)->val == NIL){
        x = z->right;
        rb_transplant(self, tree, z, z->right);
    }else if((z->right)->val == NIL){
        x = z->left;
        rb_transplant(self, tree, z, z->left);
    }else{
        y = tree_minimum(z->right);
        ycolor = y->color;
        x = y->right;

        if (y->parent == z){
            x->parent = y;
        }else {
            rb_transplant(self, tree, y, y->right);
            y->right = z->right;
            (y->right)->parent = y;
        }

        rb_transplant(self, tree, z, y);
        y->left = z->left;
        (y->left)->parent = y;
        y->color = z->color;
    }
    if (ycolor == 0)
        rb_delete_fixup(self, tree, x);
}

//찾기 함수에서 바로 delete를 호출합니다.
//수를 찾지 못한 경우에는 0을 받환합니다.
int rb_find(RBPtr self, NodePtr tree, int data){
    if(tree->val == data){
        rb_delete(self, self->root, tree);
        //printf("--;\n");
        return 1;
    }
    if ((tree->right)->val != NIL)
        return rb_find(self, tree->right, data);
    if ((tree->left)->val != NIL)
        return rb_find(self, tree->left, data);
    return 0;
}


int main() {

    RBPtr rb = rb_alloc();
    NodePtr nil = node_alloc(NIL);
    int allNodeNum = 0;
    int blackNodeNum = 0;
    int blackHeightNum = 0;
    int missNum = 0;
    int insertNum = 0;
    int delNum = 0;

    int *all = &allNodeNum;
    int *black = &blackNodeNum;
    int *bh = &blackHeightNum;

    int data;
    void* name = NULL;

    FILE *input = fopen("input/input.txt", "r");
    FILE *list = fopen("output/list.txt", "w");
    DIR *dp;
    struct dirent *ep = NULL;
    dp = opendir ("input/");




    while(fscanf(input, "%d", &data) != EOF){
        //printf("%d", data);
        if (data == 0)
            break;
        else if(data > 0){
            rb_tree_insert(rb, rb->root, node_alloc(data),nil);
            insertNum++;
        }//printf("+\n");}
        else if (data < 0){
            if(rb_find(rb, rb->root, ((-1)*data)) == 0){
                //0이 반환되는 경우에 list.txt로 값이 출력됩니다.
                fprintf(list, "%d \n", data);
                missNum ++;
            }else
                delNum ++;

        }
    }


    //출력준비//
    rb_count(rb, rb->root, 0, all, black);
    rb_bh_count(rb, rb->root, 0, bh);
    printf("\n");
    {
        while ((ep = readdir (dp)))
            name =&(ep->d_name);
        (void) closedir (dp);
    };

    //출력//

    printf("filename = %s", name);
    printf("\n");
    printf("total = %d \n", allNodeNum);
    printf("insert = %d \n", insertNum);
    printf("deleted = %d \n", delNum);
    printf("miss = %d \n", missNum);
    printf("nb : %d \n", blackNodeNum);
    printf("bh : %d \n", blackHeightNum);

    printf("\n");
    rb_inorder(rb, rb->root);

    fclose(input);
    fclose(list);
    return 0;
}
