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

#ifndef _DEBUG_MODE

#define _DEBUG_MODE 0

#endif

#include <vector>
#include <cstdlib>
#include <string.h>
#include <stdio.h>

#include <sstream>

void logError( std::string s ) { // [e]: my error line
	std::cout << "[e]: " << s << std::endl;
}

void debug( std::string s ) {    // [l]: my debug line
	if ( _DEBUG_MODE ) {
		std::cerr << "[l]: " << s << std::endl;
	}
}

void debug( std::vector<std::string> * foo ) {    // [l]: my debug line
	if ( _DEBUG_MODE ) {
		std::cerr << "[l]: The following is an array" << std::endl;
		for ( unsigned int i = 0; i < foo->size(); ++i ) {
			std::cerr << "[l]:   " << foo->at(i) << std::endl;
		}
	}
}

void breakupLine( std::vector<std::string> * locs, std::string lines ) {
	std::string token;
	std::istringstream iss(lines);
	while ( getline(iss, token, ':') ) {
		locs->push_back( token );
	}
}

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

#define _DEFAULT_XDG_HOME "~/.config/autostart/"
#define _DEFAULT_XDG_DIRS "/etc/xdg/autostart/"

std::vector<std::string> * getConfFiles() {
	std::vector<std::string> * ret = new std::vector<std::string>(); // files to process
	std::vector<std::string> * loc = new std::vector<std::string>(); // locations to look

	const char * xdg_home = getenv("XDG_CONFIG_HOME");
	const char * xdg_dirs = getenv("XDG_CONFIG_DIRS");
	const char * uzr_home = getenv("HOME");

	if ( uzr_home != NULL ) {
		if ( xdg_dirs != NULL ) {
			debug( "We have an overridden xdg dir. Using it." );
		} else {
			xdg_home = _DEFAULT_XDG_DIRS;
		}

		if ( xdg_home != NULL ) {
			debug( "We have an overridden xdg home. Using it." );
		} else {
			xdg_home = _DEFAULT_XDG_HOME;
		}

		debug("");
		debug( "XDG Home: " );
		debug( xdg_home );
		debug("");

		debug( "XDG Dirs: " );
		debug( xdg_dirs );
		debug("");

		debug( "User Home: " );
		debug( uzr_home );
		debug("");

		breakupLine( loc, std::string( xdg_dirs ) );
		breakupLine( loc, std::string( xdg_home ) );
		debug("Using the following array to find files:");
		debug(loc);

		return ret;
	} else {
		logError( "the env var HOME is not set. Panic!" );
		return NULL;
	}
}

