#include <stdint.h>

#define LOG_INFO 0
#define LOG_ERROR 1
//#define LOG_WARN 2

void log_init (void);
uint8_t log_open (uint8_t type, char * fn);
//uint8_t log_open_sock (uint8_t type, int sock);
void log_print (char * msg, uint8_t type);
void log_exit (void);

