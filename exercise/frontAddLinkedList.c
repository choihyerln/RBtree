#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct listNode {
    struct listNode *next;      // 현재 노드의 다음 노드 포인터
    int data;                   // 노드에 저장할 데이터
} Node;

Node *pStart = NULL;    // 리스트의 첫 노드의 포인터
Node *pEnd = NULL;      // 리스트의 마지막 노드의 포인터

// 리스트 앞에 추가
void addFront(int val) {
    Node *new_node;          // 리스트에 추가할 새 노드 생성
    new_node = (Node*)malloc(sizeof(Node));
    new_node -> data = val; // data 필드에 val 저장

    if(pStart == NULL) {    // 첫번째 노드일 경우
        new_node -> next = NULL;    // 다음 노드가 없으므로 null
        pStart = pEnd = new_node;
    }
    else {                  // 이미 값이 있는 경우
        new_node -> next = pStart;  // 새로 추가한 노드의 다음 노드가 기존의 첫번째 노드
        pStart = new_node;      // 첫번째 노드를 이번에 새로 만든 노드로 변경
    }
}

void printList(Node *cur) {
    // current가 가리키는 리스트 출력
    cur = pStart;
    while (cur != NULL) {
        printf("%d\n", cur->data);
        cur = cur->next;
    }    
}

// 특정한 data를 가진 노드 1개만 지우는 delete() 함수 구현
void delete(int val) {
    // data 필드 값이 val인 Node 1개를 찾아 삭제
    if (pStart == NULL)
        return;

    if (pStart->data == val) {
        Node *cur = pStart;
        free(cur);
        pStart = pStart -> next;
    return;
    }
    
    Node *p = pStart;
    Node *q = p->next;
    while (q != NULL)
    {
        if (q->data == val) {
            p->next = q->next;
            free(q);
            break;
        }
        p=q;
        q=p->next;
    }
}

int main(void) {
    for (int i=1; i<6; i++)
        addFront(i);        // 새 노드를 만들어 리스트 앞에 추가
    printList(pStart);
    printf("---------\n");
    delete(4);
    printList(pStart);
    return 0;
}