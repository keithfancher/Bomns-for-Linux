#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "net.h"


// Whether we are doing any kind of netplay at all.
bool g_bNetPlay = false;

// If g_bNetPlay, true if we are the listener/host/master, false if we are the
// connector/client/slave.
bool g_bNetHost = false;

// If hosting, the local IP/port to bind to; if connecting, the remote IP/port
// to connect to.
static struct sockaddr g_addr;


static void NetError(const char * s)
{
   perror(s);
   exit(1);
}

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
         for(int j = i; argv[j]; ++j)
            argv[j] = argv[j+2];
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
      NetError("socket()");

   if(g_bNetHost)
   {
      printf("Listening...");

      if(bind(sd, &g_addr, sizeof(g_addr)))
         NetError("bind()");
      if(listen(sd, 1))
         NetError("listen()");

      struct sockaddr remote_addr;
      socklen_t remote_addr_len;
      int sd2;
      if((sd2 = accept(sd, &remote_addr, &remote_addr_len)) < 0)
         NetError("accept()");
      close(sd);
      sd = sd2;
   }
   else
   {
      printf("Connecting...");

      if(connect(sd, &g_addr, sizeof(g_addr)))
         NetError("connect()");
   }

   // Also, we need our newly-connected socket to be non-blocking, which means
   // setting the O_NONBLOCK flag.
   int old_fl, new_fl;
   if((old_fl = fcntl(sd, F_GETFL)) < 0)
      NetError("fcntl(F_GETFL)");

   new_fl = old_fl | O_NONBLOCK;
   if(new_fl != old_fl)
   {
      if(fcntl(sd, F_SETFL, new_fl) < 0)
         NetError("fcntl(F_SETFL)");
   }

   printf("ok!\n");
}
