#AUTOMAKE_OPTIONS = 1.7.5 readme-alpha
bin_PROGRAMS = keytest learnkeys
keytest_SOURCES = keytest.c input.c parser.c esekey.h
learnkeys_SOURCES = learnkeys.c input.c parser.c esekey.h

sbin_PROGRAMS = esekeyd
esekeyd_SOURCES = esekeyd.c input.c parser.c esekey.h

noinst_PROGRAMS = mkparser
mkparser_SOURCES = mkparser.c

parser.c: mkparser /usr/include/linux/input.h
	./mkparser /usr/include/linux/input.h > parser.c

clean:
	rm -f parser.c

indent:
	indent *.c *.h
	rm -f *~
