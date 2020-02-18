#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/header.h"
#include "../src/array_list.h"

void print(ArrayList_data);
int cmp(ArrayList_data, ArrayList_data);
void copy(ArrayList_data, ArrayList_data *);

int main(int argc, char* argv[])
{
  int             array[1000];
  int             n, i, j, *item;
  size_t          index;
  p_ArrayList     list, cpy;
  srand((unsigned)time(NULL));

  if (argc != 2)
    err_exit("parameter error", -1);

  if (ArrayList_init(&list) == ERROR)
    err_exit("memory error", -1);

  n = atoi(argv[1]);

  for (i = 0; i < n; ++i) {
    array[i] = rand() % 1000;
    printf("%4d, ", array[i]);
  }
  printf("\n\n");

  for (i = 0; i < n; ++i) {
    if (ArrayList_insertBack(list, &array[i]) == ERROR)
      print_msg("insert error");
  }

/* copy test.
  if (ArrayList_copy(list, &cpy, copy) == OK)
    ArrayList_map(cpy, 0, ArrayList_length(cpy) - 1, print);
  else
    print_msg("copy error");
  printf("\n");
*/

/* delete test.
  for (i = 0; i < n; ++i) {
    j = rand() % ArrayList_length(list) - 1;
    printf("delete the %dth item\n", j);
    if (ArrayList_delete(list, j, (void **)&item, NULL) == OK) {
//    if (ArrayList_deleteFront(list, (void **)&item, NULL) == OK) {
      ArrayList_map(list, 0, ArrayList_length(list) - 1, print);
      printf("\n%d has been deleted\n", *item);
    }
  }    
*/

/* max, min test.
 if (ArrayList_maxIndex(list, &index, cmp) == OK) {
   ArrayList_getData(list, index, (ArrayList_data *)&item);
   printf("The max is the %ldth item: %d\n", index, *item);
 } else {
   print_msg("error");
 }
 if (ArrayList_minIndex(list, &index, cmp) == OK) {
   ArrayList_getData(list, index, (ArrayList_data *)&item);
   printf("The min is the %ldth item: %d\n", index, *item);
 } else {
   print_msg("error");
 }
*/
  return 0;
}

int cmp(ArrayList_data d1, ArrayList_data d2)
{
  int  *pi1, *pi2;
  pi1 = (int *)d1; pi2 = (int *)d2;
  return *pi1 - *pi2;
}

void print(ArrayList_data data)
{
  printf("%4d, ", *((int *)data));
}

void copy(ArrayList_data src, ArrayList_data *cpy)
{
  *cpy = src;
}
