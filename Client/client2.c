#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

static void app(const char *address, const char *name)
{
   SOCKET sock = init_connection(address);
   char buffer[BUF_SIZE];
   char welcome[1024];
   fd_set rdfs;
   char messages[100][1024];
   int nb=0;
   int choix=0;
   char groupname[30];
   char name1[1024];
   char msg1[1024];
   char msg[1024];
   char keyword[30];
   char receiver[30];
   char t[1024];

   /* send our name */
   write_server(sock, name);

   while(1)
   {
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the socket */
      FD_SET(sock, &rdfs);

      if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      if(FD_ISSET(sock, &rdfs))
      {
         int n = read_server(sock, buffer);
         /* server down */
         if(n == 0)
         {
            printf("Server disconnected !\n");
            break;
         }
         //buffer[n]=0;
         //printf("%s",buffer);
         //buffer[0]=0;
         char* var=strtok(buffer,"\n");
         
         while(var != NULL){
            strcpy(messages[nb],var);
            nb++;
            var=strtok(NULL,"\n");
            //i=i+strlen(var);
         }
         
         //puts(buffer);
      }
      /* something from standard input : i.e keyboard */
      display_menu();
      printf("enter a valid number:\n");
      scanf("%d",&choix);
      fflush(stdin);
      switch(choix){
         case 0 : return;
         case 1 : printf("******* global chat *******\n");;
                  for(int i=0;i<nb;i++){
                     sscanf(messages[i],"%s",name1);
                     memcpy( msg1, &messages[i][strlen(name1)+2], strlen(messages[i])-strlen(name1)-2 );
                     msg1[strlen(messages[i])-strlen(name1)-2] = '\0';
                     //puts(msg1);
                     //name1[4]='\0';
                     sscanf(msg1,"%s \" %[^\"] \"%s",keyword,msg,receiver);
                     if(strcmp(keyword,"grp") && strcmp(keyword,"msg")){
                     
                        strncpy(t, name1, BUF_SIZE - 1);
                        strncat(t, " : ", sizeof t - strlen(t) - 1);
                        strncat(t, msg1, sizeof t - strlen(t) - 1);
                        //strncat(t, "\n", sizeof t - strlen(t) - 1);
                        puts(t);
                     }
                  

                  };printf("****************\n");printf("press enter to return to menu\n");break;
         case 2 : printf("enter a group name:\n");
                  scanf("%s",groupname);
                  fflush(stdin);
                  printf("******* group : %s *******\n",groupname);
                  for(int i=0;i<nb;i++){
                     //printf("%%%%%%%%%% %s \n",messages[i]);
                     sscanf(messages[i],"%s",name1);
                     memcpy( msg1, &messages[i][strlen(name1)+2], strlen(messages[i])-strlen(name1)-2 );
                     msg1[strlen(messages[i])-strlen(name1)-2] = '\0';
                     //puts(msg1);
                     //name1[4]='\0';
                     sscanf(msg1,"%s \" %[^\"] \"%s",keyword,msg,receiver);
                     if(!strcmp(keyword,"grp")){
                     if(!strcmp(receiver,groupname)){
                        strncpy(t, name1, BUF_SIZE - 1);
                        strncat(t, " : ", sizeof t - strlen(t) - 1);
                        strncat(t, msg, sizeof t - strlen(t) - 1);
                        //strncat(t, "\n", sizeof t - strlen(t) - 1);
                        puts(t);
                     }
                     }
                  

                  } printf("****************\n");printf("press enter to return to menu\n");break;
               case 3: printf("enter a user name:\n");
                  scanf("%s",groupname);
                  fflush(stdin);
                  printf("******* user : %s *******\n",groupname);
                  for(int i=0;i<nb;i++){
                     sscanf(messages[i],"%s",name1);
                     memcpy( msg1, &messages[i][strlen(name1)+2], strlen(messages[i])-strlen(name1)-2 );
                     msg1[strlen(messages[i])-strlen(name1)-2] = '\0';
                     //puts(msg1);
                     //name1[4]='\0';
                     sscanf(msg1,"%s \" %[^\"] \"%s",keyword,msg,receiver);
                     if(!strcmp(keyword,"msg")){
                     if((!strcmp(receiver,groupname) && !strcmp(name1,name))|| (!strcmp(name1,groupname)&& !strcmp(receiver,name)) ){
                        strncpy(t, name1, BUF_SIZE - 1);
                        strncat(t, " : ", sizeof t - strlen(t) - 1);
                        strncat(t, msg, sizeof t - strlen(t) - 1);
                        //strncat(t, "\n", sizeof t - strlen(t) - 1);
                        puts(t);
                     }
                     }
                  

                  } printf("****************\n");printf("press enter to return to menu\n");break;
               case 4 : printf("write your message \n");
                        char temp;
                        scanf("%c",&temp);
                        scanf("%[^\n]",msg1);
                        
                        printf("******%s\n",msg1);
            
       
                  write_server(sock,msg1);
                  strncpy(messages[nb],name, BUF_SIZE - 1);
                        strncat(messages[nb], " : ", sizeof messages[nb] - strlen(messages[nb]) - 1);
                        strncat(messages[nb], msg1, sizeof messages[nb] - strlen(messages[nb]) - 1);
                        strncat(messages[nb], "\n", sizeof messages[nb] - strlen(messages[nb]) - 1);
                        nb++;
                  printf("your message was sent successfully\n");break;
      
               case 5 : printf("enter a user name\n");
                        fflush(stdin);
                        scanf("%s",name1);
                        printf("write your message \n");
                        fflush(stdin);
                        char temp1;
                        scanf("%c",&temp1);
                        scanf("%[^\n]",msg1);
                        strncpy(msg,"msg", BUF_SIZE - 1);
                        strncat(msg, " \"", sizeof msg - strlen(msg) - 1);
                        strncat(msg, msg1, sizeof msg - strlen(msg) - 1);
                        strncat(msg, "\" ", sizeof msg - strlen(msg) - 1);
                        strncat(msg, name1, sizeof msg - strlen(msg) - 1);
                        strncat(msg, "\n", sizeof msg - strlen(msg) - 1);
                        
                        write_server(sock,msg);
                        strncpy(t,name, BUF_SIZE - 1);
                        strncat(t," : ", sizeof t - strlen(t) - 1);
                        strncat(t, msg, sizeof t - strlen(t) - 1);
                        //strncat(t, "\n", sizeof t - strlen(t) - 1);
                        strcpy(messages[nb],t);
                        nb++;
                        printf("your message was sent successfully\n");break;
               case 6 : printf("enter a group name \n");
                        fflush(stdin);
                        scanf("%s",name1);
                        printf("write your message \n");
                        fflush(stdin);
                        char temp2;
                        scanf("%c",&temp2);
                        scanf("%[^\n]",msg1);
                        strncpy(msg,"grp", BUF_SIZE - 1);
                        strncat(msg, " \"", sizeof msg - strlen(msg) - 1);
                        strncat(msg, msg1, sizeof msg - strlen(msg) - 1);
                        strncat(msg, "\" ", sizeof msg - strlen(msg) - 1);
                        strncat(msg, name1, sizeof msg - strlen(msg) - 1);
                        //strncat(msg, "\n", sizeof msg- strlen(msg) - 1);
                        
                        write_server(sock,msg);
                        
                        printf("your message was sent successfully\n");break;
               default : printf("invalid number\n");
      }

      
   }

   end_connection(sock);
}

static int init_connection(const char *address)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }


   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_server(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }

   buffer[n] = 0;

   return n;
}

static void write_server(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

void display_menu(){
   printf("-------------------- menu -------------------- \n");
   printf("0.quit\n");
   printf("1.display all my messages\n");
   printf("2.display a group messages\n");
   printf("3.display private messages with a user\n");
   printf("4.send message to global chat\n");
   printf("5.send private message to a specified user\n");
   printf("6.send a group message\n");
   //TODO
   /*
   printf("7.list connected users\n");
   printf("8.list groups\n");
   printf("9.list my groups' users\n");
   printf("10.join a group\n");
   printf("11.left a group\n");
   printf("12.create a group\n");
   printf("13.add a user to a group\n");
   printf("14.remove a user from a group\n");
   */
   printf("---------------------------------------------- \n");

}

void listMessages(char **messages,int n){
   for(int i=0;i<n;i++){
      printf("%s\n",messages[i]);
   }
   /*
   else if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         fgets(buffer, BUF_SIZE - 1, stdin);
         {
            char *p = NULL;
            p = strstr(buffer, "\n");
            if(p != NULL)
            {
               *p = 0;
            }
            else
            {
               //fclean 
               buffer[BUF_SIZE - 1] = 0;
            }
         }
         write_server(sock, buffer);
      }
      */
}

int main(int argc, char **argv)
{
   if(argc < 2)
   {
      printf("Usage : %s [address] [pseudo]\n", argv[0]);
      return EXIT_FAILURE;
   }

   init();

   app(argv[1], argv[2]);

   end();

   return EXIT_SUCCESS;
}
