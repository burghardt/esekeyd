/*
 * esekey.h --- ESE Key Daemon --- Keycode Daemon for Funny/Function Keys.
 *
 * $Id$
 *
 * (c) 2004,2006 Krzysztof Burghardt.
 *
 * Released under the GNU Public License.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <linux/input.h>
#include "../version.h"
#include "../config.h"

#define INPUT_HANDLERS "/proc/bus/input/handlers"
#define INPUT_DEVICES "/proc/bus/input/devices"
#define EVENT_DEVICE "/dev/input/event"
#define PID_FILE "/var/run/esekeyd.pid"
#define KEY_SEPARATOR "+"
#define KEYS_DELAY 300000

struct esekey
{
    char *name;
    int value;
    char *command;
};

signed char check_handlers (void);
signed char find_input_dev (void);
char *parse (struct input_event);
