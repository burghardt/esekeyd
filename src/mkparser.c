/*
 * mkparser.c --- Make Parser for ESE Key Daemon
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
#include "../config.h"

int
main (int argc, char *argv[])
{
  char name[128];
  int value = 0;
  char *buff = 0;
  size_t len = 0;

  FILE *fp = fopen (argv[1], "r");

  if (!fp)
    return -1;

  printf ("/* autogenerated by mkparser - DO NOT EDIT */\n\n");
  printf ("#include \"esekey.h\"\n\n");

  printf ("char*\n");
  printf ("parse(struct input_event ev)\n");
  printf ("{\n");
  printf ("\tstatic char *ret_val;\n\n");
  printf ("\tif (ev.type == EV_KEY && ev.value == 1) {\n\n");
  printf ("\t\tswitch (ev.code) {\n\n");

//  printf ("\t\t\t/* Keys with keykodes < 111 are useless for Funkey\n");
//  printf ("\t\t\t   because they are present on all keyboards\n");
//  printf ("\t\t\t   (e.g. letters, numbers, ENTER, SHIFT, other) */\n\n");

  while (!feof (fp))
    {
      getline (&buff, &len, fp);
      sscanf (buff, "#define KEY_%s %i", name, &value);
      if (value != 0 /* && value > 110 */ )
	{
	  printf ("\t\t\tcase %i:\n", value);
	  printf ("\t\t\t\treturn \"%s\";\n", name);
	  value = 0;
	}
    }

  printf ("\t\t\tdefault:\n");
  printf ("\t\t\t\tif (! ret_val) ret_val = malloc (12);\n");
  printf ("\t\t\t\tsprintf(ret_val, \"KEY_%%d\", ev.code);\n");
  printf ("\t\t\t\treturn ret_val;\n");

  printf ("\n\t\t}\n\n");
  printf ("\t}\n\n");
  printf ("\treturn NULL;\n\n");
  printf ("}\n");
  printf ("\n");

  fclose (fp);

  return 0;

}