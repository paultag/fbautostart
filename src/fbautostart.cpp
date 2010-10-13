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

	return 0;
}
