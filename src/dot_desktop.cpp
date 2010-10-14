/*
 * dot_desktop.cpp
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

/*
 * BIG BOLDFACE WARNING:
 *  BIG BOLDFACE WARNING:
 *   BIG BOLDFACE WARNING:
 *
 * This is *NOT* a full implementation of the
 * .desktop spec. If you decide to use this code
 * for anything outside of basic basic use ( read:
 * what we're using it for )
 *
 * In fact, this is almost as bare bones as I can make it
 * It will break on more then one directive, and barf to stderr
 * when it finds something it thinks is disgusting.
 * 
 * It also does not trim strings, so there may be whitespace crap
 * screwing stuff up.
 *
 * This needs some love. Patches welcome. Don't suggest
 * the GNOME library. That relies on GTK+. WTF, really GNOME.
 *   -- PRT
 * 
 */

#define _DOT_DESKTOP_GIVEAWAY "[Desktop Entry]"

#include <dot_desktop.h>
#include <iostream>
#include <fstream>

#include <fbautostart.h>

dot_desktop::dot_desktop( std::string s ) {
	this->attr = new std::vector<dot_desktop_attr *>();
	this->file = s;
}

bool dot_desktop::validate() {
	std::ifstream fp_in(this->file.c_str());
	std::string line;
	bool ret = false;
	bool gah = true;
	while ( gah && std::getline(fp_in, line) ) { // XXX: This sucks. Fix me.
		if ( line.substr(0,strlen(_DOT_DESKTOP_GIVEAWAY)) == _DOT_DESKTOP_GIVEAWAY ) {
			// debug("");
			// debug("Found us a Desktop file!");
			ret = true;
			gah = false;
		} else if ( line.substr(0,1) == "#" || line.substr(0,1) == "" ) {  // XXX: This sucks. Fix me.
			// ok. comment or whitespace
		} else {
			ret = false;
			gah = false;

			debug("");
			debug("Looks like the following file is not a desktop file.");
			debug(this->file);
			debug("The giveaway was:");
			debug(line);
		}
	}
	fp_in.close();
	return ret;
}

void dot_desktop::load() {
	if ( this->validate() ) {
		std::ifstream fp_in(this->file.c_str());
		std::string line;

		while ( std::getline(fp_in, line) ) {
			int index = -1;
			if ( line.substr(0,1) != "#" ) {
				index = line.find("=");
				if ( index > 0 ) {
					std::string id   = line.substr(0,index);
					std::string data = line.substr(index+1,strlen(line.c_str()));
					dot_desktop_attr * new_attr = new dot_desktop_attr;
					new_attr->attr  = id;
					new_attr->value = data;
					this->attr->push_back( new_attr );
				} else if ( line.substr(0,1) == "[" && line.substr(line.length()-1,line.length()) == "]" ) {
					// debug("Found a new header:"); // XXX: This sucks. Fix me.
					// debug(line);
				} else if ( line == "" ) {
					// just a blank line.
				} else {
					logError("");
					logError( "We've got an issue with the following file:" );
					logError( this->file );
					logError( "Line in question is: " );
					logError( line );
				}
			}
		}
		fp_in.close();
	} else {
		logError("");
		logError( "We've got an issue with the following file:" );
		logError( this->file );
	}

	// debug("");
	// debug("Loaded up a file. Here's the name and count of attrbs.");
	// debug(this->file);
	// debug(this->attr->size());
}

std::string dot_desktop::getFile() {
	return this->file;
}

std::string dot_desktop::getAttr( std::string s ){
	for ( unsigned int i = 0; i < this->attr->size(); ++i ) {
		if ( this->attr->at(i)->attr == s )
			return this->attr->at(i)->value;
	}
	return "";
}
