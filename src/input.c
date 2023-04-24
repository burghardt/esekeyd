/*
 * input.c --- ESE Key Daemon --- Keycode Daemon for Funny/Function Keys.
 *
 * $Id$
 *
 * (c) 2002-2004,2006 Krzysztof Burghardt.
 *
 * Released under the GNU Public License.
 */

#include "esekey.h"

#include <dirent.h>
#define CLASS_DIR "/sys/class/input"
#define DEV_DIR "/dev/input"

#ifndef PATH_MAX
#  define PATH_MAX 1024
#endif

signed char check_handlers (void)
{
    FILE *fp = NULL;
    signed char have_evdev = 0;

    fp = fopen (INPUT_HANDLERS, "r");

    if (!fp)
        return -1;

    while (!feof (fp))
    {
        char name[128];
        char *buff = NULL;
        size_t len = 0;
        unsigned short int number = 0;
        unsigned short int minor = 0;
        if (getline (&buff, &len, fp) > 0) {
            sscanf (buff, "N: Number=%hu Name=%s Minor=%hu", &number, name,
                    &minor);
            free (buff);
            if (strncmp (name, "evdev", 5) == 0 && minor == 64)
            {
                have_evdev = 1;
                break;
            }
        }
    }

    fclose (fp);

    if (!have_evdev)
        return -2;

    return 0;
}

/* return true if /dev/input/event<devno> is a keyboard.
    This could also have been done by way of ioctls instead of reading
    from /sys files. Hopefully the /sys API doesn't change...
    It's documented thoroughly here:
    https://unix.stackexchange.com/questions/74903/explain-ev-in-proc-bus-input-devices-data
 */
static int is_keyboard(int devno)
{
    char filename[PATH_MAX + 1];
    char *buf = NULL;
    size_t len = 0, caps = 0;
    size_t wantcaps = (1 << EV_SYN | 1 << EV_KEY | 1 << EV_MSC | 1 << EV_LED | 1 << EV_REP);
    FILE *fp;

    sprintf(filename, "%s/event%d/device/capabilities/ev", CLASS_DIR, devno);
    if(!(fp = fopen(filename, "r"))) return 0;

    if (getline(&buf, &len, fp) < 1) return 0;;
    fclose(fp);
    if(!buf) return 0;

    caps = strtol(buf, NULL, 16);
    free(buf);

    return (caps & wantcaps) == wantcaps;
}

/* returns the highest-numbered keyboard found. The common case is
    that the internal keyboard on a laptop is numbered lower than an external
    keyboard, and we assume that if there's an external, it's the one the
    user actually uses. If no keyboards are found, returns -1. */
signed char find_input_dev (void)
{
    DIR *dir;
    struct dirent *entry;
    int last_kbd = -1;
    char buf[PATH_MAX + 1];

    dir = opendir(CLASS_DIR);
    if(!dir) {
        perror(CLASS_DIR);
        return -1;
    }

    while( (entry = readdir(dir)) ) {
        sprintf(buf, "%s/%s", CLASS_DIR, entry->d_name);
        if(strncmp(entry->d_name, "event", 5) == 0) {
            int devno = atoi(entry->d_name + 5);
            if(is_keyboard(devno)) {
                fprintf(stderr, "event%d is a keyboard\n", devno);
                if(devno > last_kbd) last_kbd = devno;
            }
        }
    }
    closedir(dir);

    fprintf(stderr, "Autodetected keyboard: %s/event%d\n", DEV_DIR, last_kbd);

    return last_kbd;
}
