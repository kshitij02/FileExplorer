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
void copyDirector(const char *sou_path,const char *des_path){
  DIR *dir_s,*dir_d;
  struct dirent *ent;
  if ((dir_d = opendir (des_path)) != NULL && (dir_s = opendir (sou_path)) != NULL) {
    ent = readdir (dir_s);
    chdir(sou_path);
    while ((ent) != NULL) { 
        struct stat stat_buf;
        stat(ent->d_name,&stat_buf);
        string str_file_from = string(sou_path)+"/" + string(ent->d_name);
        string str_file_to = string(des_path)+"/" + string(ent->d_name);
        if (S_ISDIR(stat_buf.st_mode) && ent->d_name[0]!='.'){
          mkdir(str_file_to.c_str(),(S_IRWXU) | (S_IRWXG) | (S_IROTH) | (S_IXOTH));
          copyDirector(str_file_from.c_str(),str_file_to.c_str());
        }
        else if(ent->d_name[0]!='.'){
        fstream file; 
        file.open(str_file_to.c_str(),ios::out|ios_base::binary);
        ifstream in(str_file_from.c_str(),ios_base::in | ios_base::binary); 
        std::ofstream out(str_file_to.c_str(),ios_base::out | ios_base::binary);
        char buf[4096];
        do {
          in.read(&buf[0], 4096);      
          out.write(&buf[0], in.gcount());
        } while (in.gcount() > 0);         
        in.close();
        out.close();
        }
      ent = readdir (dir_s);
  }
 chdir("..");
 }
else {
  perror ("");
}
 closedir(dir_s);
 closedir(dir_d);
  
} 

void copyFile(int arrg , char **fileFrom ){
  DIR *dir_s,*dir_d;
  struct dirent *ent;
  char *des_path = fileFrom[arrg-1];
  char *sou_path = fileFrom[1];
  int flag_from=0,flag_to=0,i;
  if ((dir_d = opendir (fileFrom[arrg-1])) == NULL) {
    cout<<"Destination Directory doesn't exists\n";
    return ;
  }
  if ((dir_s = opendir (fileFrom[1]))!= NULL) {
  for(i=2;i<arrg-1;i++){
    int flag_dir=0;
    char *file=fileFrom[i];
    ent = readdir (dir_s);
    chdir(fileFrom[1]);
    while ((ent) != NULL) { 

      if(strcmp(ent->d_name,file)==0){
        flag_from=1;
        struct stat stat_buf;
        stat(ent->d_name,&stat_buf);
        if (S_ISDIR(stat_buf.st_mode)){
          flag_dir=1;
     }
      }
      ent = readdir (dir_s);
  }
  if(!flag_from){
    cout<<"Source File doesn't exists \n";
  }
  if((flag_from)){
    string str_file_from = string(sou_path)+"/" + string(file);
    string str_file_to = string(des_path)+"/" + string(file);
    if(!flag_dir){
    //cout<<str_file_from<<"copied to "<<str_file_to;
    fstream file; 
    file.open(str_file_to.c_str(),ios::out|ios_base::binary);
    ifstream in(str_file_from.c_str(),ios_base::in | ios_base::binary); 
    std::ofstream out(str_file_to.c_str(),ios_base::out | ios_base::binary);

   char buf[4096];
   do {
      in.read(&buf[0], 4096);      
      out.write(&buf[0], in.gcount());
   } while (in.gcount() > 0);         
   in.close();
   out.close();
  //cout<<"File copied\n";
  }
  else{
    mkdir(str_file_to.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    copyDirector(str_file_from.c_str(),str_file_to.c_str());
  }
}
  chdir("..");
 }
}
else {
  perror ("");
}
 closedir(dir_s);
 closedir(dir_d);
  
}



int main(int arrg ,char **agv){
  copyFile(arrg,agv);
  return 0;
}
