//		commands.c
//********************************************
#include "commands.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include <signal.h>
#include "signals.h"
using namespace std;

extern list <Job> jobs;
char* prev_path = new char[MAX_LINE_SIZE]; // this is the previous path - used in "cd"
list <string> history_list;
extern Job fg_job;


void delete_finished_jobs_from_list(){
	//cout << "in delete jobs\n";
	std::list<Job>::iterator it;
	for(it = jobs.begin(); it != jobs.end(); it++ ){
		if(waitpid(it->pid, NULL, WNOHANG) != 0){

			jobs.erase(it);
			//cout << "job erased " << it->name; // FOR DEBBUGING
			it = jobs.begin();
		}
	}
}

/*
list<Job>::iterator get_job(int job_num){
		int index = 1;
		std::list<Job>::iterator it;
		for(it = jobs.begin(); it != jobs.end(); it++ ){
			if(index == job_num){
				return it;
			}
			index++;
		}
	return NULL; // error - no such job num in list
}
*/

void add_cmd_to_history(string cmdString){
	if(history_list.size() >= 50){
		history_list.pop_back();
	}
	history_list.push_front(cmdString);
	
}




//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	//char pwd[MAX_LINE_SIZE];
	const char* delimiters = " \t\n"; 
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "pwd") ) 
	{
		add_cmd_to_history(cmdString);
		if (num_arg == 0){
			char* curr_dir = get_current_dir_name(); // get_current_dir_name return char*
			if(curr_dir != NULL){ // if malloc succeed
				printf("%s \n",curr_dir);

				return 0;

			}
			else{ //malloc failed
				perror("Error: malloc failed");
			}
		}
		else{
			illegal_cmd = true;
		}

	}	
	/*************************************************/
	else if (!strcmp(cmd, "cd")) 
		{
		add_cmd_to_history(cmdString);
		if(num_arg != 0){
			char* current_path = get_current_dir_name();
			if(prev_path == NULL){
				strcpy(prev_path,current_path);
			}
			// error in path
			if(strcmp(args[1],"-")!=0 && chdir(args[1]) == -1 ){ //this also changes the path if success
				printf("smash error:>\"%s\"-No such file or directory\n", args[1]);
				return 0;
			}
			// go back to previous path
			else if (strcmp(args[1],"-") == 0){
				chdir(prev_path);
				printf("%s\n", prev_path );
				strcpy(prev_path,current_path);

			}
			//else
			//normal cd  (already bean preformed if no error)
			return 0;
		}
		else{
			illegal_cmd = true;
		}
	} 

	
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{

		if(num_arg == 0){
			if (history_list.size() != 0){
				list <string> :: iterator it;
				for (it = history_list.begin(); it != history_list.end(); ++it){
					cout << '\t' << *it << "\n";
				}
				//cout << '\n';
			}
			return 0;
		}
		else{
			illegal_cmd = true;
		}
		add_cmd_to_history(cmdString);
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{
		add_cmd_to_history(cmdString);
		if(num_arg != 0){
			illegal_cmd = true;
		}
		else{
			//delete finished jobs from list
			delete_finished_jobs_from_list();
			//iterate and print all jobs
			int index = 1;
			time_t curr_time;
			time(&curr_time);
			std::list<Job>::iterator it;
			for(it = jobs.begin(); it != jobs.end(); it++ ){
				time_t run_time = curr_time - it->start_time;
				if (it->stop_f == false){ //running in bg
					cout << "[" << index << "]" <<" "<< it->name << " : " << it->pid << " " << run_time << " secs\n" ;
				}
				else{ //stopped
					cout << "[" << index << "]" <<" "<< it->name << " : " << it->pid << " " << run_time << " secs (Stopped)\n";
				}
				index++;
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill")) 
	{
		add_cmd_to_history(cmdString);
 		if(num_arg != 2){
 			illegal_cmd = true;
 		}
 		else{
 			int sig_num = -(atoi(args[1]));
 			int job_num = atoi(args[2]);
 			//string sig_name = sig_num_to_sig_name(sig_num); // NOT NECCESARY
 			//cout << sig_name;
 			int index = 0;
 			Job selected_job(jobs,"",0 ); // initialize somthing..
 			std::list<Job>::iterator it;
 			for(it = jobs.begin(); it != jobs.end(); it++ ){
 			index++;
 			if(index == job_num){
 				selected_job = *it;
 				}
			}

 			if(selected_job.pid == 0){
 					printf("‫‪smash‬‬ ‫‪error:‬‬ >‬ ‫‪kill‬‬ %d ‫–‬ ‫‪job‬‬ ‫‪does‬‬ ‫‪not‬‬ ‫‪exist\n‬‬",job_num) ;
 				return -1;
 			}
 			else if(kill(selected_job.pid,sig_num) == -1){
 				printf("‫‪smash‬‬ ‫‪error:‬‬ >‬ ‫‪kill‬‬ %d ‫–‬ cannot send signal\n",job_num);
 				//perror("");
 				return -1;
 			}

 		}


	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		if(num_arg ==0){
			cout << "smash pid is " << getpid() <<endl;
			return 0;
		}
		else{
			illegal_cmd = true;
		}

	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		add_cmd_to_history(cmdString);
		if(num_arg>1){
			illegal_cmd = true;
		}
		else{
			if(num_arg == 0){ //take last job
				delete_finished_jobs_from_list();
				Job last_job = jobs.back(); //get last job name
				if(last_job.stop_f == true){ //the proccess was in stop
					(jobs.back()).stop_f = false;
					kill((jobs.back()).pid, SIGCONT);
				}
				if(jobs.empty()){
					printf("smash error: > \"%s\"\n", cmdString);
					return 1;
				}
				cout << last_job.name << "\n";
				fg_job = jobs.back();
				waitpid(last_job.pid, NULL, WUNTRACED); //wait for it to end aka fg
			}
			else if (num_arg == 1){ //take according to job num
				int job_num = atoi(args[1]);
				int index = 0;
				Job selected_job(jobs,"",0 ); // initialize with something..
				std::list<Job>::iterator it;
				for(it = jobs.begin(); it != jobs.end(); it++ ){ //find the right job
					index++;
					if(index == job_num){
						selected_job = *it;
						if(it->stop_f == true){ //the proccess was in stop
							it->stop_f = false;
							kill(it->pid, SIGCONT);
						}
					}

				}
				if(selected_job.pid == 0 || jobs.empty()){
					printf("smash error: > \"%s\"\n", cmdString);
					return 1;
				}

				cout << selected_job.name << "\n";
				//cout << "watinig for proccess to stop or exit\n";
				fg_job = selected_job;
				waitpid(selected_job.pid, NULL, WUNTRACED); //wait for it to end or stop aka fg
				//cout << "proccess stopped or exited\n";
				delete_finished_jobs_from_list();
			}
		}

	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		add_cmd_to_history(cmdString);
		if(num_arg>1){
			illegal_cmd = true;
		}
		else{
			if(num_arg == 0){ //take last job
				delete_finished_jobs_from_list();
				Job last_job = jobs.back(); //get last job
				if(last_job.stop_f == false && !jobs.empty()){ // the last job is ruuning in background - error
					printf("smash error: > \"bg\" - The job is already running in background\n");
					return 1;
				}
				else if(jobs.empty()){
					printf("smash error: > \"bg\" - no jobs in list\n") ;
					return 1;
				}
				else{ //resume the last job (stopped) to run in background
					cout << last_job.name << "\n";
					(jobs.back()).stop_f = false;

					kill(last_job.pid, SIGCONT);
				}
			}
			else if (num_arg == 1){ //take according to job num
				int job_num = atoi(args[1]);
				int index = 0;
				Job selected_job(jobs,"",0 ); // initialize somthing..
				std::list<Job>::iterator it;
				for(it = jobs.begin(); it != jobs.end(); it++ ){
					index++;
					if(index == job_num){
						//cout << job_num << " " << index;
						selected_job = *it;
						break;
					}
				}
				if(selected_job.stop_f == false && jobs.empty()==false){ // the job is ruuning in background - error
					printf("smash error: > \"bg\" - The job is already running in background\n");
					return 1;
				}
				else{ //resume the job (stopped) to run in background
					//cout << "continue" << "\n";
					it->stop_f = false;
					//cout << selected_job.pid;
					kill(selected_job.pid, SIGCONT);
				}
				if(selected_job.pid == 0){
					if(jobs.empty()){
						printf("smash error: >  \"bg\" - no jobs in list\n") ;
						return 1;
					}
					else{
						printf("smash error: > \"bg\" - job does not exist, did you mean job [%d] (last job in list)\n",index) ;
						return 1;
					}
					 // ******UNCOMMEN THIS LINE AFTER DONE DEBUGGING ******
				}
				cout << selected_job.name << "\n";

				/*
				// FOR DEBBUGING ONLY (ctrl c, ctrl z)
						if(job_num == 101){ //stop the last proccess
							kill((jobs.back()).pid,SIGSTOP);
							(jobs.back()).stop_f = true;
						}
						if(job_num == 102){ //stop the last proccess
							kill((jobs.back()).pid,9);
							delete_finished_jobs_from_list();
							//(jobs.back()).stop_f = true;
						}
				//----------------------
				*/
				return 0;
			}

		}




	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if(num_arg>1){
					illegal_cmd = true;
		}
		else if(num_arg == 0){
			exit(0);
		}
		else if (strcmp(args[1], "kill") == 0){ // quit kill
			std::list<Job>::iterator it;
			int index = 1;
			for(it = jobs.begin(); it != jobs.end(); it++ ){
				time_t send_time;
				kill(it->pid,SIGTERM);
				time(&send_time);
				cout << "["<< index++ << "] " << it->name << " - Sending SIGTERM... " << flush;
				while(time(NULL) < send_time+5); // wait 5 secs
				if(waitpid(it->pid, NULL, WNOHANG) != 0){ // if signal didnt terminate
					kill(it->pid,SIGKILL);
					cout << "(5 sec passed) Sending SIGKILL... ";
				}
				cout << "Done\n" << flush;

			}
		}
		exit(0);

	} 	
	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
		add_cmd_to_history(cmdString);
		if(num_arg == 2){
			FILE *file1;
			FILE *file2;
			file1 = fopen(args[1], "r");
			file2 = fopen(args[2], "r");
			if(file1 == NULL || file2 == NULL){
				perror("smash error");
				return 1;
			}
			while(1){
				char word1;
				char word2;
				word1 = fgetc(file1);
				word2 = fgetc(file2);
				if(feof(file1) == ~feof(file2)){
					cout << "1" << endl;
					break;
				}
				else if (~feof(file1) && ~feof(file2)){
					if(word1 != word2){
						cout << "1" << endl;
						break;
					}
				}
				else {
					cout << "0" << endl;
				}
			} 		
		}
		else{
			illegal_cmd = true;
		}

	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	//cout << "external\n";
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					perror("ERROR: external command error ");
					exit(1);
					return;

        	case 0 :
                	// Child Process
               		setpgrp();

			        // Add your code here (execute an external command)
					execvp(args[0],args);
					perror("ERROR"); //get here only if exec returns
					exit(1); //exit
					return;
					/* 
					your code
					*/

			default:
				add_cmd_to_history(cmdString);

				Job new_job(jobs, args[0],pID);
				new_job.stop_f = false;

                jobs.push_back(new_job);
                delete_finished_jobs_from_list();
               // cout << "created new job - " << new_job.name << "\n";
                fg_job = new_job;
                waitpid(pID, NULL, WUNTRACED);
                break;
				// Add your code here
					


	}
}

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, char* cmdString )
{


	//cout << "bgcmd\n";
	//char* Command;
	const char* delimiters = " \t\n";
	char *args[MAX_ARG];

	//int i = 0, num_arg = 0;
	if (lineSize[strlen(lineSize)-2] == '&')
	{

		lineSize[strlen(lineSize)-2] = '\0';

		// Add your code here (execute a in the background)

		char* Command = strtok(lineSize,delimiters);

		if(Command == NULL){
			return 0;
		} //not a BG command
		args[0] = Command;
		for(int i=1; i<MAX_ARG; i++){ //parsing argumants

			args[i] = strtok(NULL, delimiters);
		//	if(args[i] == NULL)
			//	cout << "no args";
		}


		int pID;

		switch(pID = fork())
		{
		    		case -1:
							// Add your code here (error)
							perror("ERROR: external command error ");
							exit(1);
							break;

		        	case 0 :
		                	// Child Process
		               		setpgrp();

					        // Add your code here (execute an external command)
							execvp(args[0],args);
							perror("ERROR"); //get here only if exec returns
							exit(1); //exit
							break;

					default:
						add_cmd_to_history(cmdString);
						string bg(args[0]);
						 Job new_bg_job(jobs,bg,pID);

						 new_bg_job.stop_f = false;

						 jobs.push_back(new_bg_job);
						 //cout << "new bg job " << jobs.back().name << "\n";
						 delete_finished_jobs_from_list();
						// waitpid(pID, NULL, WUNTRACED);

						 return 0 ;
						// Add your code here
		                break;
			}
/*
	switch (pID = fork())
		{
		// in case fork failed and no child was created
		case -1:
			perror("smash error");
			exit(1);
			break;
		// this case is the child
		case 0:
			setpgrp();
			execvp(args[0], args);
			perror("smash error");
			exit(0);
			break;
		// parent
		default:
			string bg_cmd(args[0]);
			Job bg_job(jobs, args[0],pID);

			jobs.push_back(bg_job);
			delete_finished_jobs_from_list();

			return 0;
			break;
		}
	}
*/

	}
	return -1;

}

//**************************************************************************************


