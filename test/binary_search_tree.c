#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/binary_search_tree.h"

int cmp(BSTree_data, BSTree_data);
void print(BSTree_data, size_t);

int main(int argc, char *argv[])
{
  int      array[1000];
  size_t   size, i;
  p_BSTree bst;
  int      input;
  srand((unsigned)time(0));

  if (argc != 2)
    err_exit("parameter error", -1);

  size = atoi(argv[1]);
  if (BSTree_init(&bst, cmp, print, NULL, NULL) == ERROR)
    err_exit("init error", -1);
  for (i = 0; i < size; ++i)
    array[i] = rand() % 1000;
  for (i = 0; i < size; ++i)
    BSTree_insert(bst, &array[i]);
  BSTree_inOrderMap(bst);

  printf("delete: ");
  scanf("%d", &input);

  if (BSTree_delete(bst, &input) == ERROR)
    print_msg("fail to delete");
  else {
    print_msg("remain size: %ld", BSTree_size(bst));
    BSTree_inOrderMap(bst);
  }

  return 0;
}

int cmp(BSTree_data pd1, BSTree_data pd2)
{
  int *pi1 = (int *)pd1;
  int *pi2 = (int *)pd2;
  return *pi1 - *pi2;
}

void print(BSTree_data pd, size_t depth)
{
  size_t  i;
  int    *pi = (int *)pd;
  for (i = 0; i < depth; ++i)
    printf(" ");
  printf("%3d\n", *pi);
}
