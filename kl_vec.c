#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KL_VEC_CAPACITY 16
#define KL_LOAD_FACTOR 0.75f

typedef struct {
  size_t size;
  size_t capacity;
  char *items[KL_VEC_CAPACITY];
} KlVec;

KlVec *kl_vec_init(void) {
  KlVec *vec = malloc(sizeof(*vec));
  vec->capacity = KL_VEC_CAPACITY;

  return vec;
}

void kl_vec_set(KlVec *vec, size_t index, char *el) {
  char *new_el = malloc(strlen(el));
  strcpy(new_el, el);

  if (index > vec->capacity) {
    size_t new_size = index+1;
    char **new_items = malloc(sizeof(char *) * new_size);
    memcpy(new_items, vec->items, vec->capacity);
    vec->capacity = new_size;
    *vec->items = *new_items;
  }

  if(vec->size >= (size_t)(vec->capacity * KL_LOAD_FACTOR)) {
    char **new_items = malloc(sizeof(char *) * vec->capacity * 2);
    memcpy(new_items, vec->items, vec->capacity);
    vec->capacity *= 2;
    *vec->items = *new_items;
  }

  if (vec->items[index] == NULL) {
    vec->size = index + 1;
  }
  vec->items[index] = new_el;
}

char *kl_vec_get(KlVec *vec, size_t index) {
  if (index < vec->capacity) {
    return vec->items[index];
  } else {
    return NULL;
  }
}

void kl_vec_push(KlVec *vec, char *el) { kl_vec_set(vec, vec->size, el); }

char *kl_vec_pop(KlVec *vec) {
  size_t index = vec->size - 1;
  if (index < 0 || index > vec->capacity - 1) {
    return NULL;
  } else {
    char *el = kl_vec_get(vec, index);
    if (el != NULL) {
      vec->size--;
      vec->items[index] = NULL;
    }
    return el;
  }
}

void kl_vec_destroy(KlVec *vec) {
  for (size_t i = 0; i < vec->size; ++i) {
    free(vec->items[i]);
  }
  free(vec);
}

void kl_vec_print(KlVec *vec) {
  printf("Vector size: %zu\n", vec->size);
  printf("Vector capacity: %zu\n", vec->capacity);
  printf("Vector contents: ");
  printf("[");
  for (size_t i = 0; i < vec->size; ++i) {
    printf("%s", vec->items[i]);
    if (i < vec->size - 1)
      printf(", ");
  }
  printf("]\n");
}

int main() {
  KlVec *vec = kl_vec_init();
  kl_vec_print(vec);

  // test pop
  char *a = kl_vec_pop(vec);
  assert(a == NULL);
  assert(vec->size == 0);
  assert(vec->capacity == KL_VEC_CAPACITY);
  a = kl_vec_pop(vec);
  assert(a == NULL);
  assert(vec->size == 0);
  assert(vec->capacity == KL_VEC_CAPACITY);

  // test push & pop
  kl_vec_push(vec, "foo");
  kl_vec_print(vec);
  assert(vec->size == 1);
  a = kl_vec_pop(vec);
  assert(vec->size == 0);
  printf("a = %s\n", a);
  kl_vec_print(vec);
  a = kl_vec_pop(vec);
  assert(vec->size == 0);
  assert(a == NULL);

  // test get
  kl_vec_push(vec, "foo1");
  kl_vec_push(vec, "foo2");
  kl_vec_push(vec, "foo3");
  assert(vec->size == 3);
  assert(strcmp(kl_vec_get(vec, 0), "foo1") == 0);
  assert(vec->size == 3);
  assert(strcmp(kl_vec_get(vec, 1), "foo2") == 0);
  assert(strcmp(kl_vec_get(vec, 2), "foo3") == 0);
  assert(vec->size == 3);
  assert(kl_vec_get(vec, 3) == NULL);

  // test set
  kl_vec_set(vec, 5, "foo4");
  assert(vec->size == 6);
  kl_vec_print(vec);
  kl_vec_push(vec, "foo5");
  assert(vec->size == 7);
  kl_vec_print(vec);

  kl_vec_set(vec, 5, "foo42");
  assert(vec->size == 7);
  kl_vec_print(vec);

  kl_vec_set(vec, 500, "foo500");
  assert(vec->size == 501);
  assert(vec->capacity >= 501);
  kl_vec_print(vec);

  // test destroy
  kl_vec_destroy(vec);

  vec = kl_vec_init();
  kl_vec_print(vec);

  // test autoresize
  for(size_t i = 0; i < 35; ++i) {
    kl_vec_push(vec, "a");
    kl_vec_print(vec);
  }

  kl_vec_destroy(vec);

  return 0;
}
