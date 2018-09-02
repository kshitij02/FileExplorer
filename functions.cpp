#include "functions.h"
string root1;
void setRoot(string &root){
  root1=root;
}
//Relative to Absolute
string realtiveToAbsolute(string abs_path){
  string z;
  if(abs_path[0]=='~'||abs_path[0]=='.')
  z=root1+abs_path.substr(1);
  else
    z=abs_path;
  return z;
}

//Creeates File & Checks whether file already exists or not 
string createFile(const char *path , vector <string> &v){
	DIR *dir;
   string res;
   struct dirent *ent;
   int flag=0;
   const char *fileName =v[1].c_str();
   const char *des_path;
   string z=realtiveToAbsolute(v[2]);
   des_path=z.c_str();
   //return des_path;
   if ((dir = opendir (des_path)) != NULL) {
    ent = readdir (dir);
    chdir(des_path);
    while ((ent) != NULL) {	
  		if(strcmp(ent->d_name,fileName)==0){
  			flag=1;
  			break;
  		}
  		ent = readdir (dir);
	}
	if(flag==1){
	 res= "File already exists ";
	}
	else{
		string str = string(des_path)+"/" + string(fileName);
		fstream file; 
   		file.open(str.c_str(),ios::out);
		res= "File created";
	}
		chdir("..");
 }
 else{res="Invaild Path";}
  return res;
}

 //Creates a folder 
string createFolder(const char *path , vector<string> &v){
  DIR *dir;
  struct dirent *ent;
  int flag=0;
  const char *folderName =v[1].c_str();
  const char *des_path;
  string z;
  string res;
  z=realtiveToAbsolute(v[2]);
  des_path=z.c_str();
  if ((dir = opendir (des_path)) != NULL) {
    ent = readdir (dir);
    chdir(des_path);
    while ((ent) != NULL) {	
  		if(strcmp(ent->d_name,folderName)==0){
  			flag=1;
  			break;
  		}
  		ent = readdir (dir);
	}
	if(flag==1){
		res="Folder already exists";
	}
	else{
		string str = string(des_path)+"/" + string(folderName);
		mkdir(str.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	   res="Folder created";
	}
	chdir("..");
 }

else {
  res="Invaild Path";
}

 closedir(dir);
 return res;
}

//Copies a Directory to given Desitination point
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

//Copies given file and folder to destination folder if exits 
string copyFile(const char* sou_path, vector<string>& fileFrom ){
  DIR *dir_s,*dir_d;
  string res;
  int arrg=fileFrom.size();
  struct dirent *ent;
  string des = realtiveToAbsolute( fileFrom[arrg-1]);
  const char *des_path = des.c_str();
  //return des_path;
  int flag_from=0,flag_to=0,i;
  if ((dir_d = opendir (des_path)) == NULL) {
    return "Destination Directory doesn't exists";
  }
  if ((dir_s = opendir (sou_path))!= NULL) {
  for(i=1;i<arrg-1;i++){
    int flag_dir=0;
    const char *file=fileFrom[i].c_str();
    ent = readdir (dir_s);
    chdir(sou_path);
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
    return "Source File doesn't exists ";
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
  return "Copying Completed";
  
}
//Determines File Type
string filetype(struct stat stat_buf){
if (S_ISDIR(stat_buf.st_mode))
    return "d"; 
else 
    return "-";
}
//Determines file premissions

string premission(struct stat stat_buf){
  string res;
  if(stat_buf.st_mode & S_IRUSR) res+="r";
    else res+="-";
     if(stat_buf.st_mode & S_IWUSR) res+="w";
    else res+="-";
    if (stat_buf.st_mode & S_IXUSR) res+="x";
    else res+="-";
  if(stat_buf.st_mode & S_IRGRP) res+="r";
    else res+="-";
     if(stat_buf.st_mode & S_IWGRP) res+="w";
     else res+="-";
    if (stat_buf.st_mode & S_IXGRP) res+="x";
    else res+="-";
  if(stat_buf.st_mode & S_IROTH) res+="r";
    else res+="-";
     if(stat_buf.st_mode & S_IWOTH) res+="w";
     else res+="-";
    if (stat_buf.st_mode & S_IXOTH) res+="x";
    else
    res+="-";  
    return res;
}
//Prints Directory Detials ls
void printFileDetails(const char* s){

  DIR *dir;
  struct dirent *ent;

  //const char *s= "/home/ksh/Desktop/sem1/os";
  if ((dir = opendir (s)) != NULL) {
    ent = readdir (dir);
  while ((ent) != NULL) {
    int flag=0;
    struct stat stat_buf;
    stat(ent->d_name, &stat_buf);
    //printf("%-10s",ent->d_name);
    //printf("%-10s",premission(stat_buf));
    //printf("%-2s",filetype(stat_buf));  
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


//Goto Function 
void gotoloc(const char* s,vector<struct dirent *>& ent){

  DIR *dir;
  //vector<struct dirent *>ent;
  struct dirent *e;

  //const char *s= "/home/ksh/Desktop/sem1/os";
  if ((dir = opendir (s)) != NULL) {
    chdir(s);
    e=readdir (dir);
    int i=0;
  while (e != NULL) {
    ent.push_back(e);
    e=readdir(dir);
  }
  closedir (dir);
} 
else {
  perror ("");
}
//return ent;
}
//prints directory 
void prints(vector <struct dirent *>& ent,const char* s,int start ,int end,int window_size){
  DIR *dir;
 chdir(s);
if ((dir = opendir (s)) != NULL) {
 for (int i=start;i<ent.size() && i<=end ;i++){
  struct stat stat_buf;
  stat(ent[i]->d_name, &stat_buf);
  printf("%-15s",ent[i]->d_name);
  if(window_size>30){
  string pre=premission(stat_buf);
  cout<<" "<<pre;
  //printf("%10s",pre);
  string type = filetype(stat_buf);
  cout<<type<<" ";}
  if(window_size>45){
  printf("%6d",stat_buf.st_size);
  struct group *grp = getgrgid( stat_buf.st_gid);
  printf("%4s",grp->gr_name);
  struct passwd *pws= getpwuid(stat_buf.st_uid);
  printf("%4s ",pws->pw_name);}
  if(window_size>65){
  cout<<ctime(&stat_buf.st_mtime);
  }
  else{
    cout<<endl;
  }
  
 }
 chdir(".."); 
}
closedir(dir);
}


//Moving Directory/ Files from source to Destination

void moveDirector(const char *sou_path,const char *des_path){
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
          mkdir(str_file_to.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
          moveDirector(str_file_from.c_str(),str_file_to.c_str());
          rmdir(str_file_from.c_str());
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
        remove(str_file_from.c_str());
  
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

string moveFile(const char* sou_path, vector<string>& fileFrom ){
  DIR *dir_s,*dir_d;
  string res;
  int arrg = fileFrom.size();
  struct dirent *ent;
  string des= fileFrom[arrg-1];
  const char *des_path = des.c_str();
  int flag_from=0,flag_to=0,i;
  if ((dir_d = opendir (des_path)) == NULL) {
    res="Destination Directory doesn't exists";
  }
  if ((dir_s = opendir (sou_path))!= NULL) {
  for(i=1;i<arrg-1;i++){
    int flag_dir=0;
    const char *file=fileFrom[i].c_str();
    ent = readdir (dir_s);
    chdir(sou_path);
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
    res="Source File doesn't exists ";
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
   remove(str_file_from.c_str());
  
  }
  else{
    mkdir(str_file_to.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    moveDirector(str_file_from.c_str(),str_file_to.c_str());
    rmdir(str_file_from.c_str());
  }
res="Move Completed";
}
  chdir("..");
 }
}
else {
  perror ("");
}
 closedir(dir_s);
 closedir(dir_d);
  return res;
}
//Removes File 
string removeFile(const char *path , vector <string> &v){
  DIR *dir;
  string res;
   struct dirent *ent;
   int flag=0;
   const char *fileName =v[2].c_str();
   const char *des_path;
   string des=realtiveToAbsolute(v[1]);
   des_path=des.c_str();
  if(des_path[0]!='.' ||des_path[0]!='~'|| des_path[0]!='/' )
    return "Invaild Path";
  if ((dir = opendir (des_path)) != NULL) {
    ent = readdir (dir);
    chdir(des_path);
    while ((ent) != NULL) {	
  		if(strcmp(ent->d_name,fileName)==0){
  			flag=1;
  			break;
  		}
  		ent = readdir (dir);
	}
	if(flag==0){
    res="File doesn't exists ";
	}
	else{
		string str = string(des_path)+"/" + string(fileName);
		remove(str.c_str());
		res="File removed";
	}
	chdir("..");
 closedir(dir);
 
 }

else {
  res="Invaild Path";
}

 return res;
}

//Remove Sub Directory from given folder helper fuction of removeFolder  

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
      //cout<<"Removing "<<str;
      removeSubFoldersFiles(str.c_str());
      rmdir(str.c_str());
      } else{
      //cout<<"file removed"<<str;
      remove(str.c_str());
      }
    }
    ent =readdir(dir);

  }
 }
 chdir("..");


}
//Remove File & Folder frome given location 

string removeFolder(const char *path , vector <string> &v){
  DIR *dir;
   struct dirent *ent;
   int flag=0;
   const char *fileName =v[2].c_str();
   const char *des_path;
   string res;
   string des = realtiveToAbsolute(v[1]);
   des_path=des.c_str();
   if ((dir = opendir (des_path)) != NULL) {
    ent = readdir (dir);
    chdir(des_path);
    while ((ent) != NULL) {	
  		if(strcmp(ent->d_name,fileName)==0){
  			flag=1;
  			break;
  		}
  		ent = readdir (dir);
	}
	if(flag==0){
		res="Folder doesn't exists";
	}
	else{
   // cout<<"starting \n";
		string str = string(des_path)+"/" + string(fileName);
    const char *cstr =str.c_str();
    removeSubFoldersFiles(cstr);		
    rmdir(str.c_str());
		res="Folder removed";
	}
	chdir("..");
 }

else {
  res="Invaild Path";
}

 closedir(dir);
 return res;
}

// Checks Wheather a File/folder to rename exits or not and rename it 
string renameFile(const char *path ,vector<string> &v){
  DIR *dir;
  string res;
  struct dirent *ent;
  const char *fileOldName = v[1].c_str();
  const char *fileNewName = v[2].c_str();  
  int flag=0;
  if ((dir = opendir (path)) != NULL) {
    ent = readdir (dir);
    chdir(path);
    while ((ent) != NULL) {	
  		if(strcmp(ent->d_name,fileOldName)==0){
  			flag=1;
  			break;
  		}
  		ent = readdir (dir);
	}
	if(flag==0){
		res="File doesn't exists ";
	}
	else{
		//string str = string(path)+"/" + string(fileName);
		rename(fileOldName,fileNewName);
		res="File renamed";
	}
	chdir("..");
 }

else {
  perror ("");
}

 closedir(dir);
 return res;
}

//Search Recurively for a file / folder in Given Directory
void searchFile(const char *path , const char *fileName){
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

//Snapshots Directory (ls -R) 
int printSnapShot(const char* s,string &strSnap){

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
    string str =string(ent->d_name);
    strSnap=strSnap+str+"\n";
    const char *cstr = (str.c_str());
    printSnapShot(cstr,strSnap);
    chdir(".."); }
    else{
      strSnap=strSnap+string(ent->d_name)+"\n";
    }
   } 
    ent = readdir (dir);
  }

  closedir (dir);
  return 1;
} 
else {
  return 0;
}
}
void formatChanging(vector<string> &v){
  int i;
  string str=v[1];
  //cout<<str;
  for(i=str.length()-1;str[i]!='/';i--);
  if(i>=0){
  string str1=str.substr(0,i);
  //cout<<str1;
  v.pop_back();
  v.push_back(str1);
  str1=str.substr(i+1);
  v.push_back(str1);}

}
void removeSpace(string &s , vector<string> &v){
  string word="";
  for(int i=0;i<s.length();i++){
    if(s[i]==' '){
      v.push_back(word);
  //    cout<<word<<endl;
      word="";
    }
    else{
      word+=s[i];
    }
  }
  v.push_back(word);
}

