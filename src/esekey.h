/*
 * esekey.h --- ESE Key Daemon --- Keycode Daemon for Funny/Function Keys.
 *
 * (c) 2004 Krzysztof Burghardt.
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
#include <linux/input.h>
#include "../config.h"

#define INPUT_HANDLERS "/proc/bus/input/handlers"
#define INPUT_DEVICES "/proc/bus/input/devices"
#define EVENT_DEVICE "/dev/input/event"

struct esekey
{
  char *name;
  char *command;
};

signed char check_handlers (void);
signed char find_input_dev (void);
char *parse (struct input_event);
