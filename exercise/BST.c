#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
    int data;
    struct _Node *left;
    struct _Node *right;
} Node;

// 탐색
Node* searchBST(Node *root, int x) {
    Node *p = root;     // 뉴 노드가 루트
    while (p != NULL) {
        if (p->data == x)
            return p;
        else if (p->data < x)
            p = p->right;
        else if (p->data > x)
            p = p->left;
    }
    return NULL;    // p가 NULL인 경우
}

// 삽입
Node* insertBST(Node *root, int x) {
    Node *p = root;
    Node *parent = NULL;        // p의 부모노드 (따라다닐 예정..)

    while (p!=NULL) {           // p가 null이 될 때까지 루프 반복
        parent = p;             // parent는 항상 현재노드 p의 부모노드를 가리킨다.
        if (p->data == x) {
            return p;
            printf("해당 노드가 이미 존재합니다.\n");
        }
        else if (p->data < x)
            p = p->right;
        else if (p->data > x)
            p = p->left;
    }
    
    // 삽입하기 위해 새 노드 할당
    Node *new_node = (Node*)malloc(sizeof(Node));
    // malloc 할당 실패했을 경우
    if (new_node == NULL) {
        fprintf(stderr, "할당 실패했습니다.\n");
        exit(1);
    }
    new_node -> data = x;
    new_node -> left = new_node -> right = NULL;
    
    // parent의 자식으로 새 노드 붙이기
    if (parent != NULL) {
        if (parent->data < new_node->data)
            parent->right = new_node;
        else if (parent->data > new_node->data)
            parent->left = new_node;
    }
    return new_node;
}

// 삭제
Node* deleteBST(Node *root, int x) {
    Node *p = root;
    Node *parent = NULL;

    // p가 null이 아니고, p가 찾는 값이 아닐 때 (탐색 과정)
    while ((p!=NULL) && (p->data!=x)) {
        parent = p;
        if (p->data < x)
            p = p->right;
        else if (p->data > x)
            p = p->left;        
    }

    // 끝까지 갔는데 삭제하려고 하는 노드가 없을 때 -> 처리할 일 X
    if (p==NULL) {
        printf("삭제하려고 하는 노드가 없습니다.\n");
        return root;
    }

    // 해당 노드의 차수가 0인 경우
    if ((p->left == NULL) && (p->right == NULL)) {
        // 현재 루트인데 차수가 0일 때
        if (parent == NULL)
            root = NULL;
        // 루트 아닌데 차수가 0인 경우
        else {
            if (parent->left == p)
                parent->left = NULL;
            else if (parent->right == p)
                parent->right = NULL;
        }
    }

    // 해당 노드의 차수가 1인 경우
    else if ((p->left==NULL) || (p->right == NULL)) {
        Node *child = (p->right == NULL) ? p->left : p->right;
        // 현재 루트인데 차수가 1일 때
        if (parent == NULL)
            root = child;   // 루트가 삭제되지만 그 루트노드가 자식 하나를 가지고 있기 때문에 그 자식이 새로운 루트노드가 됨
        // 루트 아니고 차수가 1인 경우
        else {
            if (parent->left == p)
                parent->left = child;
            else if (parent->right == p)
                parent->right = child;
        }
    }

    // 해당 노드의 차수가 2인 경우 - 위에서 다 걸러졌기 때문에 조건 줄 필요 X
    else {
        Node *heir_parent = p;  // 삭제 대상
        Node *heir = p->right;  // 오른쪽에서 후계자 뽑는 경우
        while (heir->left != NULL) {
            heir_parent = heir;
            heir = heir->left;
        }
        // 오른쪽중에서 가장 최솟값 찾았을 때 (heir->left == NULL)
        p->data = heir->data;   // p에 후계자 값 넣어주고 후계자를 삭제할거임
        if (heir_parent->left ==  heir)
            heir_parent->left = heir->right;
        else if (heir_parent->right == heir)
            heir_parent->right = heir->right;
        p = heir;   // p 포인터가 heir 포인터가 가리키는 메모리 주소를 가리키도록 만듦
    }
    free(p);
    return root;
}

// 중위 운행하는 함수
void inorder(Node* root) {
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

int main() {
    Node *root = insertBST(NULL, 10);
    insertBST(root, 6);
    insertBST(root, 4);
    insertBST(root, 8);
    insertBST(root, 12);
    insertBST(root, 15);
    insertBST(root, 14);
    insertBST(root, 20);
    inorder(root);
    printf("\n");

    root = deleteBST(root, 15);
    inorder(root);
    printf("\n");
    return 0;
}