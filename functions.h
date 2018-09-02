
#ifndef INC_FUNCTION_H
#define INC_FUNCTION_H
#include <sys/types.h>
#include <pwd.h>
#include <bits/stdc++.h>
#include <unistd.h>
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
#include <stack>
#include<termios.h>

using namespace std;


void setRoot(string &);
string realtiveToAbsolute(string);
string createFile(const char*,vector<string> &);
string createFolder(const char*,vector<string> &);
void copyDirector(const char *,const char *);
string copyFile(const char * , vector <string>&);
string filetype(struct stat );
string premission(struct stat );
void printFileDetails(const char* );
void moveDirector(const char *,const char *);
string moveFile(const char * , vector <string>&);
string removeFile(const char*,vector<string> &);
void removeSubFoldersFiles(const char *);
string removeFolder(const char*,vector<string> &);
string renameFile(const char *path ,vector<string> &);
void searchFile(const char * , const char *);
int printSnapShot(const char* ,string &);
void gotoloc(const char* s,vector<struct dirent *>& );
void prints(vector <struct dirent *>&, const char*,int ,int,int);
void formatChanging(vector<string> &);
void removeSpace(string &,vector<string> &);




#endif  /*INC_MODULE1_H*/
