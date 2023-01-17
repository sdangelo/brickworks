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
 * along with Brickworks.  If not, see <http://www.gnu.org/licenses/>.
 *
 * File author: Stefano D'Angelo
 */

#include <stddef.h>

void *memset(void *ptr, int value, size_t num) {
	unsigned char *p = (unsigned char *)ptr;
	for (size_t i = 0; i < num; i++)
		p[i] = (unsigned char)value;
	return ptr;
}
