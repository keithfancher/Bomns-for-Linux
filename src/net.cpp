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


#define PROTO     "BfLv1\n"
#define PROTO_LEN (sizeof(PROTO)-sizeof(char))

#define CMD_LEVEL "lvl\n"
#define CMD_GO    "go!\n"


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

static void SocketWriteNonblocking(int sd, const char * buf, int len)
{
   int n;
   while((n = write(sd, buf, len)) < 0)
   {
      if(errno == EINTR)
         continue;
      QuitWithErrorErrno("Socket nonblocking write() error", errno);
   }
   if(n != len)
      QuitWithError("Short socket write()");
   }

static int SocketReadNonblocking(int sd, char * buf, int len)
   {
   int n;
   while((n = read(sd, buf, len)) < 0)
   {
      if(errno == EINTR)
         continue;
      if(errno == EAGAIN)
      {
         n = 0;
         break;
   }
      QuitWithErrorErrno("Socket nonblocking read() error", errno);
   }
   return n;
}

//Partially taken from libesmtp... don't ask.  It's GPL-licensed, so we' cool.
//<http://www.stafford.uklinux.net/libesmtp/>.
static void SocketWriteBlocking(int sd, const char * buf, int len, int timeout_ms)
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
            QuitWithErrorErrno("Socket write() error", errno);

         pollfd.revents = 0;
         int status;
         while((status = poll(&pollfd, 1, timeout_ms)) < 0)
         {
            if(errno != EINTR)
               QuitWithErrorErrno("Socket poll() error", errno);
         }

         if(status == 0 || !(pollfd.revents & POLLOUT))
            QuitWithErrorErrno("Socket poll() error", ETIMEDOUT);
         errno = 0;
      }
   }
}

//Also from libesmtp.
static int SocketReadBlocking(int sd, char * buf, int len, int timeout_ms)
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
         QuitWithErrorErrno("Socket read() error", errno);

      pollfd.revents = 0;
      int status;
      while((status = poll(&pollfd, 1, timeout_ms)) < 0)
      {
         if(errno != EINTR)
            QuitWithErrorErrno("Socket poll() error", errno);
      }

      if(status == 0 || !(pollfd.revents & POLLIN))
         QuitWithErrorErrno("Socket poll() error", ETIMEDOUT);
      errno = 0;
   }
   return n;
}

static void SocketReadN(int sd, char * buf, int len, int timeout_ms)
{
   for(int total = 0;
       total < len;
       total += SocketReadBlocking(sd, buf + total, len - total, timeout_ms))
      ;
}

#define SendCommand(cmd) SocketWriteNonblocking(g_sd, cmd, 4)

static void ExpectCommand(const char * cmd, int timeout_ms)
{
   char received[5];
   SocketReadN(g_sd, received, 4, timeout_ms);
   received[4] = '\0';
   if(strcmp(cmd, received))
   {
      char err[256];
      snprintf(err, sizeof(err), "NET: Error: expecting command %c%c%c, got %c%c%c\n",
               cmd[0], cmd[1], cmd[2], received[0], received[1], received[2]);
      QuitWithError(err);
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
      QuitWithErrorErrno("socket() failed", errno);

   if(g_bNetHost)
   {
      fprintf(stderr, "NET: Listening...");

      if(bind(sd, &g_addr, sizeof(g_addr)))
         QuitWithErrorErrno("bind() failed", errno);
      if(listen(sd, 1))
         QuitWithErrorErrno("listen() failed", errno);

      struct sockaddr remote_addr;
      socklen_t remote_addr_len = sizeof(remote_addr);
      int sd2;
      if((sd2 = accept(sd, &remote_addr, &remote_addr_len)) < 0)
         QuitWithErrorErrno("accept() failed", errno);
      close(sd);
      sd = sd2;
   }
   else
   {
      fprintf(stderr, "NET: Connecting...");

      if(connect(sd, &g_addr, sizeof(g_addr)))
         QuitWithErrorErrno("connect() failed", errno);
   }

   // Also, we need our newly-connected socket to be non-blocking, which means
   // setting the O_NONBLOCK flag.
   int old_fl, new_fl;
   if((old_fl = fcntl(sd, F_GETFL)) < 0)
      QuitWithErrorErrno("fcntl(F_GETFL) failed", errno);

   new_fl = old_fl | O_NONBLOCK;
   if(new_fl != old_fl)
   {
      if(fcntl(sd, F_SETFL, new_fl) < 0)
         QuitWithErrorErrno("fcntl(F_SETFL) failed", errno);
   }

   SocketWriteNonblocking(sd, PROTO, PROTO_LEN);

   char proto[PROTO_LEN + 1];
   SocketReadN(sd, proto, PROTO_LEN, 10 * 1000);
   proto[PROTO_LEN] = '\0';
   if(strcmp(proto, PROTO))
      QuitWithError("wrong protocol versions!");

   g_sd = sd;

   fprintf(stderr, "ok!\n");
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

      fprintf(stderr, "NET: Sending level...");
      SendCommand(CMD_LEVEL);
      SocketWriteBlocking(g_sd, level_data, LEVEL_WIDTH * LEVEL_HEIGHT + 4, -1);
      fprintf(stderr, "ok!\n");
   }
   else
   {
      fprintf(stderr, "NET: Receiving level...");
      ExpectCommand(CMD_LEVEL, -1);
      SocketReadN(g_sd, level_data, LEVEL_WIDTH * LEVEL_HEIGHT + 4, -1);
      fprintf(stderr, "ok!\n");

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

void MaybeWaitForOpponent(void)
{
   if(!g_bNetPlay)
      return;

   fprintf(stderr, "NET: Waiting for opponent...");
   SendCommand(CMD_GO);
   ExpectCommand(CMD_GO, -1);
   fprintf(stderr, "ok!\n");
}
