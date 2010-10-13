/*
 * fbautostart.cpp
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

#include <iostream>
#include <vector>
#include <string>

#include <fbautostart.h>
#include "dot_desktop.cpp"

using namespace std;

int main ( int argc, char ** argv ) {
	std::cout << "Launching on behalf of " << _ON_BEHALF_OF << std::endl;
	std::vector<dot_desktop *> * files = loadDesktopFiles(getConfFiles(getConfDirs())); // XXX: This kinda sucks. Fix me if you have freetime.

	for ( unsigned int i = 0; i < files->size(); ++i ) {
		files->at(i)->load();  // I'm doing this so you can load on-demand
		//                        if you need to. I might change that later. ( Load, fork, load, fork )
	}

	for ( unsigned int i = 0; i < files->size(); ++i ) {
		dot_desktop * d = files->at(i);
		bool happy = true;

		// do only / not checks.

		std::string only = d->getAttr("OnlyShowIn");
		std::string noti = d->getAttr("NotShowIn");

		if ( only != "" ) {
			int index = -1;
			index = only.find(_ON_BEHALF_OF);
			if ( index < 0 ) {
				happy = false;
				debug("");
				debug("Not running the following app ( Excluded by a OnlyShowIn )");
				debug(d->getAttr("Name"));
			}
		}

		if ( noti != "" ) { // NAUGHTY NAUGHTY
			int index = -1;
			index = noti.find(_ON_BEHALF_OF);
			if ( index < 0 ) {
				happy = true;
				debug("");
				debug("Forced into running the following app ( Included by not being in NotShowIn )");
				debug(d->getAttr("Name"));
			}
		}

		if ( d->getAttr("Hidden") == "" && happy ) {
			std::string appl = d->getAttr("Exec");
			if ( appl != "" ) {
				std::cout << appl << std::endl;
			}

		} // otherwise, we're out of here.
	}

	return 0;
}



