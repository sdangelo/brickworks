/*
 * Brickworks
 *
 * Copyright (C) 2024 Orastron Srl unipersonale
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
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void * impl;

impl impl_new(void);
void impl_free(impl handle);
void impl_set_sample_rate(impl handle, float sample_rate);
void impl_reset(impl handle);
void impl_set_parameter(impl handle, size_t index, float value);
float impl_get_parameter(impl handle, size_t index);
void impl_process(impl handle, const float ** inputs, float ** outputs, size_t n_samples);
void impl_midi_msg_in(impl handle, size_t index, const uint8_t * data);

#ifdef __cplusplus
}
#endif
