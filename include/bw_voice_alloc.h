/*
 * Brickworks
 *
 * Copyright (C) 2023 Orastron Srl unipersonale
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

#include <bw_note_queue.h>
#include <bw_voice.h>

typedef struct _bw_voice_alloc bw_voice_alloc;

void bw_voice_alloc_reset(bw_voice_alloc *voice_alloc);

void bw_voice_alloc_process(bw_voice_alloc *voice_alloc, bw_note_queue *queue);

void bw_voice_alloc_set_unison(bw_voice_alloc *voice_alloc, char value);

struct _bw_voice_alloc {
	bw_voice *voices;
	int n_voices;

	char unison;
};

void bw_voice_alloc_reset(bw_voice_alloc *voice_alloc) {
}

void bw_voice_alloc_process(bw_voice_alloc *voice_alloc, bw_note_queue *queue) {
}

void bw_voice_alloc_set_unison(bw_voice_alloc *voice_alloc, char value) {
	voice_alloc->unison = value;
}
