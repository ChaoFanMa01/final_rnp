#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/header.h"
#include "../src/array_list.h"

void print(ArrayList_data);

int main(int argc, char* argv[])
{
  int             array[1000];
  int             n, i;
  p_ArrayList     list;
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

  ArrayList_map(list, 0, ArrayList_length(list) - 1, print);
  printf("\n");

  return 0;
}

void print(ArrayList_data data)
{
  printf("%4d, ", *((int *)data));
}
