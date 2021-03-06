/*********************************************************************
client.cpp - Programme client RAT
�crit par: Pierre-Marc Laforest
Date: 2017-03-24
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "comClientServer.h"
#include "RATFonctions.h"

#define PORT_NUMB 1030

#define CHEMIN_DOSSIER_MAX 1024
#define MAX_COMMANDE 500
#define MAX_ARGS 500

using namespace std;

char rootDirectory[1024];

int main(int argc, char **argv)
{
  //WORD wVersionRequested = MAKEWORD(1, 1);
  //WSADATA wsaData;
  SOCKET listenSocketClient, remoteSocketClient;
  int nRet;
  //short nPort;
  char dossierCourant[CHEMIN_DOSSIER_MAX];
  char commandeAExecuter[MAX_COMMANDE];
  char argumentsBuf[MAX_ARGS];
  int retTraiteCommandes = 0;
	
  // Check for port argument
  if (argc != 1) {
    cout << "\nSyntax: RATClient.exe";
    cout << endl;
    return -1;
  }

  // Initialize WinSock and check version
  /*
    nRet = WSAStartup(wVersionRequested, &wsaData);
    if (wsaData.wVersion != wVersionRequested)
    {
    cout << "Wrong version";
    return;
    }
  */

  RATClient_initComAvecServer(PORT_NUMB, &listenSocketClient, &remoteSocketClient);

  // Reception du dossier courant
  memset(dossierCourant, 0, sizeof(dossierCourant));
  receptionStrParSocket(dossierCourant, remoteSocketClient);											
  do
    {
      RATClient_envoiCommandes(dossierCourant, commandeAExecuter, sizeof(commandeAExecuter), argumentsBuf, remoteSocketClient);
      retTraiteCommandes = RATClient_traiteCommandes(commandeAExecuter, argumentsBuf, dossierCourant, remoteSocketClient);

    }while (retTraiteCommandes != -1);

  fermerCom(&remoteSocketClient);
  fermerCom(&listenSocketClient);
  terminerCom();			// Release WinSock

  return 0;
}

