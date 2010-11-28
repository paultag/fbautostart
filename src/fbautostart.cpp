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

#include "fbautostart.h"
#include "dot_desktop.cpp"

#define _APPNAME_ "fbautostart"
#define _VERSION_ "2.71828"

using namespace std;

bool noexec = false;

void version() {
	std::cout << "" << _APPNAME_ << " " << _VERSION_ << std::endl;
}

void lecture() {
	version();
	std::cout << "Copyright (C) 2010 Paul Tagliamonte" << std::endl;
	std::cout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>" << std::endl;
	std::cout << "This is free software: you are free to change and redistribute it." << std::endl;
	std::cout << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
}

void help() {
	std::cout << "Usage: fbautostart [OPTION] ... " << std::endl;
	std::cout << "Startup all applications according to the" << std::endl;
	std::cout << "XDG Spec. Right now, I'm hardcoded to think that" << std::endl;
	std::cout << "I am doing this on behalf of '" << _ON_BEHALF_OF << "'" << std::endl;

	std::cout << std::endl;

	std::cout << "--license             Print out license information" << std::endl;
	std::cout << "--help                Print out this message" << std::endl;
	std::cout << "--noexec              Don't exec, just do a dry run" << std::endl;
	std::cout << "--window-manager ID   Override hardcoded value ( " << _ON_BEHALF_OF <<  " )" << std::endl;

	std::cout << "" << std::endl;

	std::cout << "Copyleft (c) Paul Tagliamonte, 2010, GNU GPLv3+" << std::endl;

	std::cout << "" << std::endl;
}

int runCommand( std::string appl ) {
	/*
	 * The following is almost directly ripped off from
	 * Fluxbox's implementation. ( at ak|ra's behest )
	 */

	pid_t pid = fork();
	if (pid)
		return pid;

	// get shell path from the environment
	// this process exits immediately, so we don't have to worry about memleaks
	const char *shell = getenv("SHELL");

	if ( !shell )
		shell = "/bin/sh";

	if ( ! noexec ) { // we'll do it live
		execl( shell, shell, "-c", appl.c_str(), static_cast<void*>(NULL) );
		exit ( EXIT_SUCCESS );
		return pid; // compiler happy -> we are happy ;)
	} else { // dummy mode ( test )
		std::cout << "Would have run: " << appl << std::endl;
		exit(0);
		return 0;
	}
}

void processArgs( int argc, char ** args ) {
	for ( int i = 0; i < argc; ++i ) {
		if ( strcmp(args[i], "--license") == 0 ) {
			lecture();
			exit(0);
		} else if ( strcmp(args[i], "--version") == 0 ) {
			version();
			exit(0);
		} else if ( strcmp(args[i], "--help") == 0 ) {
			help();
			exit(0);
		} else if ( strcmp(args[i], "--noexec") == 0 ) {
			noexec = true;
		} else if ( strcmp(args[i], "--window-manager") == 0 ) {
			_ON_BEHALF_OF = args[i+1];
			i = i+1;
		} else {
			help();
			exit(1);
		}
	}
}

int main ( int argc, char ** argv ) {
	processArgs( argc, argv );

	if ( noexec ) {
		std::cout << "Warning: In noexec mode." << std::endl;
	}

	std::cout << "Launching on behalf of " << _ON_BEHALF_OF << std::endl;

	std::vector<dot_desktop>   files;
	std::vector<std::string>   dirs;

	if ( getConfDirs( dirs ) ) { // if no directories barf in our face
		if ( getDesktopFiles( dirs, files ) ) { // and we load everything with glee
			for ( unsigned int i = 0; i < files.size(); ++i ) { // run through all the files
				dot_desktop d = files.at(i);
				bool happy = true;
				std::string only = d.getAttr("OnlyShowIn"); // Only one per file ( per xdg )
				std::string noti = d.getAttr("NotShowIn");  // We'll ignore that until we care
				                                             // XXX: This is most likely a bug.
				if ( only != "" ) { // even if an attr does not exist
				                    // the object will return it as "".
					int index = -1;
					index = only.find(_ON_BEHALF_OF); // if we have our WM in the OnlyLaunch
					if ( index < 0 ) { // we're disabled ( not found )
						happy = false;
						debug("");
						debug("Not running the following app ( Excluded by a OnlyShowIn )");
						debug(d.getAttr("Name"));
					}
				}
				if ( noti != "" ) { // NAUGHTY NAUGHTY
					int index = -1;
					index = noti.find(_ON_BEHALF_OF); // if we have found our WM
					if ( index >= 0 ) { // We're in Launch, stop from launching it.
						happy = false;
						debug("");
						debug("Forced into not running the following app ( Included by not being in NotShowIn )");
						debug(d.getAttr("Name"));
					}
				}
				if ( d.getAttr("Hidden") == "" && happy ) { // If we sould exec
					std::string appl = d.getAttr("Exec"); // get the line to run
					if ( appl != "" ) { // if it's defined and ready to go
						debug( "Processing File: ");
						debug(d.getFile());
						runCommand( appl ); // kickoff
					}
				} // otherwise, we're out of here.
			}
			return 0;
		}
		return 0xDEADBEEF;
	}
	return 0xCAFEBABE;
}

