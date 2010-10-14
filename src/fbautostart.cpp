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
#define _VERSION_ "2.7"

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
	std::cout << "--license           Print out license information" << std::endl;
	std::cout << "--help              Print out this message" << std::endl;
	std::cout << "--noexec            Don't exec, just do a dry run" << std::endl;
	std::cout << "" << std::endl;

	std::cout << "Copyleft (c) Paul Tagliamonte, 2010, GNU GPLv3+" << std::endl;
	std::cout << "" << std::endl;
}

void runApplication( std::string appl ) {
	std::cout << "Doing: " << appl << " " << std::endl;
	pid_t pID = fork();

	if (pID == 0) {
		if ( ! noexec ) {
			exit(system( appl.c_str() ));
		} else {
			std::cout << "Would have run: " << appl << std::endl;
		}
		exit(1);
	} else if (pID < 0) {
		logError( "" );
		logError( "Failed to fork " );
		logError( appl );
		exit(1);
	}

	return;
}

void processArgs( int argc, char ** args ) {
	for ( int i = 0; i < argc; ++i ) {
		if ( strcmp(args[i], "--license") == 0 ) {
			lecture();
			exit(0);
		}
		if ( strcmp(args[i], "--version") == 0 ) {
			version();
			exit(0);
		}
		if ( strcmp(args[i], "--help") == 0 ) {
			help();
			exit(0);
		}
		if ( strcmp(args[i], "--noexec") == 0 )
			noexec = true;
	}
}

int main ( int argc, char ** argv ) {
	processArgs( argc, argv );

	if ( noexec ) {
		std::cout << "Warning: In noexec mode." << std::endl;
	}

	std::cout << "Launching on behalf of " << _ON_BEHALF_OF << std::endl;

	std::vector<dot_desktop *> * files = new std::vector<dot_desktop *>();
	std::vector<std::string>   * dirs  = new std::vector<std::string>  ();

	if ( getConfDirs( dirs ) ) {
		if ( getDesktopFiles( dirs, files ) ) {
			//
		}
	}

	//std::vector<dot_desktop *> * files = loadDesktopFiles(getConfFiles(getConfDirs())); // XXX: This kinda sucks. Fix me if you have freetime.

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
			if ( index < 0 ) { // we're disabled
				happy = false;
				debug("");
				debug("Not running the following app ( Excluded by a OnlyShowIn )");
				debug(d->getAttr("Name"));
			} // we're not disabled ( therefore enabled )
		}
		if ( noti != "" ) { // NAUGHTY NAUGHTY
			int index = -1;
			index = noti.find(_ON_BEHALF_OF);
			if ( index >= 0 ) { // we're in 'dis
				happy = false;
				debug("");
				debug("Forced into not running the following app ( Included by not being in NotShowIn )");
				debug(d->getAttr("Name"));
			} // we're not enabled ( therefore disabled )
		}
		if ( d->getAttr("Hidden") == "" && happy ) { // LET'S DO THIS
			std::string appl = d->getAttr("Exec");
			if ( appl != "" ) {
				debug( "Processing File: ");
				debug(d->getFile());
				runApplication( appl );
			}
		} // otherwise, we're out of here.
	}
	return 0;
}

