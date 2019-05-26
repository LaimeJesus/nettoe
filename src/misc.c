/* netToe Version 1.5.1
 *
 * Copyright 2000,2001 Gabriele Giorgetti <ggdev@users.sourceforge.net>
 *           2009-2014 Mats Erik Andersson <meand@users.sourceforge.net>
 *		
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <glib/gi18n.h>

/* Macros AF_* for address families. */
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#  include <sys/socket.h>
#endif 
#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif

#include "game.h"
#include "misc.h"
#include "terminal.h"

int who_start_first(void)
{
	long generated_number;

	generated_number = 1 + (random() % 10);

	if ( (generated_number == 2) || (generated_number == 4)
			|| (generated_number == 6) || (generated_number == 8)
			|| (generated_number == 10) )
	{
		return 1;
	}
 
	return 2;
} /* who_start_first(void) */

void get_player_pname(char * str, size_t maxlen)
{
	char buffer[1024], *pek;
	size_t n = 0;

	str[0] ='\0';
	if ( maxlen <= 1 )
		return;

	if (!fgets(buffer, sizeof(buffer), stdin))
	  buffer[0] = '\0';

	pek = buffer;

	/* Skip initial white space. */
	while ( *pek == ' ' || *pek == '\t' || *pek == '\r' )
		++pek;

	while ( *pek && (n + 1 < maxlen) && (n + 1 < sizeof(buffer)) )
	{
		if ( *pek == '\n' )
			break;

		/* Promote LF and TAB to simple space. */
		if ( *pek == '\r' || *pek == '\t' )
			*pek = ' ';

		/* Avoid duplicate white space. */
		if ( (n > 0) && (str[n - 1] == ' ') && (*pek == ' ') )
		{
			++pek;
			continue;
		}

		str[n++] = *(pek++);
	}

	/* Skip trailing white space. */
	while ( (n > 0) && (str[n - 1] == ' ') )
		--n;

	/* Safe guard for empty name. */
	if ( n == 0 )
		strcpy(str, "Anon");
	else
		str[n] = '\0';
} /* get_player_pname(char *, size_t) */

int check_pname(const char *pname, size_t maxlen)
{
	if ( pname == NULL )
		return 1;
   
	if ( (strlen(pname)) > maxlen )
		return 1;
   
	return 0;
} /* check_pname(const char *, size_t) */

void print_infos_screen (void)
{
	nettoe_term_reset_color();
	printf(_(" netToe is a Tic Tac Toe-like game for Linux and UNIX.    "));
	printf("\n");
	printf(_(" It is possible to play it against the computer, another  "));
	printf("\n");
	printf(_(" player on the same PC, or against another player over    "));
	printf("\n");
	printf(_(" a network (Internet, and everything using TCP/IP).       "));
	printf("\n");
	printf(_(" To play it over a network you must first set up a server."));
	printf("\n");
	printf(_(" This is done in the network game options menu, selecting "));
	printf("\n");
	printf(" \"");
	printf(_("Host the game"));
	printf("\".");
	printf(_(" Then the (remote) second player must  "));
	printf("\n");
	printf(_(" connect to the server by typing its IP address, or name. "));
	printf("\n");
	printf(_(" That should be enough for you to have some leisure.      "));
	printf("\n");
	printf(_(" For a detailed guide on HOW TO PLAY, and for other info, "));
	printf("\n");
	printf(_(" do read the manual page "));
	nettoe_term_set_color (COLOR_BLUE, ATTRIB_BRIGHT);
	printf("nettoe");
	nettoe_term_reset_color();
	printf("(6).\n\n");

	printf(" netToe %s (%s)\n", PACKAGE_VERSION, PACKAGE_RELEASE_DATE);
	printf(_(" Copyright 2000,2001 Gabriele Giorgetti"));
	printf("\n");
	printf(_("           2009-2014 Mats Erik Andersson"));
	printf("\n");
	printf(" %s\n", HOMEPAGE);
	printf("\n");
	printf(_(" Press"));
	nettoe_term_set_color (COLOR_RED, ATTRIB_BRIGHT);
	printf(_(" enter"));
	nettoe_term_reset_color();
	printf(_(" to go back to the main menu. "));
	 
	fflush (stdin);
	getchar ();

	nettoe_term_set_default_color();
} /* print_infos_screen(void) */

void parse_cmd_args(int argc, char *argv[])
{
	int i;

	/* Any address family will do. */
	addrfamily = AF_UNSPEC;

	if ( argc < 2 )
		return;

	for (i = 1; i <= argc - 1; i++)
	{
		if ( (!strcmp(argv[i], "-h")) || (!strcmp(argv[i], "--help")) )
		{
			fprintf(stdout, _("netToe %s the enhanced, and networked, Tic Tac Toe game."), PACKAGE_VERSION);
			fprintf(stdout, "\n\n");
			fprintf(stdout, _("Usage:  nettoe [OPTIONS]"));
			fprintf(stdout, "\n\n");
			fprintf(stdout, _("-4,  --ipv4        only IPv4 networking"));
			fprintf(stdout, "\n");
			fprintf(stdout, _("-6,  --ipv6        only IPv6 networking"));
			fprintf(stdout, "\n");
			fprintf(stdout, _("-nb, --no-beep     disable beeps"));
			fprintf(stdout, "\n");
			fprintf(stdout, _("-nc, --no-colors   disable colors"));
			fprintf(stdout, "\n");
			fprintf(stdout, _("-h,  --help        display this help, then exit"));
			fprintf(stdout, "\n");
			fprintf(stdout, _("-v,  --version     output version information, then exit"));
			fprintf(stdout, "\n\n");
			fprintf(stdout, _("The netToe project can be found at: "));
			fprintf(stdout, "\n");
			fprintf(stdout, "  %s\n", HOMEPAGE);
			fprintf(stdout, "\n");
			fprintf(stdout, _("Please send any bug reports, or comments to:"));
			fprintf(stdout, "\n");
			fprintf(stdout, "  %s\n", AUTHOR2_EMAIL);

			exit(EXIT_SUCCESS);
		}
		else if ( (!strcmp(argv[i], "-v"))
				|| (!strcmp(argv[i], "--version")) )
		{
			fprintf(stdout, "netToe %s (%s)",
				PACKAGE_VERSION, RELEASE_DATE);
#if USE_TERMINFO
			fprintf(stdout, _(" with terminfo support"));
#elif USE_TERMCAP
			fprintf(stdout, _(" with termcap support"));
#endif
			fprintf(stdout, _(", at TCP port %d."), SERVER_PORT_NUMBER);
			fprintf(stdout, "\n\n");
			fprintf(stdout, _("Written by Gabriele Giorgetti %s"), AUTHOR_EMAIL);
			fprintf(stdout, "\n");
			fprintf(stdout, _("Copyright 2000,2001 Gabriele Giorgetti"));
			fprintf(stdout, "\n");
			fprintf(stdout, _("          2009-2014 Mats Erik Andersson"));
			fprintf(stdout, "\n\n");
			fprintf(stdout, _("This software is released under GNU GPL 2."));
			fprintf(stdout, "\n");
			
			exit(EXIT_SUCCESS);
		}
		else if ( (!strcmp(argv[i], "-4"))
				|| (!strcmp(argv[i], "--ipv4")) )
		{
			addrfamily = AF_INET;
			continue;
		}
		else if ( (!strcmp(argv[i], "-6"))
				|| (!strcmp(argv[i], "--ipv6")) )
		{
			addrfamily = AF_INET6;
			continue;
		}
		else if ( (!strcmp(argv[i], "-nb"))
				|| (!strcmp(argv[i], "--no-beep")) )
		{
			NO_BEEP = 1;
			continue;
		}
		else if ( (!strcmp(argv[i], "-nc"))
				|| (!strcmp(argv[i], "--no-colors")) )
		{
			NO_COLORS = 1;
			continue;
		} else {
			fprintf(stdout, _("%s: unrecognized option `%s'"), argv[0], argv[i]);
			fprintf(stdout, "\n");
			fprintf(stdout, _("Try `%s --help' for more information."), argv[0]);
			fprintf(stdout, "\n");

			exit (EXIT_SUCCESS);
		}
	}

	return;
} /* parse_cmd_args(int, char *[]) */

/*
 * vim: sw=4 ts=4
 */
