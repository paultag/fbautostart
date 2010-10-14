/*
 * dot_desktop.h
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

#ifndef _DOT_DESKTOP_H
#define _DOT_DESKTOP_H ohai

class dot_desktop_attr {
	public:
		std::string attr;
		std::string value;
};

class dot_desktop {
	protected:
		std::vector<dot_desktop_attr *> * attr;
		std::string file;
		std::string id;
	public:
		dot_desktop( std::string s );
		void load();
		std::string getAttr( std::string s );
		bool validate();
		std::string getFile();
};

#endif
