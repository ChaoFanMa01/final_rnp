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

#include <stdlib.h>

#include "random.h"

static Status random_double(int, int, double*);

/* @fn
 * Generate a random number of double type between 
 * "upper" and "lower" bounds, and return this number
 * through parameter "re".
 */
Status
Random_Double(int upper, int lower, double* re)
{
    return random_double(upper, lower, re);
}

static Status
random_double(int upper, int lower, double* re)
{
    unsigned int     integer, offset;
    double           frac;

    if (lower >= upper)
        return ERROR;

    offset = upper - lower;

    frac    = (double)(rand() % 100) / 100;
    integer = rand() % offset;

    *re = (double)integer + frac + (double)lower;

    return OK;
}
