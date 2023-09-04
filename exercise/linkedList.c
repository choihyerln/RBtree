#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;      // 다음 노드 가리킴
} Node_t;   // 구조체 Node의 멤버 next가 가리키는 데이터 타입

// 프린트
void linkPrint(Node_t **head) {
        Node_t *cur = *head;
        while (cur!=NULL){
            printf("%d\n",cur->data);
            cur = cur -> next;
        }
};

// 찾기
int find(Node_t **head, int F){
        Node_t *cur = *head;
        while (cur!=NULL){
            if (cur->data==F){
                return 1;
            }
            cur = cur -> next;
        }
        return 0;
};

// 업데이트
int update(Node_t **head, int F, int H){
        Node_t *cur = *head;

        while (cur!=NULL){
            if (cur->data==F){
                cur ->data = H;
                return 1;
            }
            cur = cur -> next;
        }
        return 0;
};

// // 삭제
int delete(Node_t **head, int D){
        Node_t *cur = *head;
        Node_t *pre;
        // 예외케이스
        // 1. 노드가 비어있을 때
        if (*head==NULL){
            printf("비어있음\n");
            return 0;
        } 
        // 2. 맨 처음꺼(head)가 D일 때
        if (cur->data==D){
            *head = cur->next;
            free(cur);                   // 메모리 반환(체크아웃): 메모리 누수방지
            return 1;
        }
        // 3. 그 외
        while (cur!=NULL){
            if (cur->data==D){
                pre->next = cur->next;
                free(cur);        
                return 1;
            }
            pre = cur;
            cur = cur -> next;
        }
        return 0;
};

// 넣기
// 더블포인터: 포인터의 주소값
void insert(Node_t **head, int N){
    // void* 를 반환한다 반드시 타입캐스팅을 하고 사용해야한다.
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));     // 구조체 포인터
    new_node->next = NULL;        // 현재 노드 다음 노드를 null로 만들어 준다.
    new_node->data = N;

    struct Node;
    
    // 2가지 경우
    // 1. head가 null인 경우에는 그대로 넣기
    if (*head==NULL){
        *head= new_node;
        return;
    }

    // 2. head가 null이 아닌 경우에는 노드를 타고 들어가서 null 노드를 발견하면 넣는다.
    else {
        Node_t *cur = *head;        // 커서로 시작지점을 알려줌
        while (cur->next!=NULL){    // 커서가 null을 가리킬 때 멈추도록 만든다.
            cur = cur -> next;      // 커서를 next로 이동
        }
        cur -> next = new_node;
        return;
    }
}

int main(){
    Node_t *head = NULL;
    for (int i=0;i<10;++i){
        insert(&head,i);
    }
    linkPrint(&head);
    delete(&head,1);
    printf("--------\n");
    linkPrint(&head);
}