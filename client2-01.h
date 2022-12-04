#ifndef CLIENT_H
#define CLIENT_H

#include "server2.h"

#define MAX_MEMBERS 10

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
}Client;

typedef struct
{
char name[BUF_SIZE];
char message[1024];

}Historique;


typedef struct
{
   char name[BUF_SIZE];
   Client members[MAX_MEMBERS];
   int nb_members;
}Group;

#endif /* guard */
