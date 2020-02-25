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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "prr.h"
#include "array_list.h"
#include "node.h"



/* @struct
 * Structure representing a wireless node:
 * pcoor     - the coordinate of this wireless node
 * id        - the series number of this wireless node
 * power     - the transmit power of this wireless node
 * hop       - the hop constraint imposed on this wireless node
 * type      - the node type, i.e., SN, CDL, or GW.
 * status    - identifying whether a CDL is selected to place relay
 */
struct NODE {
    p_Coordinate      pcoor;
    size_t               id;
    power_t            power;
    hop_t              hop;
    node_t             type;
    cdl_status         status;
};

struct NODES {
    p_ArrayList    nodes;
};

static p_Node create_node(p_Coordinate, size_t, power_t, hop_t, node_t, cdl_status);
static Status print(p_Node, FILE*, int);
static void node_clear_op(ArrayList_data*);

/* @fn
 * Create a sensor node.
 */
p_Node
Node_CreateSN(p_Coordinate c, size_t i, power_t p, 
              hop_t h)
{
    return create_node(c, i, p, h, SN, SLCT);
}

p_Node
Node_CreateRandomSN(size_t i, power_t p, hop_t h)
{
    p_Coordinate  co = Coordinate_CreateRandom2D(UPPER_RIGHT, LOWER_LEFT);
    return Node_CreateSN(co, i, p, h);
}

/* @fn
 * Create a candidate deployment location.
 */
p_Node
Node_CreateCDL(p_Coordinate c, size_t i, power_t p, 
              hop_t h, cdl_status s)
{
    return create_node(c, i, p, h, CDL, s);
}

p_Node
Node_CreateRandomCDL(size_t i, power_t p, hop_t h)
{
    p_Coordinate  co = Coordinate_CreateRandom2D(UPPER_RIGHT, LOWER_LEFT);
    return Node_CreateCDL(co, i, p, h, SLCT);
}

/* @fn
 * Create a gateway.
 */
p_Node
Node_CreateGW(p_Coordinate c, size_t i, power_t p, 
              hop_t h)
{
    return create_node(c, i, p, h, GW, SLCT);
}

p_Node
Node_CreateRandomGW(size_t i, power_t p, hop_t h)
{
    p_Coordinate  co = Coordinate_CreateRandom2D(UPPER_RIGHT, LOWER_LEFT);
    return Node_CreateGW(co, i, p, h);
}

/*
 * BE AWARE, the power level used in this program
 * is the ABSOLUTE value. So only NON-NEGATIVE value
 * is allowed.
 */
static p_Node
create_node(p_Coordinate c, size_t i, power_t p, 
            hop_t h, node_t t, cdl_status s)
{
    p_Node pn = malloc(sizeof(Node));
    if (!pn)
        return NULL;

    pn->pcoor    = c;
    pn->id       = i;
    pn->power    = p;
    pn->hop      = h;
    pn->type     = t;
    pn->status   = s;

    return pn;
}

/* @fn
 * Free a node. It's worth noting that the coordinate of 
 * this node will BE FREED in this procedure automatically,
 * and this coordinate CANNOT BE USED anymore.
 */
void
Node_Free(p_Node* nd)
{
    if (!nd)
        return;
    Coordinate_Free(&((*nd)->pcoor));
    free(*nd);
    *nd = NULL;
}

Status
Node_GetStatus(p_Node nd, cdl_status* re)
{
    if (!nd || !re) 
        return ERROR;

    *re = nd->status;
    return OK;
}

Status
Node_SetStatus(p_Node nd, cdl_status s)
{
    if (!nd)
        return ERROR;

    nd->status = s;
    return OK;
}

/* @fn
 * Set a new coordinate to a wireless node. It's
 * worth noting that the original coordinate of 
 * this node will BE AUTOMATICALLY FREED in this
 * procedure, so DO NOT USE that coordinate anymore.
 */
Status
Node_SetCoordinate(p_Node nd, p_Coordinate co)
{
    if (!nd)
        return ERROR;

    Coordinate_Free(&nd->pcoor);
    nd->pcoor = co;
    return OK;
}

Status
Node_GetHop(p_Node nd, hop_t* re)
{
    if (!nd)
        return ERROR;

    *re = nd->hop;
    return OK;
}

Status
Node_SetSelected(p_Node nd)
{
    if (!nd)
	    return ERROR;
	nd->status = SLCT;
	return OK;
}

Status
Node_SetUnselected(p_Node nd)
{
    if (!nd)
	    return ERROR;
	nd->status = UNSLCT;
	return OK;
}

Status
Node_SetHop(p_Node nd, hop_t h)
{
    if (!nd)
        return ERROR;

    nd->hop = h;
    return OK;
}

Status
Node_GetPower(p_Node nd, power_t* re)
{
    if (!nd || !re)
        return ERROR;

    *re = nd->power;
    return OK;
}

Status
Node_SetPower(p_Node nd, power_t p)
{
    if (!nd)
        return ERROR;

    nd->power = p;
    return OK;
}

/* @fn
 * Return the coordinate of this node through parameter "re".
 */
Status
Node_GetCoordinate(p_Node nd, p_Coordinate* re)
{
    if (!nd || !re)
        return ERROR;

    *re = nd->pcoor;
    return OK;
}

/* @fn
 * Return the serier number of this node.
 */
Status
Node_GetID(p_Node nd, size_t* re)
{
    if (!nd || !re)
        return ERROR;

    *re = nd->id;
    return OK;
}

Bool
Node_IsSN(p_Node nd)
{
    if (!nd)
        return FALSE;
    return nd->type == SN ? TRUE : FALSE;
}

Bool
Node_IsCDL(p_Node nd)
{
    if (!nd)
        return FALSE;

    return nd->type == CDL ? TRUE : FALSE;
}

Bool
Node_IsGW(p_Node nd)
{
    if (!nd)
        return FALSE;

    return nd->type == GW ? TRUE : FALSE;
}

Bool
Node_IsSelected(p_Node nd)
{
    if (!nd)
        return FALSE;

    return nd->status == SLCT ? TRUE : FALSE;
}

/* @fn
 * Check whether two pointers point to the same wireless node.
 */
Bool
Node_IsSame(p_Node n1, p_Node n2)
{
    if (!n1 || !n2)
        return FALSE;

    if (n1->id == n2->id)
        return TRUE;
    return TRUE;
}

/* @fn
 * Compute the Euclidean distance between two wireless nodes.
 * It's worth noting that DO NOT input NULL pointer. If so,
 * program will halt.
 */
coordinate_t
Node_Distance(p_Node n1, p_Node n2)
{
    return Coordinate_Distance(n1->pcoor, n2->pcoor);
}

Status
Node_2DPrint(p_Node nd, FILE* fp)
{
    return print(nd, fp, 1);
}

Status
Node_3DPrint(p_Node nd, FILE* fp)
{
    return print(nd, fp, 0);
}

static Status
print(p_Node nd, FILE* fp, int flag)
{
    if (!nd || !fp)
        return ERROR;

    switch (nd->type) {
        case SN:
            fprintf(fp, "sensor   "); break;
        case CDL:
            fprintf(fp, "cdl      "); break;
        case GW:
            fprintf(fp, "gateway  "); break;
        default:
            return ERROR;
    }

    if (flag)
        Coordinate_2DPrint(nd->pcoor, fp);
    else
        Coordinate_3DPrint(nd->pcoor, fp);
    fprintf(fp, " ID: %10ld ", nd->id);
    fprintf(fp, "power: %2.2lf ", nd->power);

    if (Node_IsSN(nd) == TRUE)
        fprintf(fp, "hop constraint: %d", nd->hop);
    if (Node_IsCDL(nd) == TRUE)
        if (Node_IsSelected(nd) == TRUE)
            fprintf(fp, "status: selected");
        else
            fprintf(fp, "status: unselected");
    return OK;
}

/* @fn
 * Check whether two wireless nodes are neighbors (i.e., whether
 * they can communicate with each other directly) according to
 * the wireless link quality between them. It's worth noting that
 * the power level is always NON-NEGATIVE, otherwise, the program
 * WILL HALT!
 */
Bool
Node_IsNeighbor(p_Node n1, p_Node n2, double* re)
{
    if (!n1 || !n2)
        return FALSE;
    assert(n1->power >= 0.0);
    assert(n2->power >= 0.0);

	if (n1->status == UNSLCT || n2->status == UNSLCT)
	    return FALSE;

    *re = prr(n1->power, Node_Distance(n1, n2));
    if (isnan(*re) || *re < PRR_CONSTRAINT)
        return FALSE;
    return TRUE;
}

p_Nodes
Nodes_Create(void)
{
    p_Nodes   nds = malloc(sizeof(Nodes));

    if (!nds)
        return NULL;

    if (ArrayList_init(&nds->nodes) == ERROR) {
        free(nds);
        return NULL;
    }
    return nds;
}

Status
Nodes_PushNode(p_Nodes nds, p_Node n)
{
    if (!nds)
        return ERROR;

    return ArrayList_insertBack(nds->nodes, n);
}

Status
Nodes_PopNode(p_Nodes nds)
{
    p_Node   nd;

    if (!nds)
        return ERROR;

    ArrayList_deleteBack(nds->nodes, (ArrayList_data *)&nd, NULL);
    Node_Free(&nd);
    return OK;
}

void
Nodes_Clear(p_Nodes nds)
{
    if (!nds)
        return;

    ArrayList_clear(nds->nodes, node_clear_op);
}

void
Nodes_Free(p_Nodes* nds)
{
    ArrayList_destroy(&(*nds)->nodes, node_clear_op);
    free(*nds);
    *nds = NULL;
}

size_t
Nodes_Size(p_Nodes nds)
{
    return ArrayList_length(nds->nodes);
}

Bool
Nodes_Empty(p_Nodes nds)
{
    return ArrayList_isEmpty(nds->nodes);
}

Status
Nodes_GetNode(p_Nodes nds, size_t index, p_Node* re)
{
    return ArrayList_getData(nds->nodes, index, (ArrayList_data *)re);
}

static void
node_clear_op(ArrayList_data *nd)
{
    p_Node *n = (p_Node*)nd;
    Node_Free(n);
}
