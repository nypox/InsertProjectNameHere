/* network.h
* 
* Datatypes, functions and includes for the core networking.
* 
 
    Copyright (C) 2013 Tox project All Rights Reserved.

    This file is part of Tox.

    Tox is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tox.  If not, see <http://www.gnu.org/licenses/>.
    
*/
 

#ifndef NETWORK_H 
#define NETWORK_H 

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>


#ifdef WIN32 //Put win32 includes here

#include <winsock2.h>
#include <windows.h>

#else //Linux includes

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/time.h>
#endif

#define MAX_UDP_PACKET_SIZE 65507

typedef union
{
    uint8_t c[4];
    uint16_t s[2];
    uint32_t i;
}IP;

typedef struct
{
    IP ip;
    uint16_t port;
    //not used for anything right now
    uint16_t padding; 
}IP_Port;

typedef struct
{
    int16_t family;
    uint16_t port;
    IP ip;
    uint8_t zeroes[8];
    #ifdef ENABLE_IPV6
    uint8_t zeroes2[12];
    #endif
}ADDR;


//returns current time in milleseconds since the epoch.
uint64_t current_time();

//return a random number
int random_int();

//Basic network functions:

//Function to send packet(data) of length length to ip_port
unsigned int sendpacket(IP_Port ip_port, char * data, uint32_t length);

//Function to recieve data, ip and port of sender is put into ip_port
//the packet data into data
//the packet length into length.
unsigned int recievepacket(IP_Port * ip_port, char * data, uint32_t * length);

//initialize networking
//bind to ip and port
//ip must be in network order EX: 127.0.0.1 = (7F000001)
//port is in host byte order (this means don't worry about it)
//returns 0 if no problems
//TODO: add something to check if there are errors
uint8_t init_networking(IP ip, uint16_t port);

//function to cleanup networking stuff(doesn't do much right now)
void shutdown_networking();
#endif
