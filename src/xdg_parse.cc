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

#include <vector>
#include <fstream>
#include <iostream>
#include <dirent.h>

#include <string.h>

#include "machine.hh"
#include "entry.hh"
#include "xdg_model.hh"
#include "exceptions.hh"
#include "xdg_autostart.hh"

/**
 * XXX: Document me
 */
void parse_file ( std::string file ) {
    std::ifstream fd(file.c_str(), std::ios::in | std::ios::binary);

    if ( ! fd ) {
        std::cerr << "Crapola file." << std::endl;
        // XXX: Raise exception
    }

    xdg_machine_next_state = &xdg_entry;
    xdg_machine_turnkey(); // Vvvrooom!

    char c;

    int row = 1;
    int col = 0; // some cosmetics

    do {
        fd.read(&c, 1);
        col++;

        if ( c == '\n' ) { col = 0; row++; }

        try {
            xdg_machine_process( c );
        } catch ( parser_exception * fu ) {
            parser_exception ex(row, col);
            std::cerr << "Error parsing: R/C: " << row << ", " << col << std::endl;
            throw &ex; // XXX: Fix this fucking hack
        }
    } while ( ! fd.eof() );
    /* In the event that they don't newline the end of the
       file */
    xdg_machine_process( '\n' );
}

/**
 * XXX: Document me
 */
void parse_folder( xdg_autostart_map * binaries, std::string folder ) {
    DIR * dir;
    struct dirent * ent;
    dir = opendir(folder.c_str());

    if (dir != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {
            if (
                    strcmp(ent->d_name, ".")  != 0 &&
                    strcmp(ent->d_name, "..") != 0
                    /* XXX: Check to make sure it's actually a .desktop */
               ) {
                // std::cout << "Loading: " << folder + "/" + ent->d_name << " ";
                try {
                    parse_file(folder + "/" + ent->d_name);
                    xdg_autostart_pair r = xdg_autostart_last_parsed();
                    xdg_parsed_file.clear();
                    binaries->insert(xdg_autostart_pair(ent->d_name, r.second));
                } catch ( parser_exception * ex ) {
                    std::cerr << "Exception parsing " << ent->d_name << std::endl;
                    std::cerr << " in " << folder << std::endl;
                }
            }
        }
        closedir (dir);
    } else {
        // XXX: Exceptionize this
    }
}

