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
        getline (&buff, &len, fp);
        sscanf (buff, "N: Number=%hu Name=%s Minor=%hu", &number, name, &minor);
        free (buff);
        if (strncmp (name, "evdev", 5) == 0 && minor == 64)
        {
            have_evdev = 1;
            break;
        }
    }

    fclose (fp);

    if (!have_evdev)
        return -2;

    return 0;
}

signed char find_input_dev (void)
{
    FILE *fp = NULL;
    signed char have_evdev = -2;

    fp = fopen (INPUT_DEVICES, "r");

    if (!fp)
        return -1;

    while (!feof (fp))
    {
        char *buff = NULL;
        size_t len = 0;
        short int number = -2;
        getline (&buff, &len, fp);
        sscanf (buff, "H: Handlers=kbd event%hu", &number);
        free (buff);
        if (number > -1)
        {
            have_evdev = number;
            break;
        }
    }

    fclose (fp);

    return have_evdev;
}
