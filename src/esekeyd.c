/*
 * esekeyd.c --- ESE Key Daemon --- Keycode Daemon for Funny/Function Keys.
 *
 * Taps /dev/input/event<number> and starts the required action for that keycode.
 *
 * $Id$
 *
 * Based on code from funky.c released under the GNU Public License
 * by Rick van Rein.
 *
 * (c) 2000 Rick van Rein.
 * (c) 2002-2004,2006 Krzysztof Burghardt.
 *
 * Released under the GNU Public License.
 */

#include "esekey.h"

FILE *funkey = NULL;
char *pid_name = NULL;

void cleanup ()
{
    closelog ();
    fclose (funkey);
    unlink (pid_name);
}

void signal_handler (int x)
{
    syslog (LOG_NOTICE, "caught signal %d, exiting...", x);
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
    unsigned int keycount = 0;
    struct esekey *keys = NULL;
    unsigned int i = 0;
    char *key = NULL;
    struct input_event ev;
    FILE *pid_fp = NULL;
    pid_t pid = 0;

    printf ("%s\n", PACKAGE_STRING);
    pid_name = (argc > 3) ? strdup(argv[3]) : strdup(PID_FILE);

    /* check to see if a copy of ESE Key Daemon is already running */
    if (!access(pid_name, R_OK))
    {
        if ((pid_fp = fopen(pid_name, "r" )))
        {
            fscanf(pid_fp, "%d", &pid);
            fclose(pid_fp);

            if (kill(pid, SIGUSR1)==0)
            {
                fprintf(stderr, "esekeyd: already running as process %d.\n", pid);
                exit(-1);
            }

            unlink(pid_name);
        }
    }

    if (argc < 2)
    {
        printf ("\nUsage:\n");
        printf ("%s config_file_name [input_device_name] [pidfile_name]\n\n", argv[0]);
        printf ("config_file_name  - location of esekeyd config file\n");
        printf ("input_device_name - input (event) device; if given turns off\n");
        printf ("                     autodetection of 1st keyboard device\n");
        printf ("pidfile_name      - location of esekeyd pidfile;\n");
        printf ("                     default is %s\n", PID_FILE);
        printf ("\nExample:\n");
        printf ("%s ~/.esekeyd.conf /dev/input/event3 /var/run/esekeyd.event3.pid\n", argv[0]);
        exit (1);
    }
    else
    {
        FILE *fp = NULL;
        char *buff = NULL;
        size_t len = 0;
        ssize_t read;
        fp = fopen (argv[1], "r");
        if (!fp)
        {
            printf ("%s: cannot open %s\n", argv[0], argv[1]);
            return -1;
        }
#ifdef DEBUGGER
        printf ("\n>>>> CONFIG FILE DUMP >>>>\n");
#endif

        while ((read = getline(&buff, &len, fp)) != -1)
        {
            if ((buff[0] >= 48 && buff[0] <= 57) || (buff[0] >= 65 && buff[0] <= 90))
            {
                // remove newline and whitespaces from end of string
                while (read && buff[read - 1] <= 32)
                {
                    --read;
                }
                if (!read)
                    continue;
                buff[read] = '\0';

                keys = (struct esekey *) realloc (keys, (keycount +1) * sizeof (struct esekey));
                keys[keycount].command = strdup (strchr (buff, ':') + 1);
                keys[keycount].name = (char *) malloc (strchr (buff, ':') - buff + 1);
                strncpy (keys[keycount].name, buff, strchr (buff, ':') - buff);
                keys[keycount].name[strchr (buff, ':') - buff] = '\0';
#ifdef DEBUGGER
                printf ("%s <-*-> %s\n", keys[keycount].name,
                        keys[keycount].command);
#endif
                ++keycount;
            }
        }
        free (buff);

#ifdef DEBUGGER
        printf ("<<<< CONFIG FILE DUMP <<<<\n\n");
#endif

        fclose (fp);

        if (!keycount)
        {
            printf ("%s: error reading config file\n", argv[0]);
            return -2;
        }

    }

    if (argc > 2)
    {
        device_name = argv[2];
    }
    else
    {
        switch (check_handlers ())
        {
            case -1:
                printf ("%s: cannot open %s\n", argv[0], INPUT_HANDLERS);
                return -4;
            case -2:
                printf ("%s: evdev handler not found in %s\n",
                        argv[0], INPUT_HANDLERS);
                return -5;
        }

        switch (device = find_input_dev ())
        {
            case -1:
                printf ("%s: evdev for keyboard not found in %s\n",
                        argv[0], INPUT_HANDLERS);
                return -6;
            default:
                asprintf (&device_name, "%s%hu", EVENT_DEVICE, device);
        }
    }

    if (!(funkey = fopen (device_name, "r")))
    {
        printf ("%s: can`t open %s\n", argv[0], device_name);
        return -7;
    }
#ifndef DEBUGGER
    fclose (stdin);
    fclose (stdout);
    fclose (stderr);
#endif

    register_signal_handlers();

    openlog ("esekeyd", LOG_PID, LOG_DAEMON);
    syslog (LOG_NOTICE, "started key daemon for %s", device_name);

#ifndef DEBUGGER
    switch (pid = fork ())
    {
        case -1:
            syslog (LOG_NOTICE, "fork() error");
            exit (-3);
        case 0:
            break;
        default:
            /* create the pid file */
            if ((pid_fp = fopen(pid_name, "w")))
            {
                fprintf(pid_fp, "%d\n", pid);
                fclose(pid_fp);
            }
            else
                syslog (LOG_NOTICE, "fail to create %s", pid_name);

            exit (0);
    }
#endif

    while (fread (&ev, sizeof (struct input_event), 1, funkey))
    {

        if ((key = parse (ev)) != NULL)
        {
            for (i = 0; i < keycount; ++i)
            {
                if (strcmp (key, keys[i].name) == 0)
                {
#ifdef DEBUGGER
#warning DEBUGGER defined - esekeyd will NOT run any command in DEBUG MODE
                    printf ("%s <-*-> %s\n", keys[i].name, keys[i].command);
#else
                    system (keys[i].command);
#endif
                }
            }

        }

    }

    cleanup();

    return 0;

}
