SUBDIRS = src doc examples

BUILT_SOURCES = version.h
EXTRA_DIST = version.h

if DOT_SVN
version.h: .svn/entries
	echo \#define PACKAGE_VERSION_SVN_REV \"SVN-r`svnversion`\" > version.h
else
if DOT_GIT
version.h: .git/HEAD .git/index
	echo \#define PACKAGE_VERSION_SVN_REV \"GIT-`git describe --dirty --always --tags`\" > version.h
else
version.h:
	echo \#define PACKAGE_VERSION_SVN_REV \"unknown\" > version.h
endif
endif

clean-local:
	rm -f version.h
