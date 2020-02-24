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
#include <stddef.h>
#include <assert.h>

#include "binary_search_tree.h"

struct bst_node;
typedef struct bst_node  bst_node;
typedef struct bst_node *p_bst_node;

#define node    bst_node
#define p_node  p_bst_node
#define data_t  BSTree_data
#define tree    BSTree
#define p_tree  p_BSTree
#define cfunc   BSTree_cmp_func
#define mfunc   BSTree_map_func
#define ffunc   BSTree_free_func
#define ofunc   BSTree_copy_func

struct bst_node {
  data_t     data;
  p_node     left;
  p_node     right;
};

struct BinarySearchTree {
  p_node    root;
  size_t    size;
  cfunc     cmp;
  mfunc     map;
  ffunc     free;
  ofunc     copy;
};

static p_node create_node(data_t);
static Status insert(p_node, data_t, cfunc);
static Status pre_map(p_node, mfunc, size_t);
static Status in_map(p_node, mfunc, size_t);
static Status post_map(p_node, mfunc, size_t);
static Status find_parent(p_node, data_t, cfunc, p_node*);
static Status get_max(p_node, data_t*);
static Status get_min_parent(p_node, p_node*);
static Status get_min(p_node, data_t *);

size_t BSTree_size(p_tree bst)
{
  if (!bst)
    return 0;
  return bst->size;
}

Status BSTree_getMax(p_tree bst, data_t *re)
{
  if (!bst)
    return ERROR;
  return get_max(bst->root, re);
}

Status BSTree_getMin(p_tree bst, data_t *re)
{
  if (!bst)
    return ERROR;
  return get_min(bst->root, re);
}

static Status get_min(p_node sub, data_t *re)
{
  if (!sub || !re)
    return ERROR;
  for (; sub->left; sub = sub->left)
    ;
  *re = sub->data;
  return OK;
}

Status
BSTree_delete(p_tree bst, data_t data)
{
  p_node   pa = NULL, pa_min = NULL, del = NULL, min = NULL;

  if (!bst || !data)
    return ERROR;
	
  if (find_parent(bst->root, data, bst->cmp, &pa) == ERROR)
    return ERROR;
	
  if (!pa) {
    if (bst->root->left == NULL) {
      del = bst->root;
      bst->root = bst->root->right;
      if (bst->free)
        bst->free(del->data);
      free(del);
      bst->size--;
    } else if (bst->root->right == NULL) {
      del = bst->root;
      bst->root = bst->root->left;
      if (bst->free)
        bst->free(del->data);
      free(del);
      bst->size--;
    } else if (bst->root->right->left == NULL) {
      del = bst->root;
      bst->root->right->left = bst->root->left;
      bst->root = bst->root->right;
      if (bst->free)
        bst->free(del->data);
      free(del);
      bst->size--;
    } else {
      get_min_parent(bst->root->right, &pa_min);
      assert(pa_min);
      min = pa_min->left;
      assert(min);
      pa_min->left = min->right;
      del = bst->root;
      min->left = bst->root->left;
      min->right = bst->root->right;
      bst->root = min;
      if (bst->free)
        bst->free(del->data);
      free(del);
      bst->size--;
    }
  } else {
    if (pa->left)
      if (bst->cmp(pa->left->data, data) == 0)
        del = pa->left;
      if (pa->right)
        if (bst->cmp(pa->right->data, data) == 0)
          del = pa->right;
    assert(del);
    if (del->left == NULL) {
      if (pa->left == del)
        pa->left = del->right;
      else if (pa->right == del)
        pa->right = del->right;
      else
        assert(0);
      if (bst->free)
        bst->free(del->data);
      free(del);
      bst->size--;
    } else if (del->right == NULL) {
      if (pa->left == del)
        pa->left = del->left;
      else if (pa->right == del)
        pa->right = del->left;
      else
        assert(0);
      if (bst->free)
        bst->free(del->data);
      free(del);
      bst->size--;
    } else if (del->right->left == NULL) {
      if (pa->left == del) {
        del->right->left = del->left;
        pa->left = del->right;
        if (bst->free)
          bst->free(del->data);
        free(del);
        bst->size--;
      } else if (pa->right == del) {
        del->right->left = del->left;
        pa->right = del->right;
        if (bst->free)
          bst->free(del->data);
        free(del);
        bst->size--;
      } else {
        assert(0);
      }
    } else {
      get_min_parent(del->right, &pa_min);
      assert(pa_min);
      min = pa_min->left;
      assert(min);
      pa_min->left = min->right;
      min->left = del->left;
      min->right = del->right;
      if (pa->left == del)
        pa->left = min;
      else if (pa->right == del)
        pa->right = min;
      else
        assert(0);
      if (bst->free)
        bst->free(del->data);
      free(del);
      bst->size--;
    }
  }
  return OK;
}

/* @fn
 * Find the parent of the minimun node.
 * If the given root of the subtree is the minimum node,
 * p will set to NULL, and return DS_OK.
 */
static Status
get_min_parent(p_node sub, p_node* p)
{
  if (!sub || !p)
    return ERROR;
  if (sub->left == NULL) {
    *p = NULL;
    return OK;
  }
  for (; sub->left->left; sub = sub->left) ;
  *p = sub;
  return OK;
}

static Status
get_max(p_node sub, data_t* re)
{
  if (!sub || !re)
    return ERROR;
  for (; sub->right; sub = sub->right) ;
    *re = sub->data;

  return OK;
}

/* @fn
 * Find the parent node of the node containing given data,
 * and return the parent node through parameter "re".
 * It's worth NOTING that when the root stores given data,
 * "re" is set to NULL and return DS_OK; When NOT FIND given
 * data in this subtree, this function will return DS_ERROR.
 * That's to say when return is DS_ERROR, the data in "re"
 * is INVALID!!!
 */
static Status
find_parent(p_node sub, data_t data, cfunc cmp, p_node* re)
{
  if (!cmp || !sub || !data || !re)
    return ERROR;

  if (cmp(sub->data, data) == 0) {
    *re = NULL;
    return OK;
  }
  while (sub) {
    if (cmp(data, sub->data) > 0) {
      if (sub->right) {
        if (cmp(sub->right->data, data) == 0) {
          *re = sub;
          return OK;
        } else {
          sub = sub->right;
        }
      } else {
        return ERROR;
      }
    } else if (cmp(data, sub->data) < 0) {
      if (sub->left) {
        if (cmp(sub->left->data, data) == 0) {
          *re = sub;
          return OK;
        } else {
          sub = sub->left;
        }
      } else {
        return ERROR;
      }
    } else {
      return ERROR;
    }
  }
  return ERROR;
}

Status BSTree_setCmp(p_BSTree bst, cfunc func)
{
  assert(bst);
  assert(func);

  bst->cmp = func;
  return OK;
}

Status BSTree_setMap(p_BSTree bst, mfunc func)
{
  assert(bst);
  assert(func);

  bst->map = func;
  return OK;
}

Status BSTree_setFree(p_BSTree bst, ffunc func)
{
  assert(bst);
  assert(func);

  bst->free = func;
  return OK;
}

Status BSTree_setCopy(p_BSTree bst, ofunc func)
{
  assert(bst);
  assert(func);

  bst->copy = func;
  return OK;
}

Status BSTree_inOrderMap(p_tree bst)
{
  assert(bst);
  assert(bst->map);

  return in_map(bst->root, bst->map, 0);
}

Status BSTree_preOrderMap(p_tree bst)
{
  assert(bst);
  assert(bst->map);

  return pre_map(bst->root, bst->map, 0);
}

Status BSTree_postOrderMap(p_tree bst)
{
  assert(bst);
  assert(bst->map);

  return post_map(bst->root, bst->map, 0);
}

static Status post_map(p_node sub, mfunc func, size_t depth)
{
  if (!sub || !func)
    return ERROR;
  post_map(sub->left, func, depth + 1);
  post_map(sub->right, func, depth + 1);
  func(sub->data, depth);
  return OK;
}

static Status in_map(p_node sub, mfunc func, size_t depth)
{
  if (!sub || !func)
    return ERROR;
  in_map(sub->left, func, depth + 1);
  func(sub->data, depth);
  in_map(sub->right, func, depth + 1);
  return OK;
}

static Status pre_map(p_node sub, mfunc func, size_t depth)
{
  if (!sub || !func)
    return ERROR;
  func(sub->data, depth);
  pre_map(sub->left, func, depth + 1);
  pre_map(sub->right, func, depth + 1);
  return OK;
}

Status BSTree_insert(p_tree bst, data_t data)
{
  assert(bst);

  if (bst->root == NULL) {
    if (!(bst->root = create_node(data)))
      return ERROR;
    bst->size++;
    return OK;
  }
  if (insert(bst->root, data, bst->cmp) == OK) {
    bst->size++;
    return OK;
  }
  return ERROR;
}

static Status insert(p_node root, data_t data, cfunc cmp)
{
  p_node   nd;

  assert(root);
  assert(cmp);

  while (root) {
    if (cmp(data, root->data) > 0) {
      if (root->right) {
        root = root->right;
      } else {
        if (!(nd = create_node(data)))
          return ERROR;
        root->right = nd;
        return OK;
      }
    } else if (cmp(data, root->data) < 0) {
      if (root->left) {
        root = root->left;
      } else {
        if (!(nd = create_node(data)))
          return ERROR;
        root->left = nd;
        return OK;
      }
    } else {
      return ERROR;
    }
  }
  return ERROR;
}

Status BSTree_init(p_tree *pt, cfunc cmp, mfunc map,
                   ffunc free, ofunc copy)
{
  assert(pt);

  if (!(*pt = (p_tree)malloc(sizeof(tree))))
    return ERROR;
  (*pt)->root = NULL;
  (*pt)->size = 0;
  (*pt)->cmp  = cmp;
  (*pt)->map  = map;
  (*pt)->free = free;
  (*pt)->copy = copy;
  return OK;
}

static p_node create_node(data_t data)
{
  p_node pn;

  if (!(pn = (p_node)malloc(sizeof(node))))
    return NULL;
  pn->data  = data;
  pn->left  = NULL;
  pn->right = NULL;
  return pn;
}

#undef node
#undef p_node
#undef data_t
#undef tree
#undef p_tree
#undef cfunc
#undef mfunc
#undef ffunc
#undef ofunc
