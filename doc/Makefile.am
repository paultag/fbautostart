# doc/Makefile.am for Fluxbox 0.9  - an X11 Window manager
# Makefile.am for fluxbox/doc

CLEANFILES = fbautostart.1 
MAINTAINERCLEANFILES = Makefile.in
man_MANS = fbautostart.1
EXTRA_DIST=fbautostart.1.in \

distclean-local:
	rm -f *\~

%: %.in
	@regex_cmd@ -e "s,@pkgdatadir@,$(pkgdatadir)," $^ > $@
