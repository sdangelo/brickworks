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
 *  version {{{ 0.5.0 }}}
 *  requires {{{ bw_common bw_config }}}
 *  description {{{
 *    Note queue.
 *
 *    ...
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.5.0</strong>:
 *        <ul>
 *          <li>First release.</li>
 *        </ul>
 *      </li>
 *    </ul>
 *  }}}
 */

#ifndef _BW_NOTE_QUEUE_H
#define _BW_NOTE_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bw_common.h>

/*! api {{{
 *    #### bw_note_queue_status
 *  ```>>> */
typedef struct {
	char		pressed;
	float		velocity;	// negative = unknown / not available
} bw_note_queue_status;
/*! <<<```
 *    #### bw_note_queue_event
 *  ```>>> */
typedef struct {
	unsigned char		note;
	bw_note_queue_status	status;
	char			went_off;
} bw_note_queue_event;
/*! <<<```
 *    #### bw_note_queue
 *  ```>>> */
typedef struct {
	bw_note_queue_event	events[128];
	unsigned char		n_events;
	bw_note_queue_status	status[128];
	unsigned char		n_pressed;
} bw_note_queue;
/*! <<<```
 *    #### bw_note_queue_reset()
 *  ```>>> */
static inline void bw_note_queue_reset(bw_note_queue *BW_RESTRICT queue);
/*! <<<```
 *    #### bw_note_queue_clear()
 *  ```>>> */
static inline void bw_note_queue_clear(bw_note_queue *BW_RESTRICT queue);
/*! <<<```
 *    #### bw_note_queue_add()
 *  ```>>> */
static inline void bw_note_queue_add(bw_note_queue *BW_RESTRICT queue, unsigned char note, char pressed, float velocity, char force_went_off);

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

static inline void bw_note_queue_reset(bw_note_queue *BW_RESTRICT queue) {
	for (char i = 0; i < 128; i++)
		queue->status[i] = { 0, 0.f };
	queue->n_pressed = 0;
}

static inline void bw_note_queue_clear(bw_note_queue *BW_RESTRICT queue) {
	queue->n_events = 0;
}

static inline void bw_note_queue_add(bw_note_queue *BW_RESTRICT queue, unsigned char note, char pressed, float velocity, char force_went_off) {
	if (!pressed && !queue->status[note].pressed)
		return;

	unsigned char i;
	for (i = 0; i < queue->n_events; i++)
		if (queue->events[i].note == note)
			break;
	char went_off = 0;
	if (i == queue->n_events)
		queue->n_events++;
	else
		went_off = queue->events[i].went_off || queue->events[i].velocity <= 0.f;

	queue->events[i] = { note, { pressed, velocity }, went_off || force_went_off };
	if (pressed && !queue->status[note].pressed)
		queue->n_pressed++;
	else if (!pressed && queue->status[note].pressed)
		queue->n_pressed--;
	queue->status[note] = { pressed, velocity };
}

#ifdef __cplusplus
}
#endif

#endif
