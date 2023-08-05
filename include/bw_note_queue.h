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
 *  requires {{{ bw_common }}}
 *  description {{{
 *    Simple data structure that helps keeping track of note on/off events and
 *    pressed key status.
 *
 *    It is not concerned with timing.
 *  }}}
 *  changelog {{{
 *    <ul>
 *      <li>Version <strong>0.6.0</strong>:
 *        <ul>
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
 *    Note status:
 *     * `pressed`: whether the note is pressed (non-`0`) or not (`0`);
 *     * `velocity`: velocity in [`0.f`, `1.f`].
 *
 *    #### bw_note_queue_event
 *  ```>>> */
typedef struct {
	unsigned char		note;
	bw_note_queue_status	status;
	char			went_off;
} bw_note_queue_event;
/*! <<<```
 *    Note on/off event:
 *     * `note`: note number in [`0`, `127`];
 *     * `status`: note status;
 *     * `went_off`: whether a note off event fired on the same note (non-`0`)
 *       or not (`0`) -- see `bw_note_queue`.
 *
 *    #### bw_note_queue
 *  ```>>> */
typedef struct {
	bw_note_queue_event	events[128];
	unsigned char		n_events;
	bw_note_queue_status	status[128];
	unsigned char		n_pressed;
} bw_note_queue;
/*! <<<```
 *    Note on/off event queue and pressed key status:
 *     * `events`: events since the reset/clear -- the order is not meaningful
 *       and it contains maximum one event per note number, so that the last
 *       event added for a given note overwrites the previous if it exists;
 *       `went_off` is set to non-`0` in case of a note off event or when
 *       overwriting an event whose `went_off` was already non-`0`;
 *     * `n_events`: number of elements in `events`;
 *     * `status`: current status of all notes;
 *     * `n_pressed`: number of currently pressed keys.
 *
 *    #### bw_note_queue_reset()
 *  ```>>> */
static inline void bw_note_queue_reset(bw_note_queue *BW_RESTRICT queue);
/*! <<<```
 *    Clear both the event queue (no events) and the note statuses (all notes
 *    off, all velocities `0.f`) in `queue`.
 *
 *    #### bw_note_queue_clear()
 *  ```>>> */
static inline void bw_note_queue_clear(bw_note_queue *BW_RESTRICT queue);
/*! <<<```
 *    Clears the event queue (no events) in `queue` without affecting the note
 *    statuses.
 *
 *    #### bw_note_queue_add()
 *  ```>>> */
static inline void bw_note_queue_add(bw_note_queue *BW_RESTRICT queue, unsigned char note, char pressed, float velocity, char force_went_off);
/*! <<<```
 *    Adds a new event to `queue` with the specified `note` number, `pressed`
 *    value, and `velocity`.
 *
 *    If `force_went_off` is set to non-`0`, `went_off` is always set to
 *    non-`0`.
 *
 *    #### bw_note_queue_add()
 *  ```>>> */
static inline char bw_note_queue_is_valid(const bw_note_queue *BW_RESTRICT queue);
/*! <<<```
 *    Tries to determine whether `queue` is valid and returns non-`0` if it
 *    seems to be the case and `0` if it is certainly not. False positives are
 *    possible, false negatives are not.
 *
 *    `queue` must at least point to a readable memory block of size greater
 *    than or equal to that of `bw_note_queue`.
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

static inline void bw_note_queue_reset(bw_note_queue *BW_RESTRICT queue) {
	BW_ASSERT(queue != NULL);
	for (int i = 0; i < 128; i++)
		queue->status[i] = (bw_note_queue_status){ .pressed = 0, .velocity = 0.f };
	queue->n_pressed = 0;
	queue->n_events = 0;
	BW_ASSERT_DEEP(bw_note_queue_is_valid(queue));
}

static inline void bw_note_queue_clear(bw_note_queue *BW_RESTRICT queue) {
	BW_ASSERT(queue != NULL);
	BW_ASSERT_DEEP(bw_note_queue_is_valid(queue));
	queue->n_events = 0;
}

static inline void bw_note_queue_add(bw_note_queue *BW_RESTRICT queue, unsigned char note, char pressed, float velocity, char force_went_off) {
	BW_ASSERT(queue != NULL);
	BW_ASSERT_DEEP(bw_note_queue_is_valid(queue));
	BW_ASSERT(note < 128);
	BW_ASSERT(bw_is_finite(velocity) && velocity <= 1.f);

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
		went_off = queue->events[i].went_off || !queue->events[i].status.pressed;

	queue->events[i] = (bw_note_queue_event){ .note = note, .status = { pressed, velocity }, .went_off = went_off || force_went_off };
	if (pressed && !queue->status[note].pressed)
		queue->n_pressed++;
	else if (!pressed && queue->status[note].pressed)
		queue->n_pressed--;
	queue->status[note] = (bw_note_queue_status){ .pressed = pressed, .velocity = velocity };

	BW_ASSERT_DEEP(bw_note_queue_is_valid(queue));
}

static inline char bw_note_queue_is_valid(const bw_note_queue *BW_RESTRICT queue) {
	BW_ASSERT(queue != NULL);

	if (queue->n_events >= 128 || queue->n_pressed >= 128)
		return 0;

	for (int i = 0; i < (int)queue->n_events; i++) {
		const bw_note_queue_event *ev = queue->events + i;
		if (ev->note >= 128 || !bw_is_finite(ev->status.velocity) || ev->status.velocity > 1.f)
			return 0;
		for (int j = 0; j < i; j++) {
			const bw_note_queue_event *ev2 = queue->events + j;
			if (ev2->note == ev->note)
				return 0;
		}
	}

	int cnt = 0;
	for (int i = 0; i < 128; i++) {
		const bw_note_queue_status *st = queue->status + i;
		if (st->pressed)
			cnt++;
		if (!bw_is_finite(st->velocity) || st->velocity > 1.f)
			return 0;
	}

	return cnt == queue->n_pressed;
}

#ifdef __cplusplus
}
#endif

#endif
