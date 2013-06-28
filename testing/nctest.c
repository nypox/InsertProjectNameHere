#include <ncurses.h>
#include <unistd.h>
#include <netdb.h>

#include "../core/network.h"
#include "../core/DHT.h"
#include "../core/log.h"

#define BOOT_ADDR
#define BOOT_PORT

const char helpstr [] = "nctox - ncurses frontend to Tox\n\
Usage: nctox [option]\n\
Options:\n\n\
\t-h\t\tDisplays help\n\
\t-i=1.2.3.4\tChange the IPv4 address to bind to\n\
\t\t\tDefaults to \"0.0.0.0\"\n\
\t-p=1234\t\tChange the port to bind to\n\
\t\t\tDefaults to 33445";

void showhelp (void)
{
	puts(helpstr);
}

int main (int argc, char ** argv)
{
	// Get opts
	unsigned long o_ip	= 0;
	int o_port			= 33445;
	
	char o;
	struct hostent * h;
	// TODO add bootstrapping ip && port
	while ((o = getopt(argc, argv, "i:p:h")) != -1)
	{
		switch (o)
		{
				case 'h':
					showhelp();
					return 1;
				case 'i':
					h = gethostbyname(optarg);
					if (!h)
					{
						printf("Could not resolve host %s\n", optarg);
						return 1;
					}

					o_ip = *h->h_addr;
					break;
				case 'p':
					// TODO check port bounds
					o_port = atoi(optarg); break;
				case '?':
					if (optopt == 'i' || optopt == 'p')
					{
						printf("Option -%c requires an argument\n", optopt);
					}
					else
					{
						printf("Unknown option -%c\n", optopt);
					}
					showhelp();
					return 1;
		}
	}

	// Init logging
	log_init();
	atexit(log_exit);
	if (!log_open(LOG_INFO, "info.log") || !log_open(LOG_ERROR, "error.log"))
	{
		return 1;
	}

	// Init ncurses
	if (initscr() == -1)
	{
		log_print("Failed to init ncurses!", LOG_ERROR);
		return 1;
	}

	atexit((void (*)(void))endwin);
	// ncurses settings, turn echo off and set raw mode
	noecho();
	raw();

	// Set id
	srand(time(NULL));

	char client_id [CLIENT_ID_SIZE];
	int * p = (int *)&client_id;
	uint8_t i;

	// TODO not compatible with variable size CLIENT_ID_SIZE
	for (i = 0; i < 4; i++)
	{
		*p = rand();
		p += 4;
	}

	// Init networking
	IP ip;
	ip.i = o_ip; // From the options;
	if (!init_networking(ip, o_port))
	{
		return 1;
	}
	atexit(exit_networking);

	// Bootstrap
	IP_Port bootstrap_ip_port;
	bootstrap_ip_port = htons(BOOT_PORT);

	// Reuse struct hostent * h
	h = gethostbyname(BOOT_ADDR);
	bootstrap_ip_port.ip.i = *h->h_addr;

	if (!bootstrap(bootstrap_ip_port))
	{
		log_print("Failed to bootstrap!", LOG_ERROR);
		return 1;
	}

	return 0;
}
