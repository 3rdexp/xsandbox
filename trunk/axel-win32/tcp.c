  /********************************************************************\
  * Axel -- A lighter download accelerator for Linux and other Unices. *
  *                                                                    *
  * Copyright 2001 Wilmer van der Gaast                                *
  \********************************************************************/

/* TCP control file							*/

/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License with
  the Debian GNU/Linux distribution in file /usr/doc/copyright/GPL;
  if not, write to the Free Software Foundation, Inc., 59 Temple Place,
  Suite 330, Boston, MA  02111-1307  USA
*/

#include "axel.h"

/* Get a TCP connection */
#if WIN32
SOCKET tcp_connect(char *hostname, int port, char *local_if)
#else
int tcp_connect(char *hostname, int port, char *local_if)
#endif
{
	struct hostent *host = NULL;
	struct sockaddr_in addr;
	struct sockaddr_in local;
#if WIN32
	SOCKET fd;
#else
	int fd;
#endif

#ifdef DEBUG
#if WIN32
	size_t i = sizeof(local);	/* POSIX changed to size_t */
#else
	socklen_t i = sizeof(local);
#endif
	fprintf(stderr, "tcp_connect(%s, %i) = ", hostname, port);
#endif

#if WIN32
	if (NULL == (host = gethostbyname(hostname))) 
	{
#ifdef DEBUG
		printf("DEBUG gethostbyname error %d\n", WSAGetLastError());
#endif
		return INVALID_SOCKET;
	}
#else
	/* Why this loop? Because the call might return an empty record.
	   At least it very rarely does, on my system... */
	for (fd = 0; fd < 5; fd++)
	{
		if (NULL == (host = gethostbyname(hostname))) 
		{
			return -1;
		}
		if (*host->h_name) 
		{
			break;
		}
	}
	if (!host || !host->h_name || !*host->h_name) 
	{
		return -1;
	}
#endif

#if WIN32
	if (INVALID_SOCKET == (fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) 
	{
#ifdef DEBUG
		printf("DEBUG socket error %d\n", WSAGetLastError());
#endif
		return INVALID_SOCKET;
	}
#else
	if (-1 == (fd = socket(AF_INET, SOCK_STREAM, 0))) 
	{
		return -1;
	}
#endif
	
	if (local_if && *local_if)
	{
		local.sin_family = AF_INET;
		local.sin_port = 0;
		local.sin_addr.s_addr = inet_addr(local_if);
#if WIN32
		if (SOCKET_ERROR == bind(fd, (struct sockaddr *)&local, sizeof(struct sockaddr_in))) 
		{
#ifdef DEBUG
			printf("DEBUG bind error %d\n", WSAGetLastError());
#endif
			closesocket(fd);
			return INVALID_SOCKET;
		}
#else
		if (-1 == bind(fd, (struct sockaddr *) &local, sizeof(struct sockaddr_in)))
		{
			close( fd );
			return -1;
		}
#endif
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr = *((struct in_addr *)host->h_addr);
	
#if WIN32
	if (SOCKET_ERROR == connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))) 
	{
#ifdef DEBUG
		printf("DEBUG connect error %d\n", WSAGetLastError());
#endif
		closesocket(fd);
		return INVALID_SOCKET;
	}
#else
	if (-1 == connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
	{
		close(fd);
		return -1;
	}
#endif

#ifdef DEBUG
	getsockname(fd, &local, &i);
	fprintf(stderr, "%i\n", ntohs(local.sin_port));
#endif
	
	return fd;
}

/* FIXME: can not get ip under WIN32 */
int get_if_ip(char *iface, char *ip)
{
#if WIN32
	SOCKET fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	u_long iMode = 0;
	if (0 == ioctlsocket(fd, FIONBIO, &iMode)) 
	{
		return 1;
	} 
	else 
	{
		return 0;
	}
#else
	struct ifreq ifr;
	int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	memset(&ifr, 0, sizeof(struct ifreq));
	
	strcpy(ifr.ifr_name, iface);
	ifr.ifr_addr.sa_family = AF_INET;
	if (0 == ioctl(fd, SIOCGIFADDR, &ifr))
	{
		struct sockaddr_in *x = (struct sockaddr_in *)&ifr.ifr_addr;
		strcpy(ip, inet_ntoa(x->sin_addr));
		return 1;
	}
	else
	{
		return 0;
	}
#endif
}
