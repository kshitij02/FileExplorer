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
using namespace std;


void createFolder(char *path , char *folderName){
  DIR *dir;
  struct dirent *ent;
  int flag=0;
  if ((dir = opendir (path)) != NULL) {
    ent = readdir (dir);
    chdir(path);
    while ((ent) != NULL) {	
  		if(strcmp(ent->d_name,folderName)==0){
  			flag=1;
  			break;
  		}
  		ent = readdir (dir);
	}
	if(flag==1){
		cout<<"Folder already exists \n";
	}
	else{
		string str = string(path)+"/" + string(folderName);
		mkdir(str.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		cout<<"Folder created\n";
	}
	chdir("..");
 }

else {
  perror ("");
}

 closedir(dir);
}


int main(int arrg ,char **agv){
   createFolder(agv[1],agv[2]);
  return 0;
}
