// Copyright (C) 2001-2009 Keith Fancher <discostoo at users.sourceforge.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


/* net.cpp - The totally hacky, undocumented, probably insanely buggy code for
 * running games across the innertrons.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "bomns.h"
#include "level.h"
#include "net.h"


// Whether we are doing any kind of netplay at all.
bool g_bNetPlay = false;

// If g_bNetPlay, true if we are the listener/host/master, false if we are the
// connector/client/slave.
bool g_bNetHost = false;

// If hosting, the local IP/port to bind to; if connecting, the remote IP/port
// to connect to.
static struct sockaddr g_addr;

//Our connected socket file descriptor.
static int g_sd = -1;


static void FillAddrFromString(char * str, struct sockaddr * sa)
{
   char * ip = str;
   char * port;
   if((port = strchr(str, ':')) != NULL)
      *port++ = '\0';

   // Taken from Beej's Guide <http://beej.us/guide/bgnet/>:
   struct addrinfo hints;
   memset(&hints, 0, sizeof(hints));
   hints.ai_family   = AF_UNSPEC;   // don't care IPv4 or IPv6
   hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
   hints.ai_flags    = AI_PASSIVE;  // fill in my IP for me

   int err;
   struct addrinfo * ai = NULL;
   if((err = getaddrinfo(ip, port, &hints, &ai)) != 0 || !ai)
   {
      fprintf(stderr, "getaddrinfo() error: %s\n", gai_strerror(err));
      exit(1);
   }

   // We'll just use the first one we find.  Balls deep in this shit.
   *sa = *ai->ai_addr;

   freeaddrinfo(ai);
}

// Here, we manipulate argc and argv to find netplay options, and then remove
// them so the rest of the command line parsing in main() can take effect.
void CheckNetplayCommandLineArgs(int * argc, char * argv[])
{
   for(int i = 1; argv[i]; ++i)
   {
      bool listen = false;
      bool connect = false;

      if((!strcmp(argv[i], "-listen") || !strcmp(argv[i], "--listen")) && argv[i+1])
         listen = true;
      else if((!strcmp(argv[i], "-connect") || !strcmp(argv[i], "--connect")) && argv[i+1])
         connect = true;

      if(listen || connect)
      {
         g_bNetPlay = true;
         g_bNetHost = listen;

         FillAddrFromString(argv[i+1], &g_addr);
         *argc -= 2;
         for(int j = i; (argv[j] = argv[j+2]) != NULL; ++j)
            ;
         break;
      }
   }
}

// In which we do the necessary shit to get a connected socket.
void MaybeStartNetplay(void)
{
   if(!g_bNetPlay)
      return;

   printf("Netplay!  Awesome!\n");

   int sd = socket(g_addr.sa_family, SOCK_STREAM, 0);
   if(sd < 0)
      QuitWithError("socket() failed");

   if(g_bNetHost)
   {
      printf("NET: Listening...");

      if(bind(sd, &g_addr, sizeof(g_addr)))
         QuitWithError("bind() failed");
      if(listen(sd, 1))
         QuitWithError("listen() failed");

      struct sockaddr remote_addr;
      socklen_t remote_addr_len;
      int sd2;
      if((sd2 = accept(sd, &remote_addr, &remote_addr_len)) < 0)
         QuitWithError("accept() failed");
      close(sd);
      sd = sd2;
   }
   else
   {
      printf("NET: Connecting...");

      if(connect(sd, &g_addr, sizeof(g_addr)))
         QuitWithError("connect() failed");
   }

   // Also, we need our newly-connected socket to be non-blocking, which means
   // setting the O_NONBLOCK flag.
   int old_fl, new_fl;
   if((old_fl = fcntl(sd, F_GETFL)) < 0)
      QuitWithError("fcntl(F_GETFL) failed");

   new_fl = old_fl | O_NONBLOCK;
   if(new_fl != old_fl)
   {
      if(fcntl(sd, F_SETFL, new_fl) < 0)
         QuitWithError("fcntl(F_SETFL) failed");
   }

   g_sd = sd;

   printf("ok!\n");
}

//Partially taken from libesmtp... don't ask.  It's GPL-licensed, so we' cool.
//<http://www.stafford.uklinux.net/libesmtp/>.
static void SocketWrite(int sd, const char * buf, int len)
{
   int n;
   for(int total = 0; total < len; total += n)
   {
      struct pollfd pollfd;
      pollfd.fd     = sd;
      pollfd.events = POLLOUT;
      errno = 0;
      while((n = write(sd, buf + total, len - total)) < 0)
      {
         if(errno == EINTR)
            continue;
         if(errno != EAGAIN)
            QuitWithError("Socket write() error");

         pollfd.revents = 0;
         int status;
         while((status = poll(&pollfd, 1, -1)) < 0)
         {
            if(errno != EINTR)
               QuitWithError("Socket poll() error");
         }

         if(status == 0 || !(pollfd.revents & POLLOUT))
            QuitWithError("Weird socket poll() error");
         errno = 0;
      }
   }
}

//Also from libesmtp.
static int SocketRead(int sd, char * buf, int len)
{
   struct pollfd pollfd;
   pollfd.fd     = sd;
   pollfd.events = POLLIN;

   errno = 0;
   int n;
   while((n = read(sd, buf, len)) < 0)
   {
      if(errno == EINTR)
         continue;
      if(errno != EAGAIN)
         QuitWithError("Socket read() error");

      pollfd.revents = 0;
      int status;
      while((status = poll(&pollfd, 1, -1)) < 0)
      {
         if(errno != EINTR)
            QuitWithError("Socket poll() error");
      }

      if(status == 0 || !(pollfd.revents & POLLIN))
         QuitWithError("Weird socket poll() error");
      errno = 0;
   }
   return n;
}

void MaybeTransferLevel(void)
{
   if(!g_bNetPlay)
      return;

   char level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 4];

   if(g_bNetHost)
   {
      for(int col = 0; col < LEVEL_WIDTH; ++col)
      {
         for(int row = 0; row < LEVEL_HEIGHT; ++row)
            level_data[col + row * LEVEL_WIDTH] = (char)g_anLevel[col][row];
      }
      level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 0] = (char)g_nP1StartX;
      level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 1] = (char)g_nP1StartY;
      level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 2] = (char)g_nP2StartX;
      level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 3] = (char)g_nP2StartY;

      SocketWrite(g_sd, level_data, LEVEL_WIDTH * LEVEL_HEIGHT + 4);
   }
   else
   {
      for(int i = 0;
          i < LEVEL_WIDTH * LEVEL_HEIGHT + 4;
          i += SocketRead(g_sd, level_data + i, LEVEL_WIDTH * LEVEL_HEIGHT + 4 - i))
         ;

      for(int col = 0; col < LEVEL_WIDTH; ++col)
      {
         for(int row = 0; row < LEVEL_HEIGHT; ++row)
            g_anLevel[col][row] = (int)level_data[col + row * LEVEL_WIDTH];
      }
      g_nP1StartX = (int)level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 0];
      g_nP1StartY = (int)level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 1];
      g_nP2StartX = (int)level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 2];
      g_nP2StartY = (int)level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 3];
   }
}
