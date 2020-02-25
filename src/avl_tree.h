/*
 * final_rnp
 * Copyright (C) 2019-2025 Chaofan Ma <chaofanma@hotmail.com>
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

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "header.h"

typedef struct _avl_tree   AVLTree;
typedef AVLTree*           pt_AVLTree;
typedef void *             avlt_data_t;

typedef int  (*avl_cmp)(avlt_data_t, avlt_data_t);
typedef void (*avlt_map_func)(avlt_data_t, size_t);
typedef void (*avlt_inter_func)(pt_AVLTree, avlt_data_t);

extern pt_AVLTree    AVLTree_Create(avl_cmp);
extern Status       AVLTree_PreOrderMap(pt_AVLTree, avlt_map_func);
extern Status       AVLTree_InOrderMap(pt_AVLTree, avlt_map_func);
extern Status       AVLTree_PostOrderMap(pt_AVLTree, avlt_map_func);
extern Status       AVLTree_GetMax(pt_AVLTree, avlt_data_t*);
extern Status       AVLTree_GetMin(pt_AVLTree, avlt_data_t*);
extern Status       AVLTree_Insert(pt_AVLTree, avlt_data_t);
extern size_t        AVLTree_Size(pt_AVLTree);
extern Status       AVLTree_Delete(pt_AVLTree, avlt_data_t);
extern size_t        AVLTree_Height(pt_AVLTree);
extern Bool       AVLTree_Contain(pt_AVLTree avl, avlt_data_t data);
extern void          AVLTree_Free(pt_AVLTree*);
extern void          AVLTree_Clear(pt_AVLTree);
extern pt_AVLTree    AVLTree_Copy(pt_AVLTree);
extern Status       AVLTree_InterOpt(pt_AVLTree, pt_AVLTree, avlt_inter_func);
#endif
