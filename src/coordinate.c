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

#include "coordinate.h"

struct COORDINATE {
    coordinate_t    x;
    coordinate_t    y;
    coordinate_t    z;
};

typedef enum {
    x_field, y_field, z_field
} coor_field;

static Status get_coordinate(p_Coordinate, const coor_field, coordinate_t*);
static Status set_coordinate(p_Coordinate, const coor_field, const coordinate_t);
static Bool check_equality(p_Coordinate, p_Coordinate, coor_field);

/* @fn
 * Create a 2D coordinate with given x-coordinate and
 * y-coordinate, and initialize the z-coordinate to 0.0.
 */
p_Coordinate
Coordinate_Create2D(const coordinate_t x, const coordinate_t y)
{
    p_Coordinate    co = malloc(sizeof(Coordinate));

    if (!co)
        return NULL;
    assert(co);

    co->x = x;
    co->y = y;
    co->z = 0.0;

    return co;
}

p_Coordinate
Coordinate_CreateRandom2D(const int upper, const int lower)
{
    double x, y;

    if (
        Random_Double(upper, lower, &x) == ERROR ||
        Random_Double(upper, lower, &y) == ERROR
       )
        return NULL;

    return Coordinate_Create2D(x, y);
}

/* @fn
 * Create a 3D coordinate with given parameters.
 */
p_Coordinate
Coordinate_Create3D(const coordinate_t x, const coordinate_t y, 
                    const coordinate_t z)
{
    p_Coordinate co = malloc(sizeof(Coordinate));

    if (!co)
        return NULL;
    assert(co);

    co->x = x;
    co->y = y;
    co->z = z;

    return co;
}

void
Coordinate_Free(p_Coordinate* co)
{
    if (!co || !*co)
        return;
    free(*co);
    *co = NULL;
}

/* @fn
 * Get the x-coordinate of a coorinate, returning through 
 * parameter "x".
 */
Status
Coordinate_GetX(p_Coordinate co, coordinate_t* x)
{
    return get_coordinate(co, x_field, x);
}

/* @fn
 * Get the y-coordinate of a coorinate, returning through 
 * parameter "y".
 */
Status
Coordinate_GetY(p_Coordinate co, coordinate_t* y)
{
    return get_coordinate(co, y_field, y);
}

/* @fn
 * Get the z-coordinate of a coorinate, returning through 
 * parameter "z".
 */
Status
Coordinate_GetZ(p_Coordinate co, coordinate_t* z)
{
    return get_coordinate(co, z_field, z);
}

/* @fn
 * Get x-, y-, or z-coordinate according to parameters.
 */
static Status
get_coordinate(p_Coordinate co, const coor_field f, coordinate_t* re)
{
    if (!co || !re)
        return ERROR;

    switch (f) {
        case x_field:
            *re = co->x; break;
        case y_field:
            *re = co->y; break;
        case z_field:
            *re = co->z; break;
        default:
            return ERROR;
    }
    return OK;
}

/* @fn
 * Set the x-coordinate of "co" to "x".
 */
Status
Coordinate_SetX(p_Coordinate co, const coordinate_t x)
{
    return set_coordinate(co, x_field, x);
}

/* @fn
 * Set the y-coordinate of "co" to "y".
 */
Status
Coordinate_SetY(p_Coordinate co, const coordinate_t y)
{
    return set_coordinate(co, y_field, y);
}

/* @fn
 * Set the z-coordinate of "co" to "z".
 */
Status
Coordinate_SetZ(p_Coordinate co, const coordinate_t z)
{
    return set_coordinate(co, z_field, z);
}

/* @fn
 * Set x-, y-, or z-coordinate according to parameters.
 */
static Status
set_coordinate(p_Coordinate co, const coor_field f, const coordinate_t o)
{
    if (!co)
        return ERROR;

    switch (f) {
        case x_field:
            co->x = o; break;
        case y_field:
            co->y = o; break;
        case z_field:
            co->z = o; break;
        default:
            return ERROR;
    }
    return OK;
}

/* @fn
 * lhs = rhs
 */
Status
Coordinate_Assign(p_Coordinate lhs, p_Coordinate rhs)
{
    if (!lhs || !rhs)
        return ERROR;

    lhs->x = rhs->x;
    lhs->y = rhs->y;
    lhs->z = rhs->z;

    return OK;
}

/* @fn
 * Check whether two coordinates are identical. Be aware,
 * it's the responsibility of the caller to ensure the
 * given pointers to coordinate are not null. IF EITHER 
 * NULL, PROGRAM WILL HALT.
 */
Bool
Coordinate_Equal(p_Coordinate lhs, p_Coordinate rhs)
{
    assert(lhs);
    assert(rhs);

    if (
        Coordinate_XEqual(lhs, rhs) == TRUE &&
        Coordinate_YEqual(lhs, rhs) == TRUE &&
        Coordinate_ZEqual(lhs, rhs) == TRUE
       )
        return TRUE;
    return FALSE;
}

/* @fn 
 * Check whether two coordinates are different.
 */
Bool
Coordinate_NotEqual(p_Coordinate lhs, p_Coordinate rhs)
{
    if (Coordinate_Equal(lhs, rhs) == FALSE)
        return TRUE;
    return FALSE;
}

/* @fn 
 * Check whether the x-coordinates of two coordinates 
 * are equal.
 */
Bool
Coordinate_XEqual(p_Coordinate lhs, p_Coordinate rhs)
{
    return check_equality(lhs, rhs, x_field);
}

Bool
Coordinate_XNotEqual(p_Coordinate lhs, p_Coordinate rhs)
{
    if (Coordinate_XEqual(lhs, rhs) == FALSE)
        return TRUE;
    return FALSE;
}

/* @fn 
 * Check whether the y-coordinates of two coordinates 
 * are equal.
 */
Bool
Coordinate_YEqual(p_Coordinate lhs, p_Coordinate rhs)
{
    return check_equality(lhs, rhs, y_field);
}

Bool
Coordinate_YNotEqual(p_Coordinate lhs, p_Coordinate rhs)
{
    if (Coordinate_YEqual(lhs, rhs) == FALSE)
        return TRUE;
    return FALSE;
}

/* @fn 
 * Check whether the z-coordinates of two coordinates 
 * are equal.
 */
Bool
Coordinate_ZEqual(p_Coordinate lhs, p_Coordinate rhs)
{
    return check_equality(lhs, rhs, z_field);
}

Bool
Coordinate_ZNotEqual(p_Coordinate lhs, p_Coordinate rhs)
{
    if (Coordinate_ZEqual(lhs, rhs) == FALSE)
        return TRUE;
    return FALSE;
}

static Bool
check_equality(p_Coordinate lhs, p_Coordinate rhs, coor_field f)
{
    assert(lhs);
    assert(rhs);

    switch (f) {
        case x_field:
            if (lhs->x == rhs->x)
                return TRUE;
            return FALSE;
        case y_field:
            if (lhs->y == rhs->y)
                return TRUE;
            return FALSE;
        case z_field:
            if (lhs->z == rhs->z)
                return TRUE;
            return FALSE;
        default:
            return FALSE;
    }
}

/* @fn
 * Perform addition of two coordinates, and return result through
 * parameter "res".
 */
Status
Coordinate_Add(p_Coordinate lhs, p_Coordinate rhs, p_Coordinate res)
{
    if (!lhs || !rhs || !res)
        return ERROR;
    
    res->x = lhs->x + rhs->x;
    res->y = lhs->y + rhs->y;
    res->z = lhs->z + rhs->z;

    return OK;
}

/* @fn
 * Subtract "rhs" from "lhs" (i.e., lhs - rhs), and return result through
 * parameter "res".
 */
Status
Coordinate_Subtract(p_Coordinate lhs, p_Coordinate rhs, p_Coordinate res)
{
    if (!lhs || !rhs || !res)
        return ERROR;
    
    res->x = lhs->x - rhs->x;
    res->y = lhs->y - rhs->y;
    res->z = lhs->z - rhs->z;

    return OK;
}

/* @fn
 * Perform addition, and result is stored on "lhs" (i.e., lhs += rhs).
 */
Status
Coordinate_OnbodyAdd(p_Coordinate lhs, p_Coordinate rhs)
{
    if (!lhs || !rhs)
        return ERROR;

    lhs->x += rhs->x;
    lhs->y += rhs->y;
    lhs->z += rhs->z;

    return OK;
}

/* @fn
 * lhs -= rhs.
 */
Status
Coordinate_OnbodySubtract(p_Coordinate lhs, p_Coordinate rhs)
{
    if (!lhs || !rhs)
        return ERROR;

    lhs->x -= rhs->x;
    lhs->y -= rhs->y;
    lhs->z -= rhs->z;
}

/* @fn
 * Compute the Euclidean distance between two coordinates.
 */
coordinate_t
Coordinate_Distance(p_Coordinate c1, p_Coordinate c2)
{
    coordinate_t  x, y, z;

    assert(c1);
    assert(c2);

    x = fabs(c1->x - c2->x);
    y = fabs(c1->y - c2->y);
    z = fabs(c1->z - c2->z);

    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Status
Coordinate_2DPrint(p_Coordinate co, FILE* fp)
{
    if (!co || !fp)
        return ERROR;

    fprintf(fp, "(%4.2lf, %4.2lf)", co->x, co->y);

    return OK;
}

Status
Coordinate_3DPrint(p_Coordinate co, FILE* fp)
{
    if (!co || !fp)
        return ERROR;

    fprintf(fp, "(%4.2lf, %4.2lf, %4.2lf)", co->x, co->y, co->z);

    return OK;
}
