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

typedef int   (*ArrayList_cmp_func)(ArrayList_data, ArrayList_data);
typedef void  (*ArrayList_map_func)(ArrayList_data);
typedef void  (*ArrayList_clear_func)(ArrayList_data *);

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
extern void ArrayList_map(p_ArrayList, size_t, size_t, ArrayList_map_func);
size_t ArrayList_length(p_ArrayList);
size_t ArrayList_capacity(p_ArrayList);

#endif
