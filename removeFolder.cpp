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

void removeSubFoldersFiles(const char *path){
 DIR *dir;
  struct dirent *ent;
  int flag=0;
  if ((dir = opendir (path)) != NULL) {
    ent = readdir (dir);
    chdir(path);
    while ((ent) != NULL) {
    struct stat stat_buf;
    stat(ent->d_name, &stat_buf);
    if(ent->d_name[0]!='.'){
    string str=string(path)+"/"+string(ent->d_name); 
      if (S_ISDIR(stat_buf.st_mode)){
        cout<<"Removing "<<str;
      removeSubFoldersFiles(str.c_str());
      rmdir(str.c_str());
      } else{
        cout<<"file removed"<<str;
      remove(str.c_str());
      }
    }
    ent =readdir(dir);

  }
 }
 chdir("..");


}

void removeFolder(char *path , char *fileName){
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
	if(flag==0){
		cout<<"Folder doesn't exists \n";
	}
	else{
    cout<<"starting \n";
		string str = string(path)+"/" + string(fileName);
    const char *cstr =str.c_str();
    removeSubFoldersFiles(cstr);		
    rmdir(str.c_str());
		cout<<"Folder removed\n";
	}
	chdir("..");
 }

else {
  perror ("");
}

 closedir(dir);
}


int main(int arrg ,char **agv){
   removeFolder(agv[1],agv[2]);
  return 0;
}
