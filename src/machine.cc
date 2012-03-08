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

#include "exceptions.hh"
#include "xdg_model.hh"
#include "machine.hh"
#include "state.hh"

state * xdg_machine_next_state;
state * xdg_machine_curr_state;

void xdg_machine_turnkey() {
    xdg_machine_curr_state = xdg_machine_next_state;
}

void xdg_machine_process( char c ) {
    bool passchar = false;

    try {
        xdg_machine_curr_state->process(c);
    } catch ( incomplete_process * ex ) {
        /* If the machine can't fully processes the char,
           we must pass it to the next state */
        passchar = true;
    }

    if ( xdg_machine_curr_state != xdg_machine_next_state ) {
        /* a machine has requested a state transition.
           We can hanlde this the right (tm) way. */

        xdg_machine_curr_state->leave_state();
        xdg_machine_curr_state = xdg_machine_next_state;
        xdg_machine_curr_state->enter_state();

        /* All set to keep processing. Now, let's see
           if we have to process the next char. If we do,
           then let's recurse. */
        if ( passchar )
            xdg_machine_process(c);
    }
}

