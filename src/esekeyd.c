/*
 * esekeyd.c --- ESE Key Daemon --- Keycode Daemon for Funny/Function Keys.
 *
 * Taps /dev/input/event<number> and starts the required action for that keycode.
 *
 * Based on code from funky.c released under the GNU Public License
 * by Rick van Rein.
 *
 * (c) 2000 Rick van Rein.
 * (c) 2002-2004 Krzysztof Burghardt.
 *
 * Released under the GNU Public License.
 */

#include "esekey.h"

int
main (int argc, char *argv[])
{
  unsigned short int device = 0;
  char device_name[strlen (EVENT_DEVICE)] = { 0 };
  unsigned int keycount = 0;
  unsigned int i = 0;
  FILE *funkey = NULL;
  char *key = NULL;
  struct input_event ev;
  struct esekey *keys = NULL;

  printf ("%s\n", PACKAGE_STRING);

  if (argc < 2)
    {
      printf ("%s: no config file given ?!\n", argv[0]);
      exit (1);
    }
  else
    {

      FILE *fp = 0;

      fp = fopen (argv[1], "r");

      if (!fp)
	{
	  printf ("%s: cannot open %s\n", argv[0], argv[1]);
	  return -1;
	}

#ifdef DEBUGGER
      printf ("\n>>>> CONFIG FILE DUMP >>>>\n");
#endif

      while (!feof (fp))
	{
	  char *buff = 0;
	  size_t len = 0;
	  getline (&buff, &len, fp);
	  if (buff[0] >= 65 && buff[0] <= 90)
	    {

	      buff[strlen (buff) - 1] = '\0';
	      keys =
		(struct esekey *) realloc (keys,
					   (keycount +
					    1) * sizeof (struct esekey));

	      keys[keycount].command = strdup (strchr (buff, ':') + 1);
	      keys[keycount].name =
		(char *) malloc (strchr (buff, ':') - buff + 1);
	      strncpy (keys[keycount].name, buff, strchr (buff, ':') - buff);
	      keys[keycount].name[strchr (buff, ':') - buff] = '\0';
#ifdef DEBUGGER
	      printf ("%s <-*-> %s\n", keys[keycount].name,
		      keys[keycount].command);
#endif
	      ++keycount;
	    }
	}

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

#ifndef DEBUGGER
  switch (fork ())
    {
    case -1:
      printf ("%s: fork() error", argv[0]);
      exit (-3);
    case 0:
      break;
    default:
      exit (0);
    }
#endif

  switch (check_handlers ())
    {
    case -1:
      printf ("%s: cannot open %s\n", argv[0], INPUT_HANDLERS);
      return -4;
    case -2:
      printf ("%s: evdev handler not found in %s\n", argv[0], INPUT_HANDLERS);
      return -5;
    }

  switch (device = find_input_dev ())
    {
    case -1:
      printf ("%s: evdev for keyboard not found in %s\n", argv[0],
	      INPUT_HANDLERS);
      return -6;
    default:
      sprintf (device_name, "%s%hu", EVENT_DEVICE, device);
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

  openlog ("esekeyd", LOG_PID, LOG_DAEMON);
  syslog (LOG_NOTICE, "started key daemon for %s", device_name);

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

  closelog ();

  fclose (funkey);

  return 0;

}
