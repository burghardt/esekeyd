/*
 * keytest.c --- ESE Key Daemon --- Keycode Daemon for Funny/Function Keys.
 *
 * Taps /dev/input/event<number> and reports pressed keys names.
 *
 * $Id$
 *
 * Based on code from funky.c released under the GNU Public License
 * by Rick van Rein.
 *
 * (c) 2000 Rick van Rein.
 * (c) 2004 Krzysztof Burghardt.
 *
 * Released under the GNU Public License.
 */

#include "esekey.h"

FILE *funkey = NULL;

void cleanup ()
{
    fclose (funkey);
}

void signal_handler (int x)
{
    printf ("\n\nCaught signal %d, exiting...\n", x);
    cleanup ();
    exit(x);
}

void register_signal_handlers (void)
{
    signal (SIGHUP,  signal_handler);
    signal (SIGINT,  signal_handler);
    signal (SIGQUIT, signal_handler);
    signal (SIGILL,  signal_handler);
    signal (SIGTRAP, signal_handler);
    signal (SIGABRT, signal_handler);
    signal (SIGIOT,  signal_handler);
    signal (SIGFPE,  signal_handler);
    signal (SIGKILL, signal_handler);
    signal (SIGSEGV, signal_handler);
    signal (SIGPIPE, signal_handler);
    signal (SIGTERM, signal_handler);
    signal (SIGSTOP, signal_handler);
    signal (SIGUSR1, SIG_IGN);
}

int main (int argc, char *argv[])
{
    short int device = 0;
    char *device_name = NULL;
    char *key = NULL;
    struct input_event ev;

    printf ("keytest (%s)\n", PACKAGE_STRING);

    if (argc > 1)
    {
        device_name = argv[1];
    }
    else
    {
        printf ("\n(input device name as 1st option override autodetection)\n");

        switch (check_handlers ())
        {
            case -1:
                printf ("%s: cannot open %s\n", argv[0], INPUT_HANDLERS);
                return -1;
            case -2:
                printf ("%s: evdev handler not found in %s\n", argv[0],
                        INPUT_HANDLERS);
                return -2;
        }

        switch (device = find_input_dev ())
        {
            case -1:
                printf ("%s: evdev for keyboard not found in %s\n", argv[0],
                        INPUT_HANDLERS);
                return -3;
            default:
                asprintf (&device_name, "%s%hu", EVENT_DEVICE, device);
        }
    }

    if (!(funkey = fopen (device_name, "r")))
    {
        printf ("%s: can`t open %s\n", argv[0], device_name);
        return -4;
    }

    printf ("\nPres ANY (fun)key... or Ctrl-C to exit...\n\n");

    register_signal_handlers();

    while (fread (&ev, sizeof (struct input_event), 1, funkey))
    {

        if (ev.code == 28 && ev.type == EV_KEY && ev.value == 1)
        {

            printf ("\n");
            fclose (funkey);
            return 0;

        }

        if ((key = parse (ev)) != NULL)
        {

            printf ("%s ", key);
            fflush (stdout);

        }

    }

    cleanup ();

    return 0;

}
