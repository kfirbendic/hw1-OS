#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string>
using namespace std;

//string sig_num_to_sig_name(int sig_num);
void CTRL_C(int sig_num);
void CTRL_Z(int sig_num);

#endif

