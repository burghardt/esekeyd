SUBDIRS = src doc examples

EXTRA_DIST = version.h

if DOT_SVN
version.h: .svn/entries
else
version.h:
endif
	echo \#define PACKAGE_VERSION_SVN_REV \"SVN-r`svnversion`\" > version.h

all: config.h version.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive
