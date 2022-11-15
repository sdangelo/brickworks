/*
 * Brickworks
 *
 * Copyright (C) 2021, 2022 Orastron Srl unipersonale
 *
 * Brickworks is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * Brickworks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *
 * File author: Stefano D'Angelo
 */

#ifndef _WALLOC_H
#define _WALLOC_H

#include <stddef.h>

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif
