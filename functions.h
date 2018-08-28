#include <bits/stdc++.h>
#include <unistd.h>

#ifndef INC_FUNCTION_H
#define INC_FUNCTION_H
#include <string.h>
#include <map>
#include <vector>
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
#include <bits/stdc++.h>
#include <string.h>
#include <stdlib.h>
#include<termios.h>

using namespace std;



void createFile(char *, char * );
void createFolder(char * , char *);
void copyDirector(const char *,const char *);
void copyFile(int , char **);
string filetype(struct stat );
string premission(struct stat );
void printFileDetails(const char* );
void moveDirector(const char *,const char *);
void moveFile(int  , char **);
void removeFile(char *, char *);
void removeSubFoldersFiles(const char *);
void removeFolder(char * , char *);
void renameFile(char * , char * ,char *);
void searchFile(const char * , char *);
void printSnapShot(const char* );
void gotoloc(const char* s,vector<struct dirent *>& );
void prints(vector <struct dirent *>&, const char*,int ,int);




#endif  /*INC_MODULE1_H*/
