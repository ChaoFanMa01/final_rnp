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

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "header.h"

struct BinarySearchTree;
typedef struct BinarySearchTree BSTree;
typedef BSTree                 *p_BSTree;
typedef void                   *BSTree_data;

typedef int  (*BSTree_cmp_func)(BSTree_data, BSTree_data);
typedef void (*BSTree_map_func)(BSTree_data, size_t);
typedef void (*BSTree_free_func)(BSTree_data *);
typedef void (*BSTree_copy_func)(BSTree_data, BSTree_data *);

/**
 * Initialization of a binary search tree.
 * @param1 which is to be initialized.
 * @param2, @param3, @param4, @param5 are functions used to
 * compare two data, map a data, free a data and copy a data.
 * If null pointer is given, default action will perform.
 */
extern Status BSTree_init(p_BSTree *, BSTree_cmp_func, BSTree_map_func, BSTree_free_func, BSTree_copy_func);
extern Status BSTree_insert(p_BSTree, BSTree_data);
extern Status BSTree_postOrderMap(p_BSTree);
extern Status BSTree_inOrderMap(p_BSTree);
extern Status BSTree_preOrderMap(p_BSTree);
extern Status BSTree_setCopy(p_BSTree, BSTree_copy_func);
extern Status BSTree_setFree(p_BSTree, BSTree_free_func);
extern Status BSTree_setMap(p_BSTree, BSTree_map_func);
extern Status BSTree_setCmp(p_BSTree, BSTree_cmp_func);
extern Status BSTree_delete(p_BSTree, BSTree_data);
extern Status BSTree_getMax(p_BSTree, BSTree_data *);
extern Status BSTree_getMin(p_BSTree, BSTree_data *);
extern size_t BSTree_size(p_BSTree);

#endif
