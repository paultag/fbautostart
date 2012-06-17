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
 * Use the internal state-machine implementation to process a file into the
 * internal datastructure's format.
 *
 * @param file file to parse
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
 * Check to see if a string ends with another char sequence.
 *
 * @param source string to compare against
 * @param cmp string to attempt to find at the end of the source string.
 * @return if `cmp` is the ending sequence to `source`.
 */
bool ends_with( char * source, const char * cmp ) {
    int s_len = strlen(source);
    int s_cmp = strlen(cmp);
    if ( s_len < s_cmp )
        return false;
    for ( int i = s_cmp; i > 0; --i ) {
        int offs1 = (s_len - i);
        int offs2 = (s_cmp - i);
        if ( source[offs1] != cmp[offs2] )
            return false;
    }
    return true;
}

/**
 * Attempt to process a directory full of files into a datastructure.
 *
 * @warning this will ignore *all* files that *do not* end with ".desktop".
 *
 * @param binaries data structure to map the directory to
 * @param folder folder on the filesystem to process
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
                    strcmp(ent->d_name, "..") != 0 &&
                    ends_with(ent->d_name, ".desktop")
               ) {
                // std::cout << "Loading: " << folder + "/" + ent->d_name << " ";
                try {
                    parse_file(folder + "/" + ent->d_name);
                    xdg_autostart_pair r = xdg_autostart_last_parsed();
                    xdg_parsed_file.clear();
                    xdg_autostart_map::iterator it = binaries->find(ent->d_name);
                    if ( it != binaries->end() ) {
                        binaries->erase(it);
                    }
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

