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

#ifndef NODE_H
#define NODE_H


#include "header.h"
#include "coordinate.h"

typedef double   power_t;
typedef int      hop_t;

typedef struct NODE     Node;
typedef Node*           p_Node;
typedef struct NODES    Nodes;
typedef Nodes*          p_Nodes;

/* @enum
 * Enumerate type for identifying the type of a wireless node:
 * SN  - wireless sensor node
 * CDL - candidate deployment location
 * GW  - gateway
 */
typedef enum {
    SN, CDL, GW
} node_t;

/* @enum
 * Enumerate type for identifying the status of a CDL:
 * SLCT    - this CDL is selected to place a relay node
 * UNSLCT  - this CDL is not selected to place a relay node
 */
typedef enum {
    SLCT, UNSLCT
} cdl_status;

extern p_Node      Node_CreateSN(p_Coordinate, size_t, power_t, hop_t);
extern p_Node      Node_CreateRandomSN(size_t, power_t, hop_t);
extern p_Node      Node_CreateCDL(p_Coordinate, size_t, power_t, hop_t, cdl_status);
extern p_Node      Node_CreateRandomCDL(size_t, power_t, hop_t);
extern p_Node      Node_CreateGW(p_Coordinate, size_t, power_t, hop_t);
extern p_Node      Node_CreateRandomGW(size_t, power_t, hop_t);
extern void         Node_Free(p_Node*);
extern Status      Node_GetCoordinate(p_Node, p_Coordinate*);
extern Status      Node_SetCoordinate(p_Node, p_Coordinate);
extern Status      Node_GetID(p_Node, size_t*);
extern Status      Node_GetPower(p_Node, power_t*);
extern Status      Node_GetHop(p_Node, hop_t*);
extern Status      Node_GetStatus(p_Node, cdl_status*);
extern Status      Node_SetStatus(p_Node, cdl_status);
extern Status      Node_SetSelected(p_Node);
extern Status      Node_SetUnselected(p_Node);
extern Status      Node_SetHop(p_Node, hop_t);
extern Status      Node_SetPower(p_Node, power_t);
extern Bool      Node_IsSN(p_Node);
extern Bool      Node_IsCDL(p_Node);
extern Bool      Node_IsGW(p_Node);
extern Bool      Node_IsSelected(p_Node);
extern Bool      Node_IsSame(p_Node, p_Node);
extern coordinate_t Node_Distance(p_Node, p_Node);
extern Status      Node_2DPrint(p_Node, FILE*);
extern Status      Node_3DPrint(p_Node, FILE*);
extern Bool      Node_IsNeighbor(p_Node, p_Node, double*);


extern p_Nodes     Nodes_Create(void);
extern Status      Nodes_PushNode(p_Nodes, p_Node);
extern Status      Nodes_PopNode(p_Nodes);
extern void         Nodes_Clear(p_Nodes);
extern void         Nodes_Free(p_Nodes*);
extern size_t       Nodes_Size(p_Nodes);
extern Bool      Nodes_Empty(p_Nodes);
extern Status      Nodes_GetNode(p_Nodes, size_t, p_Node*);
#endif
