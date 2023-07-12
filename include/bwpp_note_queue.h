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

#ifndef BWPP_NOTE_QUEUE_H
#define BWPP_NOTE_QUEUE_H

#include <bw_note_queue.h>

namespace Brickworks {

/*! api {{{
 *    ##### Brickworks::NoteQueue
 *  ```>>> */
class NoteQueue {
public:
	NoteQueue();
	
	void clear();
	void add(unsigned char note, bool pressed, float velocity, bool force_went_off);
	
	bw_note_queue	queue;
};
/*! <<<```
 *  }}} */

/*** Implementation ***/

/* WARNING: This part of the file is not part of the public API. Its content may
 * change at any time in future versions. Please, do not use it directly. */

inline NoteQueue::NoteQueue() {
	bw_note_queue_reset(&queue);
}

inline void NoteQueue::clear() {
	bw_note_queue_clear(&queue);
}

inline void NoteQueue::add(unsigned char note, bool pressed, float velocity, bool force_went_off) {
	bw_note_queue_add(&queue, note, pressed, velocity, force_went_off);
}

}

#endif
