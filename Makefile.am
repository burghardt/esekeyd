SUBDIRS = src doc examples

EXTRA_DIST = version.h

if DOT_SVN
version.h: .svn/entries
else
version.h:
endif
	echo \#define PACKAGE_VERSION_SVN_REV \"SVN-r`svnversion`\" > version.h

if DOT_GIT
version.h: .git/HEAD .git/index
else
version.h:
endif
	echo \#define PACKAGE_VERSION_SVN_REV \"GIT-`git describe --dirty --always --tags`\" > version.h

all: config.h version.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive
