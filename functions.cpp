#include "functions.h"


//Creeates File & Checks whether file already exists or not 
void createFile(char *path , char *fileName ){
	DIR *dir;
   struct dirent *ent;
   int flag;
   flag=0;
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

 //Creates a folder 
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
//Determines File Type
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
//Determines file premissions

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
void prints(vector <struct dirent *>& ent,const char* s,int start ,int end){
  DIR *dir;
 chdir(s);
if ((dir = opendir (s)) != NULL) {
 for (int i=start;i<ent.size() && i<=end ;i++){
  cout<<ent[i]->d_name<<endl;
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

void moveFile(int arrg , char **fileFrom ){
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
   remove(str_file_from.c_str());
  //cout<<"File copied\n";
  }
  else{
    mkdir(str_file_to.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    moveDirector(str_file_from.c_str(),str_file_to.c_str());
    rmdir(str_file_from.c_str());
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
//Removes File 
void removeFile(char *path , char *fileName){
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
		cout<<"File doesn't exists \n";
	}
	else{
		string str = string(path)+"/" + string(fileName);
		remove(str.c_str());
		cout<<"File removed\n";
	}
	chdir("..");
 }

else {
  perror ("");
}

 closedir(dir);
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
//Remove File & Folder frome given location 

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

// Checks Wheather a File/folder to rename exits or not and rename it 
void renameFile(char *path , char *fileOldName ,char *fileNewName){
  DIR *dir;
  struct dirent *ent;
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
		cout<<"File doesn't exists \n";
	}
	else{
		//string str = string(path)+"/" + string(fileName);
		rename(fileOldName,fileNewName);
		cout<<"File renamed\n";
	}
	chdir("..");
 }

else {
  perror ("");
}

 closedir(dir);
}

//Search Recurively for a file / folder in Given Directory
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

//Snapshots Directory (ls -R) 
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
