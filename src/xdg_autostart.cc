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
#include <iostream>

#include "xdg_spec.hh"
#include "xdg_model.hh"
#include "exceptions.hh"
#include "xdg_autostart.hh"

/**
 * XXX: Document me
 */
xdg_autostart_pair xdg_autostart_last_parsed() {
    return xdg_autostart_last_parsed("Desktop Entry");
}

/**
 * XXX: Document me
 */
xdg_autostart_pair xdg_autostart_last_parsed(std::string group) {
    xdg_model_entries::iterator entries = xdg_parsed_file.find(group);

    if (entries != xdg_parsed_file.end()) {
        // handle entries
        xdg_model_entry   asf = entries->second; // asf = AutoStartFile

        std::string      name  = asf["Name"];

        std::string  onlyshow  = asf["OnlyShowIn"];
        std::string   notshow  = asf["NotShowIn"];
        std::string   tryexec  = asf["TryExec"];
        std::string    hidden  = asf["Hidden"];
        std::string      exec  = asf["Exec"];


        bool notshowin  = (notshow.find( _xdg_window_manager) != std::string::npos);


        std::cout << "Now Serving: " << name << std::endl;

        if ( onlyshow != "" ) {
            bool onlyshowin = (onlyshow.find(_xdg_window_manager) != std::string::npos);
            if ( onlyshowin ) {
                std::cout << "  Attrinf:  We're the only DE that can handle this :3" << std::endl;
                std::cout << "  Attrinf:  " << onlyshow << std::endl;
            } else {
                std::cout << "  Attrinf:  We're not in the OnlyShowIn. We must not execute." << std::endl;
                std::cout << "  Attrinf:  " << onlyshow << std::endl;
                hidden = "true";
            }
        }

        if ( notshowin ) {
            std::cout << "  Attrinf:  We can't handle this item. Marking hidden :(" << std::endl;
            std::cout << "  Attrinf:  " << notshow << std::endl;
            hidden = "true";
        }

        if ( hidden == "true" ) {
            std::cout << "  Hidden:  " << hidden << std::endl;
            std::cout << "     +-> (To be clear, I'm stopping here)" << std::endl;
            xdg_autostart_pair ret = xdg_autostart_pair(name, "");
            return ret;
        } else {
            std::cout << "  Hidden:  " << hidden << std::endl;
            std::cout << "     +-> (To be clear, I'm going to continue parsing)" << std::endl;
        }

        std::string binary_name = "";

        if ( tryexec != "" ) {
            // std::cout << "  TryExec: " << tryexec << std::endl;
            binary_name = tryexec;
        } else if ( exec != "" ) {
            // std::cout << "  Exec:    " << exec << std::endl;
            binary_name = exec;
        } else {
            std::cout << "  No Exec directive!" << std::endl;
        }

        xdg_autostart_pair ret = xdg_autostart_pair(name, binary_name);

        return ret;
    } else {
        std::cerr << "Invalid .desktop file ( Can't find " <<
            group << " ). Ignoring." << std::endl;
        throw new parser_exception(-1,-1);
    }
}

