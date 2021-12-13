/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#include <list>
#include <iostream>
#include <iterator>
using namespace std;

#define MAX_LINE_SIZE 80
#define MAXARGS 20

char* L_Fg_Cmd;
list <Job> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/

    struct sigaction ctrl_c;
    struct sigaction ctrl_z;
    ctrl_c.sa_handler = &CTRL_C;
    ctrl_z.sa_handler = &CTRL_Z;
    sigaction(SIGINT, &ctrl_c, NULL); //if CTRL-C (SIGINT) is catched do ctrl_c func
    sigaction(SIGTSTP, &ctrl_z, NULL);//if CTRL-Z (SIGTSTP) is catched do ctrl_z func
	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	

	
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin); //gets command line from user and stores in linesize
		strcpy(cmdString, lineSize);    	// moves lineSize to cmdString
		cmdString[strlen(lineSize)-1]='\0'; //
					// perform a complicated Command
		//if(!ExeComp(lineSize)) continue;
					// background command	
	 	if(!BgCmd(lineSize)) continue;
					// built in commands
		ExeCmd(lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

