#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <list>
#include <iostream>
#include <iterator>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
using namespace std;
//typedef enum { FALSE , TRUE } bool;
void delete_finished_jobs_from_list();


class Job {
public:
	string name;
	pid_t pid;
	time_t start_time;
	bool stop_f;



	//c'tor
	Job(list <Job> Job_list, string name, pid_t pid){
		this->name = name;
		this->pid = pid;
		time(&(this->start_time));
		this->stop_f = false;

	}

	//copy c'tor
	Job(const Job &job1){
			this->name = job1.name;
			this->pid = job1.pid;
			this->start_time = job1.start_time;
			this->stop_f = job1.stop_f;
		}

	//assignment
	Job& operator=(const Job &job1){
		// Guard self assignment
		    if (this == &job1){
		    	return *this;
		    }
		    else{
		    this->name = job1.name;
		    this->pid = job1.pid;
		    this->start_time = job1.start_time;
		    this->stop_f = job1.stop_f;
		    }
		    return *this;
	}

};




//int ExeComp(char* lineSize);
int BgCmd(char* lineSize);
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
#endif



