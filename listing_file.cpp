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


string filetype(struct stat stat_buf){
if(S_ISBLK(stat_buf.st_mode))
    return "block special file"; 
else if(S_ISCHR(stat_buf.st_mode))
    return "character special file"; 
else if (S_ISDIR(stat_buf.st_mode))
    return "directory"; 
else if (S_ISFIFO(stat_buf.st_mode))
    return "a pipe or FIFO special file"; 
else if (S_ISREG(stat_buf.st_mode))
    return "regular file"; 
else if(S_ISLNK(stat_buf.st_mode))
    return "symbolic link"; 
return "file type not found"; 
}
string premission(struct stat stat_buf){
  string res;
  if(stat_buf.st_mode & S_IRUSR) res+="r";
    else res+="-";
     if(stat_buf.st_mode & S_IWUSR) res+="w";
    else res+="-";
    if (stat_buf.st_mode & S_IXUSR) res+="x";
    else res+="-";
    res+=" ";
  if(stat_buf.st_mode & S_IRGRP) res+="r";
    else res+="-";
     if(stat_buf.st_mode & S_IWGRP) res+="w";
     else res+="-";
    if (stat_buf.st_mode & S_IXGRP) res+="x";
    else res+="-";
    res+=" ";
  if(stat_buf.st_mode & S_IROTH) res+="r";
    else res+="-";
     if(stat_buf.st_mode & S_IWOTH) res+="w";
     else res+="-";
    if (stat_buf.st_mode & S_IXOTH) res+="x";
    else
    res+="-";
    res+=" ";       
    return res;
}

void printFileDetails(const char* s){

  DIR *dir;
  struct dirent *ent;

  //const char *s= "/home/ksh/Desktop/sem1/os";
  if ((dir = opendir (s)) != NULL) {
    ent = readdir (dir);
    chdir(s);
  while ((ent) != NULL) {
    int flag=0;
    struct stat stat_buf;
    stat(ent->d_name, &stat_buf);
    cout<<ent->d_name<<":";
    cout<<premission(stat_buf);
    cout<<filetype(stat_buf);  
    if(ent->d_name[0] == '.' )
      flag=1;
    if(flag==0)   
    cout<<stat_buf.st_size<<":";
    struct group *grp = getgrgid( stat_buf.st_gid);
    cout<<grp->gr_name<<":";
    cout<<ctime(&stat_buf.st_mtime);
    ent = readdir (dir);
  }

  closedir (dir);
} 
else {
  perror ("");
}
}

int main(int arrg ,char **agv){
  printFileDetails(agv[1]);
  return 0;

}