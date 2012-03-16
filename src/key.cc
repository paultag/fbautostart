/*
 * Copyright (C) 2011, Paul Tagliamonte <paultag@fluxbox.org>
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

#include "key.hh"
#include "value.hh"
#include "invalid.hh"
#include "machine.hh"

key xdg_key;

std::string xdg_state_key_lastparsed;

void key::leave_state() { }
void key::enter_state() {
    xdg_state_key_lastparsed = "";
}

void key::process( char c ) {
    switch ( c ) {
        case '=':
            /* Once we hit a =, we grab values */
            xdg_machine_next_state = &xdg_value;
            break;
        case '\n':
            /* If we hit a newline with now =, it's invalid */
            xdg_machine_next_state = &xdg_invalid;
            break;
        default:
            /* push back char */
            xdg_state_key_lastparsed.append(1, c);
            break;
    }
}

