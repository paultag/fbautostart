/*
 * Copyright (C) 2011, Paul Tagliamonte
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>

#include "key.hh"
#include "entry.hh"

#include "group.hh"
#include "comment.hh"
#include "invalid.hh"
#include "machine.hh"
#include "exceptions.hh"

entry xdg_entry;

void entry::leave_state() {}
void entry::enter_state() {}

void entry::process( char c ) {
	switch ( c ) {
		case '#':
			/* Lines that begin with # are comments*/
			xdg_machine_next_state = &xdg_comment;
			break;
		case '=':
			/* We've got no key, this is an invalid transition */
			xdg_machine_next_state = &xdg_invalid;
			break;
		case '[':
			/* Lines that begin with [ are groups */
			xdg_machine_next_state = &xdg_group;
			break;
		case '\n':
			/* Let's just eat newlines */
			break;
		default:
			/* Anything else is a key */
			xdg_machine_next_state = &xdg_key;
			throw new incomplete_process();
			break;
	}
}

