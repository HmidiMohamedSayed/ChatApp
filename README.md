# Application-chat
Application de chat développé en C.

**Liste des fonctionnalitées**:

 * afficher les messages globaux(conversation globale entre tous les utilisateurs)
 * afficher les messages d'un groupe particulier(si l'utilisateur appartient au group)
 * afficher les messages privés avec un utilisateur particulier
 * envoyer un message global
 * envoyer un message a un group(si le groupe existe)
 * envoyer un message privé
 * sauvegarder l'historique(à chaque modification)
 * restaurer l'historique(à chaque lancement du serveur)
 
## Compilation
### Serveur
La compilation du code serveur est assuré par un `Makefile` dans le dossier "Server".
le lancement est assuré par la commande "make run".
```
cd Server
make
make run
```
 
### Client
La compilation du code client est assuré par un `Makefile` dans le dossier "Client".
le lancement est assuré par la commande "./client Server_Addresse  Username".
```
cd Client
make
./client 127.0.0.1 useri
```

> **Note**: La création de groupe et l'ajout d'utilisateurs n'étant pas implémentés, un groupe "group1" avec 2 utilisateurs "user1" et "user2" est disponible pour la démo.

## TODO
**Liste TODO**:
 * list connected users
 * list groups
 * list my groups' users
 * join a group
 * left a group
 * create a group
 * add a user to a group
 * remove a user from a group
 * authentification system
 * message encryptage

## SCENARIO D'EXECUTION

### Menu (l'utilisateur connecté est user1)

  ![menu](https://user-images.githubusercontent.com/79282911/205509228-99faf00d-adbb-4ba5-aaea-869863873848.PNG)
  
### envoyer un message global


![4](https://user-images.githubusercontent.com/79282911/205509745-af18e1ba-f5f9-4fcd-a745-b55919468538.PNG)


### envoyer un message privé

![5](https://user-images.githubusercontent.com/79282911/205513063-ba159212-7250-4f19-8e35-fe5b470f8af5.PNG)


### afficher les messages globaux(conversation globale entre tous les utilisateurs)


![1](https://user-images.githubusercontent.com/79282911/205513138-53938bfe-199a-4a77-86aa-735619fc5fc5.PNG)

### afficher les messages privés avec un utilisateur particulier

![3](https://user-images.githubusercontent.com/79282911/205513211-1dd183fd-aefc-4fa6-b080-c2825327aa96.PNG)

### envoyer un message a un group(si le groupe existe)


![6](https://user-images.githubusercontent.com/79282911/205513311-6f18c5c3-bfd8-4757-9205-b4b4b00ab02c.PNG)

### afficher les messages d'un groupe particulier(si l'utilisateur appartient au group)

![2](https://user-images.githubusercontent.com/79282911/205513372-9c6962c1-8a2b-465a-bafe-fefb9e6b9a5d.PNG)

  
  
  ### sauvegarder l'historique(à chaque modification)
  
  ![hist](https://user-images.githubusercontent.com/79282911/205513459-0a2fe95f-2e64-4053-92ef-bf4c00ee9b0a.PNG)

  ### restaurer l'historique(à chaque lancement du serveur)
  
   **ETAPES**:
  * nouveau client se connecte(user2)
            
  ![new client](https://user-images.githubusercontent.com/79282911/205513553-035cb7d3-3946-4f29-a399-238f02c86697.PNG)
          
  * consulte les messages globaux 
        
        
  ![99](https://user-images.githubusercontent.com/79282911/205513637-a492a94b-8e86-4437-a8ba-89be5d9d85bb.PNG)

          
          
  
  
  
 

 

