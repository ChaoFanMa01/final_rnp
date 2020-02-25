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

#ifndef COORDINATE_H
#define COORDINATE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "header.h"
#include "random.h"

typedef struct COORDINATE  Coordinate;
typedef Coordinate*        p_Coordinate;
typedef double             coordinate_t;

extern p_Coordinate Coordinate_Create2D(const coordinate_t, const coordinate_t);
extern p_Coordinate Coordinate_Create3D(const coordinate_t, const coordinate_t, const coordinate_t);
extern p_Coordinate Coordinate_CreateRandom2D(const int, const int);
extern void Coordinate_Free(p_Coordinate*);
extern Status Coordinate_Assign(p_Coordinate, p_Coordinate);
extern Status Coordinate_GetX(p_Coordinate, coordinate_t*);
extern Status Coordinate_GetY(p_Coordinate, coordinate_t*);
extern Status Coordinate_GetZ(p_Coordinate, coordinate_t*);
extern Status Coordinate_SetX(p_Coordinate, const coordinate_t);
extern Status Coordinate_SetY(p_Coordinate, const coordinate_t);
extern Status Coordinate_SetZ(p_Coordinate, const coordinate_t);
extern Bool Coordinate_Equal(p_Coordinate, p_Coordinate);
extern Bool Coordinate_NotEqual(p_Coordinate, p_Coordinate);
extern Bool Coordinate_XEqual(p_Coordinate, p_Coordinate);
extern Bool Coordinate_XNotEqual(p_Coordinate, p_Coordinate);
extern Bool Coordinate_YEqual(p_Coordinate, p_Coordinate);
extern Bool Coordinate_YNotEqual(p_Coordinate, p_Coordinate);
extern Bool Coordinate_ZEqual(p_Coordinate, p_Coordinate);
extern Bool Coordinate_ZNotEqual(p_Coordinate, p_Coordinate);
extern Status Coordinate_Add(p_Coordinate, p_Coordinate, p_Coordinate);
extern Status Coordinate_Subtract(p_Coordinate, p_Coordinate, p_Coordinate);
extern Status Coordinate_OnbodyAdd(p_Coordinate, p_Coordinate);
extern Status Coordinate_OnbodySubtract(p_Coordinate, p_Coordinate);
extern coordinate_t Coordinate_Distance(p_Coordinate, p_Coordinate);
extern Status Coordinate_2DPrint(p_Coordinate, FILE*);
extern Status Coordinate_3DPrint(p_Coordinate, FILE*);
#endif
