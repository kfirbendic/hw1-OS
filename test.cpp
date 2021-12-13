#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>
using namespace std;
int main(){
	cout << "start ";
	sleep(20);
	cout << "finish";
	return 0 ;
}
