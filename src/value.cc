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

#include <string>
#include <iostream>

#include "key.hh"
#include "value.hh"
#include "entry.hh"
#include "group.hh"
#include "machine.hh"
#include "xdg_model.hh"

value xdg_value;

std::string xdg_state_value_lastparsed;

void value::leave_state() {

	std::string group = xdg_state_group_lastparsed;
	std::string key   = xdg_state_key_lastparsed;
	std::string value = xdg_state_value_lastparsed;

	xdg_model_entries::iterator iter = xdg_parsed_file.find(group);

	if (iter != xdg_parsed_file.end()) {
		xdg_model_entry *  group_map = &iter->second;
		xdg_model_entry_t  new_item( key, value );
		group_map->insert(new_item);
	} else {
		xdg_model_entry      group_map;
		xdg_model_entry_t    new_item( key, value );
		group_map.insert(new_item);
		xdg_model_entries_t  payload( group, group_map );
		xdg_parsed_file.insert(payload);
	}
}

void value::enter_state() {
	xdg_state_value_lastparsed = "";
}

void value::process( char c ) {
	switch ( c ) {
		case '\n':
			xdg_machine_next_state = &xdg_entry;
			break;
		default:
			/* push back char */
			xdg_state_value_lastparsed.append(1, c);
			break;
	}
}

