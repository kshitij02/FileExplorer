#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
using namespace std;

void createFile(char *path , char *fileName ){

DIR *dir;
  struct dirent *ent;
  int flag=0;
  if ((dir = opendir (path)) != NULL) {
    ent = readdir (dir);
    chdir(path);
    while ((ent) != NULL) {	
  		if(strcmp(ent->d_name,fileName)==0){
  			flag=1;
  			break;
  		}
  		ent = readdir (dir);
	}
	if(flag==1){
		cout<<"File already exists \n";
	}
	else{
		string str = string(path)+"/" + string(fileName);
		fstream file; 
   		file.open(str.c_str(),ios::out);
		cout<<"File created\n";
	}
	chdir("..");
 }


}



int main(int arrg ,char **agv){
   createFile(agv[1],agv[2]);
  return 0;
}
