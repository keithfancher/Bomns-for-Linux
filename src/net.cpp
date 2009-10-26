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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <SDL/SDL.h>
#include "bomns.h"
#include "level.h"
#include "net.h"


#define PROTO     "BfLv1\n"
#define PROTO_LEN (sizeof(PROTO)-sizeof(char))

#define CMD_LEVEL      0x6c766c0a  /*"lvl\n"*/
#define CMD_GO         0x676f210a  /*"go!\n"*/
#define CMD_MOVE_UP    0x6d76750a  /*"mvu\n"*/
#define CMD_MOVE_DOWN  0x6d76640a  /*"mvd\n"*/
#define CMD_MOVE_LEFT  0x6d766c0a  /*"mvl\n"*/
#define CMD_MOVE_RIGHT 0x6d76720a  /*"mvr\n"*/
#define CMD_DROP_BOMN  0x626d6e0a  /*"bmn\n"*/


//Yum!  MSG!
typedef enum MSG
{
   MSG_LEVEL,
   MSG_WAITING,

} MSG;


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
      QuitWithError("Short socket write()\n");
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

static void BadCommand(uint32_t cmd, uint32_t expected_cmd)
   {
      char err[256];

   if(expected_cmd)
      snprintf(err, sizeof(err), "NET: Error: expecting command %c%c%c, got %c%c%c\n",
               (expected_cmd >> 24) & 0xff, (expected_cmd >> 16) & 0xff, (expected_cmd >> 8) & 0xff,
               (cmd >> 24) & 0xff, (cmd >> 16) & 0xff, (cmd >> 8) & 0xff);
   else
      snprintf(err, sizeof(err), "NET: Error: bad command %c%c%c\n",
               (cmd >> 24) & 0xff, (cmd >> 16) & 0xff, (cmd >> 8) & 0xff);

      QuitWithError(err);
   }

static void SendCommand(uint32_t cmd)
{
fprintf(stderr, "NET: Sending command %c%c%c\n", (cmd >> 24) & 0xff, (cmd >> 16) & 0xff, (cmd >> 8) & 0xff);
   cmd = htonl(cmd);
   SocketWriteNonblocking(g_sd, (char *)&cmd, 4);
}

static void ExpectCommand(uint32_t cmd, int timeout_ms)
{
   uint32_t received;
   SocketReadN(g_sd, (char *)&received, 4, timeout_ms);
   received = ntohl(received);
fprintf(stderr, "NET: Received expected command %c%c%c\n", (received >> 24) & 0xff, (received >> 16) & 0xff, (received >> 8) & 0xff);
   if(received != cmd)
      BadCommand(received, cmd);
}

static uint32_t ReceiveCommand(void)
{
   static uint32_t cmd;
   static int cmd_len = 0;

   cmd_len += SocketReadNonblocking(g_sd, (char *)&cmd + cmd_len, 4 - cmd_len);
   if(cmd_len >= 4)
   {
      cmd = ntohl(cmd);
      cmd_len = 0;
fprintf(stderr, "NET: Received command %c%c%c\n", (cmd >> 24) & 0xff, (cmd >> 16) & 0xff, (cmd >> 8) & 0xff);
      return cmd;
   }
   return (uint32_t)0;
}

static void DrawMessage(MSG msg)
{
   SDL_Rect rcSrc  = {0, (int)msg * 24, 1000, 24};
   SDL_Rect rcDest = {350, 288, 0, 0};
   SDL_FillRect(g_psdlsScreen, NULL, 0);
   SDL_BlitSurface(g_psdlsNetMsgs, &rcSrc, g_psdlsScreen, &rcDest);
   SDL_Flip(g_psdlsScreen);
}

// In which we do the necessary shit to get a connected socket.
void MaybeStartNetplay(void)
{
   if(!g_bNetPlay)
      return;

   fprintf(stderr, "Netplay!  Awesome!\n");

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
      QuitWithError("wrong protocol versions!\n");

   g_sd = sd;

   fprintf(stderr, "ok!\n");
}

void MaybeTransferLevel(void)
{
   if(!g_bNetPlay)
      return;

   char level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 4];

   DrawMessage(MSG_LEVEL);

   if(g_bNetHost)
   {
      for(int col = 0; col < LEVEL_WIDTH; ++col)
      {
         for(int row = 0; row < LEVEL_HEIGHT; ++row)
            level_data[col + row * LEVEL_WIDTH] = (char)g_anLevel[col][row];
      }
      level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 0] = (char)(g_nP1StartX / 10);
      level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 1] = (char)(g_nP1StartY / 10);
      level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 2] = (char)(g_nP2StartX / 10);
      level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 3] = (char)(g_nP2StartY / 10);

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
      g_nP1StartX = (int)level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 0] * 10;
      g_nP1StartY = (int)level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 1] * 10;
      g_nP2StartX = (int)level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 2] * 10;
      g_nP2StartY = (int)level_data[LEVEL_WIDTH * LEVEL_HEIGHT + 3] * 10;
   }
}

void MaybeWaitForOpponent(void)
{
   if(!g_bNetPlay)
      return;

   DrawMessage(MSG_WAITING);

   fprintf(stderr, "NET: Waiting for opponent...");
   SendCommand(CMD_GO);
   ExpectCommand(CMD_GO, -1);
   fprintf(stderr, "ok!\n");
}

static void HandleReceivedKeypress(uint32_t cmd)
{
   switch(cmd)
   {
   case CMD_MOVE_UP:
      ProcInput(g_bNetHost ? SDLK_w : SDLK_UP);
      break;
   case CMD_MOVE_DOWN:
      ProcInput(g_bNetHost ? SDLK_s : SDLK_DOWN);
      break;
   case CMD_MOVE_LEFT:
      ProcInput(g_bNetHost ? SDLK_a : SDLK_LEFT);
      break;
   case CMD_MOVE_RIGHT:
      ProcInput(g_bNetHost ? SDLK_d : SDLK_RIGHT);
      break;
   case CMD_DROP_BOMN:
      ProcInput(g_bNetHost ? SDLK_SPACE : SDLK_RETURN);
      break;
   }
}

void MaybeReceiveKeypress(void)
{
   if(!g_bNetPlay)
      return;

   for(uint32_t cmd; (cmd = ReceiveCommand()) != (uint32_t)0; )
   {
      switch(cmd)
      {
      case CMD_MOVE_UP:
      case CMD_MOVE_DOWN:
      case CMD_MOVE_LEFT:
      case CMD_MOVE_RIGHT:
      case CMD_DROP_BOMN:
         HandleReceivedKeypress(cmd);
         break;

      default:
         BadCommand(cmd, 0);
         break;
      }
   }
}

bool MaybeSendKeypress(SDLKey key)
{
   if(!g_bNetPlay)
      return false;

   switch(key)
   {
   case SDLK_RIGHT:
      if(!g_bNetHost)
         return true;
      SendCommand(CMD_MOVE_RIGHT);
      return false;

   case SDLK_LEFT:
      if(!g_bNetHost)
         return true;
      SendCommand(CMD_MOVE_LEFT);
      return false;

   case SDLK_UP:
      if(!g_bNetHost)
         return true;
      SendCommand(CMD_MOVE_UP);
      return false;

   case SDLK_DOWN:
      if(!g_bNetHost)
         return true;
      SendCommand(CMD_MOVE_DOWN);
      return false;

   case SDLK_RETURN:
      if(!g_bNetHost)
         return true;
      SendCommand(CMD_DROP_BOMN);
      return false;

   case SDLK_d:
      if(g_bNetHost)
         return true;
      SendCommand(CMD_MOVE_RIGHT);
      return false;

   case SDLK_a:
      if(g_bNetHost)
         return true;
      SendCommand(CMD_MOVE_LEFT);
      return false;

   case SDLK_w:
      if(g_bNetHost)
         return true;
      SendCommand(CMD_MOVE_UP);
      return false;

   case SDLK_s:
      if(g_bNetHost)
         return true;
      SendCommand(CMD_MOVE_DOWN);
      return false;

   case SDLK_SPACE:
      if(g_bNetHost)
         return true;
      SendCommand(CMD_DROP_BOMN);
      return false;
   }

   return false;
}
