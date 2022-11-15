/*
 * Brickworks
 *
 * Copyright (C) 2022 Orastron Srl unipersonale
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

#include <bw_rand.h>

uint32_t bw_rand_u32(uint64_t *state) {
	// Permuted Congruential Generator,
	// taken from https://nullprogram.com/blog/2017/09/21/
	*state = *state * 0x9b60933458e17d7d + 0xd737232eeccdf7ed;
	return *state >> (29 - (*state >> 61));
}

float bw_rand_f(uint64_t *state) {
	return  (2.f / (float)UINT32_MAX) * (float)bw_random_u32(state) - 1.f;
}
