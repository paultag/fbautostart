/*
 * fbautostart.h
 * 
 * This file is part of the Fluxbox Autostart ( fbautostart )
 * Utility.
 *
 * Copyright(c) 2010 Paul Tagliamonte <paultag@ubuntu.com>
 *
 * This file may be licensed under the terms of of the
 * GNU General Public License Version 3 (the ``GPL'').
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the GPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the GPL along with this
 * program. If not, go to http://www.gnu.org/licenses/gpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef _FBAUTOSTART_H
#define _FBAUTOSTART_H ohai

#ifndef _DEBUG_MODE
#define _DEBUG_MODE 0
#endif

#ifndef _ON_BEHALF_OF
#define _ON_BEHALF_OF "FLUXBOX"
#endif


#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#include <algorithm>
#include <vector>
#include <cstdlib>
#include <string.h>
#include <stdio.h>

#include <sstream>

#include "dot_desktop.h"

/*
 * OK. Here's some Spec text. I accessed this data on
 * October 14th, 2010. 
 *
 * The URL was:
 * http://standards.freedesktop.org/autostart-spec/autostart-spec-latest.html
 *
 *
 * == Autostart Directories ==
 *
 * The Autostart Directories are $XDG_CONFIG_DIRS/autostart as
 * defined in accordance with the "Referencing this specification"
 * section in the "desktop base directory specification".
 *
 * If the same filename is located under multiple Autostart Directories
 * only the file under the most important directory should be used.
 *
 * Example: If $XDG_CONFIG_HOME is not set the Autostart Directory
 *          in the user's home directory is ~/.config/autostart/
 *
 * Example: If $XDG_CONFIG_DIRS is not set the system wide Autostart
 *          Directory is /etc/xdg/autostart/
 *
 * Example: If $XDG_CONFIG_HOME and $XDG_CONFIG_DIRS are not set and
 *          the two files /etc/xdg/autostart/foo.desktop and
 *          ~/.config/autostart/foo.desktop exist then only the file
 *          ~/.config/autostart/foo.desktop will be used because
 *          ~/.config/autostart/ is more important than /etc/xdg/autostart/
 */

#define _DEFAULT_XDG_HOME "~/.config"
#define _DEFAULT_XDG_DIRS "/etc/xdg"
#define _XDG_AUTOSTART_DIR "/autostart/"

void logError( std::string s ) { // [e]: my error line
	std::cerr << "[e]: " << s << std::endl;
}

void logError( int i ) { // [e]: (int) n
	std::cerr << "[e]: (int) " << i << std::endl;
}

void debug( const std::string & s ) {    // [l]: my debug line
	if ( _DEBUG_MODE ) {
		std::cout << "[l]: " << s << std::endl;
	}
}

void debug( const std::vector<std::string> & foo ) {    // [l]: my debug line
	if ( _DEBUG_MODE ) {
		std::cout << "[l]: The following is an array" << std::endl;
		for ( unsigned int i = 0; i < foo.size(); ++i ) {
			std::cout << "[l]:   " << foo.at(i) << std::endl;
		}
	}
}

void debug( size_t foo ) {    // [l]: my debug line
	if ( _DEBUG_MODE ) {
		std::cout << "[l]: " << foo << std::endl;
	}
}

void breakupLine( std::vector<std::string> * locs, std::string lines ) {
	std::string token;
	std::istringstream iss(lines);
	while ( getline(iss, token, ':') ) {
		token.append( _XDG_AUTOSTART_DIR ); // use the xdg autostart dir.
		locs->push_back( token );
	}
}

void fixHomePathing( std::vector<std::string> * locs, std::string home ) {
	for ( unsigned int i = 0; i < locs->size(); ++i ) {
		if ( locs->at(i)[0] == '~' && locs->at(i)[1] == '/' ) {
			debug("");
			debug("Fixing a path old / new path follows.");
			debug(locs->at(i));
			locs->at(i).replace(0, 1, home );
			debug(locs->at(i));
		}
	}
}

bool getConfDirs( std::vector<std::string> & loc ) {

	const char * xdg_home = getenv("XDG_CONFIG_HOME"); // See the spec for why
	const char * xdg_dirs = getenv("XDG_CONFIG_DIRS"); // I'm using what I'm using.
	const char * uzr_home = getenv("HOME");

	if ( uzr_home != NULL ) {

		std::string XDG_home, XDG_dirs;

		if ( xdg_dirs != NULL ) {
			debug( "We have an overridden xdg dir. Using it." );
			XDG_dirs = xdg_dirs;
		} else {
			XDG_home = _DEFAULT_XDG_DIRS;
		}

		if ( xdg_home != NULL ) {
			debug( "We have an overridden xdg home. Using it." );
			XDG_home = xdg_home;
		} else {
			XDG_home = _DEFAULT_XDG_HOME;
		}

		breakupLine( &loc, XDG_dirs );
		breakupLine( &loc, XDG_home );

		for ( unsigned int i = 0; i < loc.size(); ++i )
			fixHomePathing( &loc, uzr_home );

		debug("");
		debug("Using the following array to find files ( Expanded ): ");
		debug(loc);

		return true;
	} else {
		logError( "the env var HOME is not set. Panic!" );
		return false;
	}
}

bool getDesktopFiles(
	const std::vector<std::string> & dirs,
	std::vector<dot_desktop * >    & out_files
) {
	std::vector<dot_desktop *> files;

	for ( unsigned int i = 0; i < dirs.size(); ++i ) {
		DIR           * dp   = NULL;
		struct dirent * dirp = NULL;
		if ((dp = opendir(dirs.at(i).c_str())) == NULL ) {
			if ( errno == ENOENT ) { // E(RROR) NO EXIST ( or whatever )
				debug("");
				debug("Looks like the dir does not exist. Dir follows.");
				debug( dirs.at(i) );
				// debug("I'm going to keep this quiet. No big deal. Moving on.");
			} else {
				logError("");
				logError("Oh no! Error opening directory! Directory, then Errorno follows: ");
				logError( dirs.at(i) );
				logError(errno);
				return false;
			}
		} else {
			while ((dirp = readdir(dp)) != NULL) { // for every file in the directory
				std::string file(dirp->d_name); // char arrays suck
				if ( file != "." && file != ".."  ) { // make sure we don't use . / ..
					int dupe = -1; // there's no -1st element, silly!

					for ( unsigned int n = 0; n < files.size(); ++n ) {
						if ( files.at(n)->getID() == file ) { // make sure it's unique
						                                       // ( as the xdg 
						                                       //        spec requires )

							dupe = n; // there can be only one
							          // dupe in the array, so it's
							          // OK to think that there will
							          // be only one 
						}
					}

					std::string dees_nutz = dirs.at(i); // howabout deez nuts?
					dees_nutz.append(file); // there's a dick joke somewhere in here
					dot_desktop * new_file = new dot_desktop( dees_nutz, file );

					if ( dupe >= 0 ) {
						std::replace(
							files.begin(), files.end(),
							files.at(dupe), new_file
						);
					} else {
						files.push_back( new_file );
					}
				} 
			}
			closedir(dp);
		}
	}

	if ( ! files.empty() ) {
		std::swap( files, out_files );
		return true;
	} else {
		return false;
	}
}

#endif
