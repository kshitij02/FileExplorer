#include <bits/stdc++.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
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

 
void printSnapShot(const char* s){

  DIR *dir;
  struct dirent *ent;


  if ((dir = opendir (s)) != NULL) {
    ent = readdir (dir);
 
  chdir(s);
  while ((ent) != NULL) {
    int flag=0;
    struct stat stat_buf;
    stat(ent->d_name, &stat_buf);
    if(ent->d_name[0] != '.' ){
    if (S_ISDIR(stat_buf.st_mode)){
      cout<<ent->d_name<<":\n";
    string str =string(ent->d_name);
     str=string(s)+'/'+str;
    const char *cstr = (str.c_str());
    printSnapShot(cstr);
    chdir(".."); }
    else{
      cout<<ent->d_name<<"\n";
    }
   } 
    ent = readdir (dir);
  }

  closedir (dir);
} 
else {
  perror ("");
}
}

int main(int arrg ,char **agv){
  printSnapShot(agv[1]);
	return 0;

}
