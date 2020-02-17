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

  if (!pl)
    return ERROR;
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

#undef data_t
