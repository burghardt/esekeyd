bin_PROGRAMS = keytest learnkeys
keytest_SOURCES = keytest.c input.c parser.c esekey.h
learnkeys_SOURCES = learnkeys.c input.c parser.c esekey.h

sbin_PROGRAMS = esekeyd
esekeyd_SOURCES = esekeyd.c input.c parser.c esekey.h

noinst_PROGRAMS = mkparser
mkparser_SOURCES = mkparser.c

mkparser$(EXEEXT): $(mkparser_SOURCES)
	$(CC_FOR_BUILD) -o $@ $(mkparser_SOURCES)

parser.c: mkparser /usr/include/linux/input.h
	./mkparser /usr/include/linux/input-event-codes.h > parser.c

#parser.c: mkparser /usr/src/linux/include/linux/input.h
#	./mkparser /usr/src/linux/include/linux/input-event-codes.h > parser.c

clean-local:
	rm -f parser.c
