// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include <string.h>
#include <string>
#include "commands.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include <signal.h>
#include "signals.h"

using namespace std;

extern list <Job> jobs;
extern Job fg_job;

void CTRL_C(int sig_num){
	cout << "\n";
	if(jobs.empty() == 1) { return; }

	if(  fg_job.pid == 0 || kill(fg_job.pid,SIGINT) != 0){ // signal problem or no job in fg
		//cout << "this is exiting";
		return;
	}
	delete_finished_jobs_from_list();
}

void CTRL_Z(int sig_num){
	cout << "\n";

	if(jobs.empty() == 1) {
		//cout << "\n";
		return;
	}


	if( fg_job.pid == 0 || kill(fg_job.pid,SIGTSTP) != 0){
		return;
	}
	std::list<Job>::iterator it;
	for(it = jobs.begin(); it != jobs.end(); it++ ){ //find the right job
		if(fg_job.pid == it->pid){
			it->stop_f = true; // update job is stopped in list
			fg_job.pid = 0; // update fg_job to no job
			break;
		}
	}
}
/*
string sig_num_to_sig_name(int sig_num) {
	switch (sig_num) {
	case 1:
		return "SIGHUP";
		break;
	case 2:
		return "SIGINT";
		break;
	case 3:
		return "SIGQUIT";
		break;
	case 4:
		return "SIGILL";
		break;
	case 5:
		return "SIGTRACE";
		break;
	case 6:
		return "SIGABORT";
		break;
	case 7:
		return "SIGBUS";
		break;
	case 8:
		return "SIGFLOAT";
		break;
	case 9:
		return "SIGKILL";
		break;
	case 10:
		return "SIGUSER1";
		break;
	case 11:
		return "SIGSEGFAULT";
		break;
	case 12:
		return "SIGUSER2";
		break;
	case 13:
		return "SIGBROKENPIPE";
		break;
	case 14:
		return "SIGALARMCLK";
		break;
	case 15:
		return "SIGTERM";
		break;
	case 16:
		return "SIGSTACK";
		break;
	case 17:
		return "SIGCHILD";
		break;
	case 18:
		return "SIGCONT";
		break;
	case 19:
		return "SIGSTOP";
		break;
	case 20:
		return "SIGSTP";
		break;
	case 21:
		return "SIGSTOPTTYIN";
		break;
	case 22:
		return "SIGSTOPTTYOUT";
		break;
	case 23:
		return "SIGURGENTIO";
		break;
	case 24:
		return "SIGCPUTIME";
		break;
	case 25:
		return "SIGFILESIZE";
		break;
	case 26:
		return "SIGVIRTIMER";
		break;
	case 27:
		return "SIGPROFTIMER";
		break;
	case 28:
		return "SIGWINDOW";
		break;
	case 29:
		return "SIGIOPOSIBLE";
		break;
	case 30:
		return "SIGPOWER";
		break;
	case 31:
		return "SIGBADSYSCALL";
		break;
	default:
		return "-1";
		}
	return "-1";
}
*/
