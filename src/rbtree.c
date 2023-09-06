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

// 후계자 찾는 함수 (오른쪽 min값)
node_t *find_successor(rbtree *t, node_t *x) {
  node_t *succ = x;   // x의 후계자
  while (succ->left != t->nil) {
    succ = succ->left;
  }
  return succ;
}

// 삭제할 노드를 떼어내고 후계자 붙이는 함수
void rb_transplant(rbtree *t, node_t *target, node_t *succ) {
  if (target->parent == t->nil)
    t->root = succ;
  else if (target == target->parent->left)
    target->parent->left = succ;
  else
    target->parent->right = succ;
  succ->parent = target->parent;
  return;
}

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK) {
    // 기준이 되는 노드가 왼쪽일 때
    if (x == x->parent->left) {
      w = x->parent->right;
      // CASE 1: w 레드
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t, x->parent);
        w = x->parent->right;
      }
      // CASE 2: w 블랙, w의 두 자녀 블랙
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // CASE 3: 꺾새 모양 w의 왼쪽 자녀 red, 오른쪽 자녀 black
      else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation(t, w);
          w = x->parent->right;
        }
        // CASE 4: 일렬 (최종)
        w->color = x->parent->color;
        w->right->color = RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root; // while 종료 조건
      }
    }
    // 기준이 되는 노드가 오른쪽일 때
    else if (x == x->parent->right) {
      w = x->parent->left;
      // CASE 1: w 레드
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }
      // CASE 2: w 블랙, w의 두 자녀 블랙
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // CASE 3: 꺾새 모양 w의 오른쪽 자녀 red, 왼쪽자녀 black
      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }
        // CASE 4: 일렬 (최종)
        w->color = x->parent->color;
        w->left->color = RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
  return;
}

int rbtree_erase(rbtree *t, node_t *target) {
  node_t *p = target;   // 삭제할 노드를 p에 복사하여 포인터처럼 활용
  color_t target_original_color = p->color;    // 지워질 노드의 색을 변수에 저장
  node_t *x;    // succ의 값의 자리를 저장해두는 포인터, fixup의 기준이 될 노드

  if (target->left == t->nil) {   // 왼쪽 자식이 없을 때
    x = target->right; // 정보 저장
    rb_transplant(t, target, target->right);
  }

  else if (target->right == t->nil) { // 오른쪽 자식이 없을 때
    x = target->left;    // 정보 저장
    rb_transplant(t, target, target->left);
  }

  else { // 자식 둘 다 있을 때
    p = find_successor(t, target->right);    // x
    target_original_color =  p->color;  // x color
    x = p->right;    // nil or 값 있을수도

    if (p->parent == target)
      x->parent = p;   // succ가 nil인 경우는 nil의 부모는 설정되지 않기 때문에 따로 지정해줘야함
    else {   // p의 부모가 삭제할 노드가 아닐 때
      rb_transplant(t, p, p->right);   // 타겟의 부모노드를 p의 오른쪽 자식과 연결
      p->right = target->right;   // 삭제할 노드의 오른 자식을 p의 오른쪽 자식으로 붙여줌
      p->right->parent = p;   // 부모도 p로 설정
    }
    rb_transplant(t, target, p);
    p->left = target->left;
    p->left->parent = p;
    p->color = target->color;
  }
  free(target);    // 할당 해제
  target = NULL;   // 누수 방지
  if (target_original_color == RBTREE_BLACK)  // 블랙 제거하면 5번특성 위반
    rbtree_erase_fixup(t, x);    // fixup 함수 호출
  return 0;
}

// 트리 중위순회 함수
int rbtree_inorder(node_t *nil, node_t *root, key_t *arr, const size_t n, int index) {
  if (root == nil)
    return index;
  
  // 재귀 종료 조건
  if (index == n)
    return index;
  
  index = rbtree_inorder(nil, root->left, arr, n, index); // 왼쪽 자식으로 재귀
  if (index < n)
    arr[index++] = root->key;   // arr[현재 인덱스] = 현재 노드의 key값
  index = rbtree_inorder(nil, root->right, arr, n, index); // 오른쪽 자식으로 재귀
  return index;
}

// 오름차순 arr
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t->root == t->nil)    // 트리의 루트가 nil이면 반환
    return 0;
  rbtree_inorder(t->nil, t->root, arr, n, 0);   // 중위 순회
  return 0;
}