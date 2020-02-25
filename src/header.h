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

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <errno.h>

/*
 * These two macros identifying a square deployment field:
 *
 *                          O-------------O    <--- upper right corner
 *                          |             |
 *                          |             |
 *                          |             |
 *                          |             |
 *                          |             |
 * lower left corner --->   O-------------O
 */
#define   UPPER_RIGHT     1000
#define   LOWER_LEFT      0

#define print_msg(str, var...) (printf(str "\n", ## var))
#define print_err(error) (printf("%s" "\n", strerror(error)))
#define err_exit(str, err_code) ({\
          printf(str "\n");\
          exit(-1);\
})
#define err_return(str, var...) ({\
          printf(str "\n", ## var);\
          return ERROR;\
})

typedef enum {
  ERROR = -1, OK
} Status;

typedef enum {
  FALSE, TRUE
} Bool;

#endif
