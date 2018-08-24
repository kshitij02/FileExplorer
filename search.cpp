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


void searchFile(const char *path , char *fileName){
  DIR *dir;
  struct dirent *ent;
  int flag=0;
  if ((dir = opendir (path)) != NULL) {
    ent = readdir (dir);
    chdir(path);
    while ((ent) != NULL) {	
  		struct stat stat_buf;
      stat(ent->d_name,&stat_buf);
      if(strcmp(ent->d_name,fileName)==0){
        cout<<ent->d_name<<":"<<path<<"/"<<fileName<<"\n";
  		}
      else if(S_ISDIR(stat_buf.st_mode) && ent->d_name[0]!='.'){
        string str=string(path)+"/"+string(ent->d_name);
        searchFile(str.c_str(),fileName);
      }
  		ent = readdir (dir);
	}
	chdir("..");
 }

else {
  perror ("");
}

 closedir(dir);
}


int main(int arrg ,char **agv){
   searchFile(agv[1],agv[2]);
  return 0;
}
