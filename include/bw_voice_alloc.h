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
 *  version {{{ 1.0.0 }}}
 *  requires {{{ bw_common bw_note_queue }}}
 *  description {{{
 *    Basic voice allocator with low/high note priority.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>1.0.0</strong>:
 *        <ul>
 *          <li>Specified that velocity can be negative in
 *              <code>bw_voice_alloc_opts</code>.</li>
 *          <li>Now using <code>size_t</code> instead of
 *              <code>BW_SIZE_T</code>.</li>
 *          <li>Added <code>const</code> and <code>BW_RESTRICT</code> where
 *              needed to callback types in
 *              <code>bw_voice_alloc_opts</code> and to
 *              <code>bw_voice_alloc()</code>.</li>
 *          <li>Removed usage of reserved identifiers.</li>
 *        </ul>
 *      </li>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
 *          <li>Now using <code>BW_SIZE_T</code> to count voices in
 *              <code>bw_voice_alloc()</code>.
 *          <li>Added debugging code.</li>
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

#ifndef BW_VOICE_ALLOC_H
#define BW_VOICE_ALLOC_H

#include <bw_common.h>
#include <bw_note_queue.h>

#ifdef __cplusplus
extern "C" {
#endif

/*** Public API ***/

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
	unsigned char (*get_note)(const void *BW_RESTRICT voice);
	char (*is_free)(const void *BW_RESTRICT voice);
} bw_voice_alloc_opts;
/*! <<<```
 *    Voice allocation options:
 *     * `priority`: note priority;
 *     * `note_on`: note on callback, where `voice` is an opaque pointer to the
 *       chosen voice, `note` is the note number, and `velocity` is the note
 *       velocity in [`0.f`, `1.f`] or otherwise negative to indicate unknown /
 *       not available;
 *     * `note_off`: note off callback, where `voice` is an opaque pointer to
 *       the chosen voice and `velocity` is the note velocity in [`0.f`, `1.f`]
 *       or otherwise negative to indicate unknown / not available;
 *     * `get_note`: callback that returns the note number associated to the
 *       given `voice`;
 *     * `is_free`: callback that returns whether the given `voice` is free
 *       (non-`0`) or not (`0`);
 *
 *    #### bw_voice_alloc()
 *  ```>>> */
void bw_voice_alloc(const bw_voice_alloc_opts *BW_RESTRICT opts, bw_note_queue *BW_RESTRICT queue, void *BW_RESTRICT const *BW_RESTRICT voices, size_t n_voices);
/*! <<<```
 *    It performs voice allocation according to `opts` and using the events in
 *    `queue`.
 *
 *    `voices` is the array of opaque voice pointers and `n_voices` indicates
 *    the number of elements in `voices`.
 *  }}} */

#ifdef __cplusplus
}
#endif

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

#ifdef __cplusplus
extern "C" {
#endif

void bw_voice_alloc(const bw_voice_alloc_opts *BW_RESTRICT opts, bw_note_queue *BW_RESTRICT queue, void *BW_RESTRICT const *BW_RESTRICT voices, size_t n_voices) {
	BW_ASSERT(opts != NULL);
	BW_ASSERT(opts->priority == bw_voice_alloc_priority_low || opts->priority == bw_voice_alloc_priority_high);
	BW_ASSERT(n_voices == 0 || opts->note_on != NULL);
	BW_ASSERT(n_voices == 0 || opts->note_off != NULL);
	BW_ASSERT(n_voices == 0 || opts->get_note != NULL);
	BW_ASSERT(n_voices == 0 || opts->is_free != NULL);
	BW_ASSERT(queue != NULL);
	BW_ASSERT_DEEP(bw_note_queue_is_valid(queue));
	BW_ASSERT(n_voices == 0 || voices != NULL);

	for (unsigned char i = 0; i < queue->n_events; i++) {
		bw_note_queue_event *ev = queue->events + i;
		bw_note_queue_status *st = queue->status + ev->note;

		for (size_t j = 0; j < n_voices; j++)
			if (!opts->is_free(voices[j]) && opts->get_note(voices[j]) == ev->note) {
				if (!st->pressed || ev->went_off)
					opts->note_off(voices[j], st->velocity);
				if (st->pressed)
					opts->note_on(voices[j], ev->note, st->velocity);
				goto next_event;
			}

		if (st->pressed) {
			for (size_t j = 0; j < n_voices; j++)
				if (opts->is_free(voices[j])) { 
					opts->note_on(voices[j], ev->note, st->velocity);
					goto next_event;
				}

			size_t k = n_voices;
			unsigned char v = ev->note;
			for (size_t j = 0; j < n_voices; j++) {
				unsigned char n = opts->get_note(voices[j]);
				if (!queue->status[n].pressed && (k == n_voices || (opts->priority == bw_voice_alloc_priority_low ? n > v : n < v))) {
					v = n;
					k = j;
				}
			}
			if (k == n_voices)
				for (size_t j = 0; j < n_voices; j++) {
					unsigned char n = opts->get_note(voices[j]);
					if (opts->priority == bw_voice_alloc_priority_low ? n > v : n < v) {
						v = n;
						k = j;
					}
				}
			if (k != n_voices)
				opts->note_on(voices[k], ev->note, st->velocity);
		}

		next_event:;
	}
}

#ifdef __cplusplus
}
#endif

#endif
