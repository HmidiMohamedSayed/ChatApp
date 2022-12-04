#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "server2.h"
#include "client2.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

static void app(void)
{
   SOCKET sock = init_connection();
   char buffer[BUF_SIZE];
   /* the index for the array */
   int actual = 0;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];
   int n=0;
   Historique historique[1024];
   fd_set rdfs;
   ssize_t read;
   char* line;
   char name[1024];
   char msg1[1024];
   size_t len = 0;
   FILE *file;
   if ((file = fopen("historique.txt","r")) != NULL){
      while ((read = getline(&line, &len, file)) != -1) {
            sscanf(line,"%s",name);
            memcpy( msg1, &line[strlen(name)+2], strlen(line)-strlen(name)-3 );
            msg1[strlen(line)-strlen(name)-3] = '\0';
            printf("********************%s***************** %s\n",name,msg1);
            strcpy(historique[n].name,name);
            strcpy(historique[n].message,msg1);
            n++;
            printf("$$$$\n");


    }
    

    fclose(file);
   }

   
   //fclose(file);

   int nb_groups = 1;
   Group groups[3];

   strcpy(groups[0].name,"group1");
   groups[0].nb_members = 0;
   
   char response[BUF_SIZE];
   char msg[BUF_SIZE];
   char receiver[BUF_SIZE];
   char keyword[BUF_SIZE];
   // clear the buffer
   msg[0]='\0';
   receiver[0] = '\0';
   keyword[0]='\0';

   while(1)
   {
      int i = 0;
     
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         /* stop process when type on keyboard */
         break;
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };
         size_t sinsize = sizeof csin;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         
         printf("A new client is online\n");
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            continue;
         }

         /* after connecting the client sends its name */
         if(read_client(csock, buffer) == -1)
         {
            /* disconnected */
            continue;
         }

         /* what is the new maximum fd ? */
         max = csock > max ? csock : max;

         FD_SET(csock, &rdfs);

         Client c = { csock };
         strncpy(c.name, buffer, BUF_SIZE - 1);
         clients[actual] = c;
         actual++;
         if(!strcmp(c.name,"user1") || !strcmp(c.name,"user2") ){
         strcpy(groups[0].members[groups[0].nb_members].name, clients[actual-1].name);
         groups[0].members[groups[0].nb_members].sock = clients[actual-1].sock;
         groups[0].nb_members++;
         }
         char t[1024];
         if(n>0){
            for(int i=0;i<n;i++){
            sscanf(historique[i].message,"%s \" %[^\"] \"%s",keyword,msg,receiver);

            if(!strcmp(keyword,"msg")){
               if(!strcmp(receiver,clients[actual-1].name) || !strcmp(historique[i].name,clients[actual-1].name)){
               strncpy(t, historique[i].name, BUF_SIZE - 1);
               strncat(t, " : ", sizeof t - strlen(t) - 1);
               strncat(t, historique[i].message, sizeof t - strlen(t) - 1);
               strncat(t, "\n", sizeof t - strlen(t) - 1);
               write_client(csock,t);
               }
            }
            else if (!strcmp(keyword,"grp")){
                     int grp_index=-1;
                  for (int i=0;i<nb_groups;i++){
                        if(!strcmp(groups[i].name,receiver)){
                           grp_index=i;
                           break;
                        }
                     }
                     if(grp_index!=-1){
                        int verif=0;
                        for(int j=0;j<groups[grp_index].nb_members;j++){
                           if(!strcmp(groups[grp_index].members[j].name,clients[actual-1].name)){
                              verif=1;
                           }
                        }
                        if(verif==1){
                        strncpy(t, historique[i].name, BUF_SIZE - 1);
                        strncat(t, " : ", sizeof t - strlen(t) - 1);
                        strncat(t, historique[i].message, sizeof t - strlen(t) - 1);
                        strncat(t, "\n", sizeof t - strlen(t) - 1);
                        write_client(csock,t);
                        }
                        
                     }
            }

            else{
                        strncpy(t, historique[i].name, BUF_SIZE - 1);
                        strncat(t, " : ", sizeof t - strlen(t) - 1);
                        strncat(t, historique[i].message, sizeof t - strlen(t) - 1);
                        strncat(t, "\n", sizeof t - strlen(t) - 1);
                        write_client(csock,t);
            }
            }
            
         }
      }
      else
      {
         int i = 0;
         for(i = 0; i < actual; i++)
         {
            /* a client is talking */
            if(FD_ISSET(clients[i].sock, &rdfs))
            {
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);
               /* client disconnected */
               if(c == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !\n", BUF_SIZE - strlen(buffer) - 1);
                  send_message_to_all_clients(clients, client, actual, buffer, 1);
               }
               else
               {
                  

                  sscanf(buffer,"%s \" %[^\"] \"%s",keyword,msg,receiver);
                  //strncat(buffer, "\n", sizeof buffer - strlen(buffer) - 1);

                  if (!strcmp(keyword,"msg")){

                     int clt_index = -1;  
                     for (int i=0;i<actual;i++){
                        if(!strcmp(clients[i].name,receiver)){
                           clt_index=i;
                           break;
                        }
                     }

                     if ( clt_index == -1 ) 
                     {
                     write_client(clients[i].sock,"client not found!");
                     }
                     else{

                     send_message_to_one_client(clients, client, receiver, actual, buffer, 0);
                     strncpy(historique[n].name,client.name, 1024-1);
                     strncpy(historique[n].message,buffer,1024-1);
                     n++;
                     file = fopen("historique.txt","w");
                     for(int i=0;i<n;i++){
                     printf("%s : %s\n",historique[i].name,historique[i].message);
                     fprintf(file,"%s : %s\n",historique[i].name,historique[i].message);
                     }
                  fclose(file);

                     }



                  }

                  else if (!strcmp(keyword,"grp")){
                     
                     //todo: handle group not existing
                     int grp_index = -1;  
                     for (int i=0;i<nb_groups;i++){
                        printf("%s   %s \n",groups[i].name,receiver);
                        if(!strcmp(groups[i].name,receiver)){
                           grp_index=i;
                           break;
                        }
                     }
                     
                     if ( grp_index == -1 ) 
                     write_client(clients[i].sock,"group not found!");
                     else{
                        int verif=0;
                        for(int j=0;j<groups[grp_index].nb_members;j++){
                           if(!strcmp(groups[grp_index].members[j].name,client.name)){
                              verif=1;
                           }
                        }
                        if(verif==1){
                           send_message_to_one_group(groups[grp_index].members, client, groups[grp_index].nb_members, buffer, 0);
                           strncpy(historique[n].name,client.name, 1024-1);
                           strncpy(historique[n].message,buffer,1024-1);
                  //fprintf(file,"%s : %s\n",historique[n].name,historique[n].message);
                  n++;
                  file = fopen("historique.txt","w");
                  for(int i=0;i<n;i++){
                     printf("%s : %s\n",historique[i].name,historique[i].message);
                     fprintf(file,"%s : %s\n",historique[i].name,historique[i].message);
                  }
                  fclose(file);
                        }
                        else{
                           write_client(clients[i].sock,"access denied,you should join the group");
                        }
                        
                     }

                  }
                  else
                  {
                     send_message_to_all_clients(clients, client, actual, buffer, 0);
                     strncpy(historique[n].name,client.name, 1024-1);
                  strncpy(historique[n].message,buffer,1024-1);
                  //fprintf(file,"%s : %s\n",historique[n].name,historique[n].message);
                  n++;
                  file = fopen("historique.txt","w");
                  for(int i=0;i<n;i++){
                     printf("%s : %s\n",historique[i].name,historique[i].message);
                     fprintf(file,"%s : %s\n",historique[i].name,historique[i].message);
                  }
                  fclose(file);
                  }
               }
               break;
            }
         }
      }
   }
   fclose(file);

   clear_clients(clients, actual);
   end_connection(sock);
}

static void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
      {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
   }
}

static void send_message_to_one_client(Client *clients, Client sender, char *receiver, int actual, const char *buffer, char from_server){
 
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      if(!strcmp(receiver ,clients[i].name))
      {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
         return;
      }
   }
   strcpy(message,"user not found\n\r\n");
   write_client(sender.sock, message);

}

static void send_message_to_one_group(Client *members, Client sender, int nb_members, const char *buffer, char from_server){
 
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < nb_members; i++)
   {
         printf("nb members:%d\n/ name: %s\n",nb_members,members[i].name);
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(members[i].sock, message);
         
   }
   

}

static int init_connection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;

   return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

int main(int argc, char **argv)
{
   init();

   app();

   end();

   return EXIT_SUCCESS;
}
