/*
 * learnkeys.c --- ESE Key Daemon --- Keycode Daemon for Funny/Function Keys.
 *
 * Taps /dev/input/event<number> and makes config file for ESE Key Daemon.
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

int
main (int argc, char *argv[])
{
  unsigned short int device = 0;
  char device_name[strlen (EVENT_DEVICE)] = { 0 };
  FILE *funkey = 0;
  FILE *config = 0;
  char *key = 0;
  struct input_event ev;

  printf ("learnkeys (%s)\n", PACKAGE_STRING);

  if (argc < 2)
    {
      printf ("\nUsage:\n");
      printf ("%s config_file_name [input_device_name]\n", argv[0]);
      printf ("\nconfig_file_name  - location of esekeyd config file\n");
      printf ("input_device_name - if given turns off autodetection\n");
      printf ("                    of 1st keyboard device\n");
      printf ("\nExample:\n");
      printf ("%s ~/.esekeyd.conf /dev/input/event3\n", argv[0]);
      exit (1);
    }

  if (argc > 2)
    {
      sprintf (device_name, "%s", argv[2]);
    }
  else
    {
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
	  sprintf (device_name, "%s%hu", EVENT_DEVICE, device);
	}
    }

  if (!(funkey = fopen (device_name, "r")))
    {
      printf ("%s: can`t open %s\n", argv[0], device_name);
      return -4;
    }

  if (!(config = fopen (argv[1], "w")))
    {
      printf ("%s: can`t open %s\n", argv[0], argv[1]);
      return -5;
    }

  fprintf (config, "#\n# %s config file\n#\n\n", PACKAGE_STRING);
  fprintf (config,
	   "#\n# example: to run mutt in xterm we must set DISPLAY\n");
  fprintf (config, "#          so the command line will be as follows:\n");
  fprintf (config, "#MAIL:/bin/sh -c \"DISPLAY=:0 xterm -e mutt\"\n#\n\n");

  printf ("\nPres ANY (fun)key... or ENTER to exit...\n\n");

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

	  printf ("key %s stored in config file\n", key);
	  fprintf (config, "#%s:\n", key);

	}

    }

  fclose (funkey);
  fclose (config);

  return 0;

}
