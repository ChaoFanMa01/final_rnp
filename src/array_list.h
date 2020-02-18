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

/* List based array. */

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stddef.h>

#include "header.h"

typedef struct ArrayList  ArrayList;
typedef ArrayList         *p_ArrayList;
typedef void              *ArrayList_data;

/**
 * Function type used to compare two given data, the comparison
 * result should meet:
 * (1) > 0,  if @param1 is greater than @param2;
 * (2) = 0,  if @param1 is equal to @param2;
 * (3) < 0,  if @param1 is less than @param2.
 */
typedef int   (*ArrayList_cmp_func)(ArrayList_data, ArrayList_data);
typedef void  (*ArrayList_map_func)(ArrayList_data);
/** 
 * Function used to free the memory of one data stored in list.
 */
typedef void  (*ArrayList_free_func)(ArrayList_data *);
typedef void  (*ArrayList_copy_func)(ArrayList_data, ArrayList_data *);

extern Status ArrayList_init(p_ArrayList *);
/** 
 * Insert an item (i.e., @param3) before given index 
 * @param2 in the list @param1. 
 */
extern Status ArrayList_insert(p_ArrayList, size_t, ArrayList_data);
/** 
 * Insert an item (i.e., @param2) at the first 
 * position of the list @param1. 
 */
extern Status ArrayList_insertFront(p_ArrayList, ArrayList_data);
extern Status ArrayList_insertBack(p_ArrayList, ArrayList_data);
/**
 * Applying map function @param4 to items in the 
 * range denoted by @param2 to @param3 of a list @param1.
 */
extern void   ArrayList_map(p_ArrayList, size_t, size_t, ArrayList_map_func);
extern size_t ArrayList_length(p_ArrayList);
extern size_t ArrayList_capacity(p_ArrayList);
/**
 * Delete the item denoted by index @param2 from given 
 * list @param1, and return it through @param3.
 */
extern Status ArrayList_delete(p_ArrayList, size_t, ArrayList_data *, ArrayList_free_func);
extern Status ArrayList_deleteFront(p_ArrayList, ArrayList_data *, ArrayList_free_func);
extern Status ArrayList_deleteBack(p_ArrayList, ArrayList_data *, ArrayList_free_func);
extern Bool   ArrayList_isEmpty(p_ArrayList);
extern Status ArrayList_getData(p_ArrayList, size_t, ArrayList_data *);
extern Status ArrayList_getFrontData(p_ArrayList, ArrayList_data *);
extern Status ArrayList_getBackData(p_ArrayList, ArrayList_data *);
extern Status ArrayList_replace(p_ArrayList, size_t, ArrayList_data, ArrayList_free_func);
extern Status ArrayList_index(p_ArrayList, ArrayList_data, size_t *, ArrayList_cmp_func);
extern Bool   ArrayList_isContain(p_ArrayList, ArrayList_data, ArrayList_cmp_func);
extern Status ArrayList_clear(p_ArrayList, ArrayList_free_func);
extern Status ArrayList_destroy(p_ArrayList *, ArrayList_free_func);
extern Status ArrayList_maxData(p_ArrayList, ArrayList_data *, ArrayList_cmp_func);
extern Status ArrayList_minData(p_ArrayList, ArrayList_data *, ArrayList_cmp_func);
extern Status ArrayList_maxIndex(p_ArrayList, size_t *, ArrayList_cmp_func);
extern Status ArrayList_minIndex(p_ArrayList, size_t *, ArrayList_cmp_func);
/**
 * Copy a list @param1, and return this copy via @param2.
 * @param3 is a function provided by user performing data
 * copy for each node in list. If @param3 is null, a shallow
 * copy is made, i.e., @param1 and @param2 will point to the
 * same data set.
 */
extern Status ArrayList_copy(p_ArrayList, p_ArrayList *, ArrayList_copy_func);

#endif
