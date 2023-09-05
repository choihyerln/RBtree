#include "rbtree.h"

#include <stdlib.h>

// 초기화
rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));  // 트리 생성
  node_t *nil_node = (node_t*)calloc(1, sizeof(node_t));  // nil 노드 생성
  nil_node->color = RBTREE_BLACK;   // 모든 리프 노드는 검정색
  t->nil = nil_node;      // T.nil
  t->root = nil_node;     // 부모 노드도 nil을 가리켜야 함
  return t;
}
//노드 메모리 해제
void delete_node(node_t *node, rbtree *t) {
  // 현재 노드가 nil이면 return : 아무 값이 없어서 할당될 것도 없음
  if (node == t->nil)
    return;
  // 재귀호출로 끝까지 간 후 해제
  delete_node(node->left, t);
  delete_node(node->right, t);
  free(node);
  node = NULL;  // 할당 해제 후 현재 노드 값을 null로 초기화
  return;
}

// 트리 메모리 해제
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  // 트리 없으면 return
  if (t==NULL)
    return;
  
  delete_node(t->root, t); // 생성된 노드들의 공간 해제
  free(t->nil);    // nil 노드 해제
  t->nil = NULL;   // 할당 해제 후 nil값 null로 초기화 : 예기치않은 오류와 안전성 확보를 위함
  free(t);
  t = NULL;
  return;
}

// 좌회전
void left_rotation(rbtree *t, node_t *x) {
  node_t *y = x->right;       // y는 현재 x의 오른쪽 자식 노드
  x->right = y->left;         // y의 왼쪽 서브 트리를 먼저 떼서 x의 오른쪽 자식에 붙여줌

  if (y->left != t->nil)      // 왼쪽 자식 노드가 있으니까
    y->left->parent = x;      // 그 자식의 부모를 x로 변경

  y->parent = x->parent;      // x의 부모를 y로 연결 (좌회전시킴)

  // x의 위치 잡아주는 조건문 -> 원래 x자리에 y를 넣어주기 위해서
  if (x->parent == t->nil)    // x의 부모가 nil이라면 (루트면)
    t->root = y;              // 트리의 루트를 y로 변경
  else if (x == x->parent->left)  // x가 왼쪽 자식이라면
    x->parent->left = y;      // x의 부모의 왼쪽자식을 y로 변경
  else                        // x가 오른쪽 자식이라면
    x->parent->right = y;     // x의 부모의 오른쪽을 y로 변경

  y->left = x;                // y의 왼쪽자식 x로
  x->parent = y;              // x의 부모를 y로
  return;
}

// 우회전
void right_rotation(rbtree *t, node_t *x) {
  node_t *y = x->left;    // y는 현재 x의 왼쪽 자식 노드
  x->left = y->right;     // y의 오른쪽 서브트리를 먼저 떼서 x의 왼쪽 자식에 붙여줌

  if (y->right != t->nil)
    y->right->parent = x;
  
  y->parent = x->parent;    // x의 부모를 y로 연결 (우회전시킴)

  // x의 위치 잡아주는 조건문 -> 원래 x자리에 y를 넣어주기 위해서
  if (x->parent == t->nil)    // x가 루트일 때
    t->root = y;
  else if (x == x->parent->left)   // x가 왼쪽 자식일 때
    x->parent->left = y;
  else                      // x가 오른쪽 자식일 때
    x->parent->right = y;

  y->right = x;   // y 위치 고정된 후 y의 오른쪽 자식 x로 변경
  x->parent = y;
  return;
}

// insert_fixup (색 변경)
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED)  {  // z의 부모가 red (double red)
  
    if (z->parent==z->parent->parent->left) {
      node_t *uncle = z->parent->parent->right;
      // CASE 1 = 부모 R + 삼촌 R
      if (uncle->color == RBTREE_RED) {     // 삼촌 레드일 때
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;    // 조상은 무조건 red, red여야 while문 돈다!
      }
      else if (uncle->color == RBTREE_BLACK) {    // 삼촌 블랙일 때
          // CASE 2-(1) = 꺾새 모양일 때
          if (z == z->parent->right) {            // 타켓이 오른쪽 자식이면
            z = z->parent;                        // 타켓을 그 부모로 바꾸고
            left_rotation(t, z);                  // left 로테이션 고고
          }
          // CASE 2-(2) = 일렬일 때
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          right_rotation(t, z->parent->parent);   // 조상 기준 우회전
      }
    }
    // z의 부모가 z조상의 오른쪽이면 위의 과정 반대로 하면 됨
    else if (z->parent==z->parent->parent->right) {
      node_t *uncle = z->parent->parent->left;
      // CASE 1 = 부모 R + 삼촌 R
      if (uncle->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (uncle->color == RBTREE_BLACK) {
        // CASE 2-(1) = 꺾새 모양일 때
        if (z == z->parent->left) {
          z = z->parent;
          right_rotation(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotation(t, z->parent->parent);
      }
    }
  } // while문 종료
  t->root->color = RBTREE_BLACK;    // root는 항상 black
  return;   // void형
} 

// 데이터 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *x = t->root;    // 트리의 루트 노드
  node_t *y = t->nil;     // 트리의 nil 노드

  while (x != t->nil) {   // x가 리프노드에 도달할 때까지 반복
    y = x;    // tmp 역할 (nil을 만나기 직전노드의 값에 z를 넣어줘야돼서 y에 저장)
    if (key < x->key)    // 만약 x의 key값보다 삽입할 key값이 작으면
      x = x->left;       // x를 x의 왼쪽으로 변경
    else    // 만약 x의 key값보다 삽입할 key값이 크거나 같으면
      x = x->right;     // x를 x의 오른쪽으로 변경
  }

  // while문 종료 = x가 nil을 가리킴 -> new_node(z) 삽입할 시기
  node_t *z = (node_t*)calloc(1, sizeof(node_t)); // z 노드 생성
  z->key = key;   // z의 키값을 넣어줌

  z->parent = y;  // z의 부모는 미리 저장해놓은 y
  if (y == t->nil)  // y가 트리의 nil = 비어있던 애
    t->root = z;    // 루트는 z

  else if (z->key < y->key)   // y의 key값이 z의 key값보다 크면
    y->left = z;    // z는 y의 왼쪽 자식

  else    // y의 key값이 z의 key값보다 작거나 같으면
    y->right = z;   // z는 y의 오른쪽 자식

  z->left = t->nil;   // z의 왼쪽 자식 nil
  z->right = t->nil;  // z의 오른쪽 자식 nil
  z->color = RBTREE_RED;  // new_node는 늘 red

  rbtree_insert_fixup(t, z);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root;    // root에서부터 검색 시작

  while (x != t->nil && key != x->key) {
    if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  if (x == t->nil)
    return NULL;

  return x;
}

// 트리의 최소값
node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;    // 루트부터 시작
  while (x->left != t->nil) {   // x의 왼쪽자식이 nil이 아닐 때
    x = x->left;    // 왼쪽으로 계속 이동
  }
  return x; // 왼쪽 자식 nil이면 그게 최소값
}

// 트리의 최대값
node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;    // 루트부터 시작
  while (x->right != t->nil) {    // 오른쪽 자식이 nil이 아닐 때 
    x = x->right;     // 오른쪽으로 계속 이동 
  }
  return x;   // 오른쪽 자식 nil이면 그게 최대값
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}