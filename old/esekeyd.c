/*
 * esekeyd.c --- ESE Key Daemon --- Keycode Daemon for Funny/Function Keys.
 *
 * Taps /dev/funkey and starts the required action for that keycode.
 *
 * $Id: esekeyd.c,v 1.2 2006-02-21 21:37:29 kb Exp $
 *
 * Based on code from funky.c released under the GNU Public License
 * by Rick van Rein.
 *
 * (c) 2000 Rick van Rein.
 * (c) 2002 Krzysztof Burghardt.
 *
 * Released under the GNU Public License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) {
	int funkey;
	unsigned char buf;
	char cmd[80];
	
	fclose(stdin); fclose(stdout); fclose(stderr);
	openlog("esekeyd", LOG_PID, LOG_DAEMON);
	
	if ( argc < 2 ) {
	    syslog(LOG_ERR, "no interpreter defined ?!");
	    exit (1);
	}
	
	switch (fork()) {
	    case -1:
	    	syslog(LOG_ERR, "fork() error");
		exit (1);
	    case 0:
		break;
	    default:
		exit (0);	
	}

	funkey = open ("/dev/funkey", O_RDONLY);
	if (funkey < 0) {
		syslog(LOG_ERR, "cannot read /dev/funkey");
		exit (1);
	}
	
	syslog(LOG_NOTICE, "started key daemon for /dev/funkey");

	while (read (funkey, &buf, 1)) {
		sprintf(cmd,"%s %i &",argv[1],buf);
		system (cmd);
	}

	closelog();

	close (funkey);

	return 0;
}
