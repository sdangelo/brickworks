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

/*!
 *  module_type {{{ utility }}}
 *  version {{{ 0.6.0 }}}
 *  requires {{{ bw_common bw_note_queue }}}
 *  description {{{
 *    Basic voice allocator with low/high note priority.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Removed dependency on bw_config.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_VOICE_ALLOC_H
#define _BW_VOICE_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>
#include <bw_note_queue.h>

/*! api {{{
 *    #### bw_voice_alloc_priority
 *  ```>>> */
typedef enum {
	bw_voice_alloc_priority_low,
	bw_voice_alloc_priority_high
} bw_voice_alloc_priority;
/*! <<<```
 *    Note priority:
 *     * `bw_voice_alloc_priority_low`: low note priority;
 *     * `bw_voice_alloc_priority_high`: high note priority.
 *
 *    #### bw_voice_alloc_opts
 *  ```>>> */
typedef struct {
	bw_voice_alloc_priority	priority;

	void (*note_on)(void *BW_RESTRICT voice, unsigned char note, float velocity);
	void (*note_off)(void *BW_RESTRICT voice, float velocity);
	unsigned char (*get_note)(void *BW_RESTRICT voice);
	char (*is_free)(void *BW_RESTRICT voice);
} bw_voice_alloc_opts;
/*! <<<```
 *    Voice allocation options:
 *     * `priority`: note priority;
 *     * `note_on`: note on callback, where `voice` is an opaque pointer to the
 *       chosen voice, `note` is the note number, and `velocity` is the note
 *       velocity in [`0.f`, `1.f`];
 *     * `note_off`: note off callback, where `voice` is an opaque pointer to
 *       the chosen voice and `velocity` is the note velocity in [`0.f`, `1.f`];
 *     * `get_note`: callback that returns the note number associated to the
 *       given `voice`;
 *     * `is_free`: callback that returns whether the given `voice` is free
 *       (non-`0`) or not (`0`);
 *
 *    #### bw_voice_alloc()
 *  ```>>> */
void bw_voice_alloc(const bw_voice_alloc_opts *BW_RESTRICT opts, bw_note_queue *BW_RESTRICT queue, void **BW_RESTRICT voices, int n_voices);
/*! <<<```
 *    It performs voice allocation according to `opts` and using the events in
 *    `queue`.
 *
 *    `voices` is the array of opaque voice pointers and `n_voices` indicates
 *    the number of elements in `voices`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

void bw_voice_alloc(const bw_voice_alloc_opts *BW_RESTRICT opts, bw_note_queue *BW_RESTRICT queue, void **BW_RESTRICT voices, int n_voices) {
	for (unsigned char i = 0; i < queue->n_events; i++) {
		bw_note_queue_event *ev = queue->events + i;
		for (int j = 0; j < n_voices; j++)
			if (!opts->is_free(voices[j]) && opts->get_note(voices[j]) == ev->note) {
				if (!ev->status.pressed || ev->went_off)
					opts->note_off(voices[j], ev->status.velocity);
				if (ev->status.pressed)
					opts->note_on(voices[j], ev->note, ev->status.velocity);
				goto next_event;
			}

		if (ev->status.pressed) {
			for (int j = 0; j < n_voices; j++)
				if (opts->is_free(voices[j])) { 
					opts->note_on(voices[j], ev->note, ev->status.velocity);
					goto next_event;
				}

			int k = -1;
			int v = ev->note;
			for (int j = 0; j < n_voices; j++) {
				int n = opts->get_note(voices[j]);
				if (!queue->status[n].pressed && (k < 0 || (opts->priority == bw_voice_alloc_priority_low ? n > v : n < v))) {
					v = n;
					k = j;
				}
			}
			if (k >= 0) {
				opts->note_on(voices[k], ev->note, ev->status.velocity);
				continue;
			}

			for (int j = 0; j < n_voices; j++) {
				int n = opts->get_note(voices[j]);
				if (opts->priority == bw_voice_alloc_priority_low ? n > v : n < v) {
					v = n;
					k = j;
				}
			}
			if (k >= 0)
				opts->note_on(voices[k], ev->note, ev->status.velocity);
		}

		next_event:;
	}
}

#ifdef __cplusplus
}
#endif

#endif
