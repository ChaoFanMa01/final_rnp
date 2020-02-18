/*
 * final_rnp
 * Copyright (C) 2019-2020 Chaofan Ma <chaofanma@hotmail.com>
 *
 * This file is part of final_rnp.
 *
 * final_rnp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * final_rnp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with final_rnp.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#include "header.h"
#include "array_list.h"

#define data_t ArrayList_data

static const size_t  INIT_CAP = 10;

struct ArrayList {
  size_t    capacity;
  size_t    length;
  data_t    *base;
};

static Status reserve(p_ArrayList, size_t);
static Status insert(p_ArrayList, size_t, data_t);
static Status delete(p_ArrayList, size_t, data_t *, ArrayList_free_func);
static Status getData(p_ArrayList, size_t, data_t *);
static Status index(p_ArrayList, data_t, size_t *, ArrayList_cmp_func);
static Status clear(p_ArrayList, ArrayList_free_func);
static Status min_index(p_ArrayList, size_t *, ArrayList_cmp_func);
static Status max_index(p_ArrayList, size_t *, ArrayList_cmp_func);

Status ArrayList_copy(p_ArrayList src, p_ArrayList *cpy,
                      ArrayList_copy_func func)
{
  int i;

  assert(src);
  assert(cpy);

  if (!(*cpy = (p_ArrayList)malloc(sizeof(ArrayList))))
    err_return("ArrayList_copy: memory error");
  if (!((*cpy)->base = (data_t *)malloc(sizeof(data_t) * src->capacity)))
    err_return("ArrayList_copy: memory error");
 
  for (i = 0; i < src->length; ++i)
    if (func)
      func(*(src->base + i), (*cpy)->base + i);
    else
      *((*cpy)->base + i) = *(src->base + i);
  (*cpy)->length = src->length;
  (*cpy)->capacity = src->capacity;
  return OK;
}

Status ArrayList_minIndex(p_ArrayList pl, size_t *pi,
                          ArrayList_cmp_func cmp)
{
  return min_index(pl, pi, cmp);
}

Status ArrayList_maxIndex(p_ArrayList pl, size_t * pi,
                          ArrayList_cmp_func cmp)
{
  return max_index(pl, pi, cmp);
}

Status ArrayList_minData(p_ArrayList pl, data_t *pe, 
                         ArrayList_cmp_func cmp)
{
  size_t  i;

  if (min_index(pl, &i, cmp) == OK) {
    *pe = *(pl->base + i);
    return OK;
  }
  return ERROR;
}

Status ArrayList_maxData(p_ArrayList pl, data_t *pe, 
                         ArrayList_cmp_func cmp)
{
  size_t  i;

  if (max_index(pl, &i, cmp) == OK) {
    *pe = *(pl->base + i);
    return OK;
  }
}

Status ArrayList_clear(p_ArrayList pl, ArrayList_free_func func)
{
  return clear(pl, func);
}

Status ArrayList_destroy(p_ArrayList *pl, ArrayList_free_func func)
{
  assert(pl);
  clear(*pl, func);
  free(*pl);
  *pl = NULL;
  return OK;
}

Bool ArrayList_isContain(p_ArrayList pl, data_t data,
                         ArrayList_cmp_func cmp)
{
  size_t    i;
  if (index(pl, data, &i, cmp) == OK)
    return TRUE;
  return FALSE;
}

Status ArrayList_index(p_ArrayList pl, data_t data, 
                       size_t *pi, ArrayList_cmp_func cmp)
{
  return index(pl, data, pi, cmp);
}

Status ArrayList_replace(p_ArrayList pl, size_t index, data_t data,
                         ArrayList_free_func func)
{
  assert(pl);
  if (index < 0 || index >= pl->length)
    return ERROR;
  if (func)
    func(*(pl->base + index));
  *(pl->base + index) = data;
  return OK;
}

Status ArrayList_getData(p_ArrayList pl, size_t index, data_t *pe)
{
  return getData(pl, index, pe);
}

Status ArrayList_getFrontData(p_ArrayList pl, data_t *pe)
{
  return getData(pl, 0, pe);
}

Status ArrayList_getBackData(p_ArrayList pl, data_t *pe)
{
  return getData(pl, ArrayList_length(pl) - 1, pe);
}

Bool ArrayList_isEmpty(p_ArrayList pl)
{
  assert(pl);
  return pl->length <= 0;
}

Status ArrayList_delete(p_ArrayList pl, size_t index, data_t *pe,
                        ArrayList_free_func func)
{
  return delete(pl, index, pe, func);
}

Status ArrayList_deleteFront(p_ArrayList pl, data_t *pe,
                             ArrayList_free_func func)
{
  return delete(pl, 0, pe, func);
}

Status ArrayList_deleteBack(p_ArrayList pl, data_t *pe,
                            ArrayList_free_func func)
{
  return delete(pl, pl->length - 1, pe, func);
}

size_t ArrayList_length(p_ArrayList pl)
{
  assert(pl);
  return pl->length;
}

size_t ArrayList_capacity(p_ArrayList pl)
{
  assert(pl);
  return pl->capacity;
}

void ArrayList_map(p_ArrayList pl, size_t b, 
                   size_t e, ArrayList_map_func func)
{
  int i;

  assert(pl);
  if (pl->length <= 0)
    return;

  assert(b <= e);
  assert(b >= 0);
  assert(e < pl->length);

  for (i = b; i <= e; ++i)
    func(*(pl->base + i));
}

Status ArrayList_init(p_ArrayList *pl)
{
  if (!pl) {
    print_msg("ArrayList_init: null pointer");
    return ERROR;
  }
  if (!(*pl = (p_ArrayList)malloc(sizeof(ArrayList)))) {
    print_msg("ArrayList_init: memory allocation error");
    return ERROR;
  }
  if (!((*pl)->base = (data_t *)malloc(sizeof(data_t) * INIT_CAP))) {
    free(*pl);
    print_msg("ArrayList_init: memory allocation error");
    return ERROR;
  }
  (*pl)->length   = 0;
  (*pl)->capacity = INIT_CAP;
  return OK;
}

Status ArrayList_insert(p_ArrayList pl, size_t index, data_t data)
{
  return insert(pl, index, data);
}

Status ArrayList_insertFront(p_ArrayList pl, data_t data)
{
  return insert(pl, 0, data);
}

Status ArrayList_insertBack(p_ArrayList pl, data_t data)
{
  return insert(pl, pl->length, data);
}

/*
 * Enlarge the memery of a list `pl`.
 */
static Status reserve(p_ArrayList pl, size_t n)
{
  data_t   *new_base;
  size_t    i;

  assert(pl);
  if (!(new_base = (data_t *)malloc(sizeof(data_t) * pl->capacity * 2)))
    return ERROR;

  for (i = 0; i < pl->length; ++i)
    *(new_base + i) = *(pl->base + i);
  free(pl->base);
  pl->base = new_base;
  pl->capacity *= 2;
  return OK;
}

static Status insert(p_ArrayList pl, size_t index, data_t data)
{
  size_t   i;
  assert(pl);
  /*
   * index == pl->lenght means inserting this data
   * at the rear of this list.
   */
  if (index < 0 || index > pl->length)
    return ERROR;
  if (pl->length + 1 > pl->capacity)
    if (reserve(pl, 2) == ERROR)
      return ERROR;
  for (i = pl->length; i > index; --i)
    *(pl->base + i) = *(pl->base + i - 1);
  *(pl->base + i) = data;
  pl->length++;
  return OK;
}

static Status delete(p_ArrayList pl, size_t index, data_t *pe,
                     ArrayList_free_func func)
{
  int  i;
  assert(pl);
  if (index < 0 || index >= pl->length)
    err_return("range error");
  *pe = *(pl->base + index);
  for (i = index; i < pl->length - 1; ++i) {
    if (func)
      func(*(pl->base + i));
    *(pl->base + i) = *(pl->base + i + 1);
  }
  pl->length--;
  return OK;
}

static Status getData(p_ArrayList pl, size_t index, data_t *pe)
{
  assert(pl);
  if (pl->length <= 0)
    return ERROR;
  if (index < 0 || index >= pl->length)
    return ERROR;

  *pe = *(pl->base + index);
  return OK;
}

static Status index(p_ArrayList pl, data_t data, size_t *pi,
                    ArrayList_cmp_func cmp)
{
  int i;
  assert(pl);
  assert(pi);

  for (i = 0; i < pl->length; ++i)
    if (cmp(*(pl->base + i), data) == 0) {
      *pi = i;
      return OK;
    }
  return ERROR;
}

static Status clear(p_ArrayList pl, ArrayList_free_func func)
{
  int i;

  assert(pl);
  if (func)
    for (i = 0; i < pl->length; ++i)
      func(*(pl->base + i));
  pl->length = 0;
  return OK;
}

static Status min_index(p_ArrayList pl, size_t *pi, 
                        ArrayList_cmp_func func)
{
  size_t    i;

  assert(pl);
  assert(func);

  if (pl->length <= 0)
    return ERROR;

  for (i = 0, *pi = 0; i < pl->length - 1; ++i)
    if (func(*(pl->base + *pi), *(pl->base + i + 1)) > 0)
      *pi = i + 1;
  return OK;
}

static Status max_index(p_ArrayList pl, size_t *pi, 
                        ArrayList_cmp_func func)
{
  size_t    i;

  assert(pl);
  assert(func);

  if (pl->length <= 0)
    return ERROR;

  for (i = 0, *pi = 0; i < pl->length - 1; ++i)
    if (func(*(pl->base + *pi), *(pl->base + i + 1)) < 0)
      *pi = i + 1;
  return OK;
}

#undef data_t
