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

#include <dot_desktop.h>


dot_desktop::dot_desktop( std::string s ) {
	this->attr = new std::vector<dot_desktop_attr *>();
}

void dot_desktop::load() {
	// pull file data in. set as dot_desktop_attr objects
}

std::string dot_desktop::getAttr( std::string s ){
	// get off the vector
}