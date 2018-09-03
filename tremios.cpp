#include <stdlib.h>
#include<termios.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "functions.h"

stack <string> back_dir;
stack <string> forward_dir;
char cwd[PATH_MAX];
string root;
int console_size;
int console_width;
struct termios oldt,newt;
using namespace std;
void mouse(const char*);


void settingNonConicalMode(){
	tcgetattr(STDIN_FILENO,&oldt);
	
	newt=oldt;
	newt.c_lflag = newt.c_lflag & ~(ECHO|ICANON);
	newt.c_lflag=1;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  	
}

void settingConicalMode(){
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
void check(const char *cwd,int m,vector <struct dirent *>&ent){
	DIR *dir;
    pid_t process;
	
    struct stat stat_buf;
    chdir(cwd);
    stat(ent[m]->d_name, &stat_buf);
    string str=(string(cwd)+"/"+string(ent[m]->d_name));
    cout<<"\x1B[2J\033[;H";
    if(ent[m]->d_type==DT_DIR){
    char * d =realpath(str.c_str(),NULL);
	str=string(d);
    if (str.find(root) != string::npos) {
    	back_dir.push(cwd);
    	mouse(str.c_str());
   		}
   		else{
   			mouse(cwd);
   		}
    }
    else{
    	 process=fork();
    	 if(process==0){
    	 	execl("/usr/bin/xdg-open","xdg-open",ent[m]->d_name,NULL);
    	 	exit(1);
    	 }
    	 else{
    	 	chdir("..");
    	 	mouse(cwd);
    	 }	
    }
    
} 
void snapFile(const char* path ,string &strSnap,string des_file ){
	chdir(path);
	ofstream snapFile;
	string fileName = string(path)+"/"+des_file;
  	snapFile.open (fileName.c_str());
  	snapFile << strSnap;
  	snapFile.close();
  	chdir("..");
}
int breakCheck(char ch){
	if(ch==27)
		return 0;
	return 1;
}
void commandMode(const char*  path){
	map <string,int> m;
	m.insert(make_pair("copy" , 0));
    m.insert(make_pair("move" , 1));
    m.insert(make_pair("rename" , 2));
    m.insert(make_pair("create_file", 3));
    m.insert(make_pair("create_dir" , 4 ));
    m.insert(make_pair("delete_file" , 5)); 
    m.insert(make_pair("delete_dir" , 6 ));
    m.insert(make_pair("goto" , 7 ));
    m.insert(make_pair("search" , 8 ));
    m.insert(make_pair("snapshot" , 9 ));
	struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
	console_size = size.ws_row;
	cout << "\x1B["<<console_size-1<<";1H:";

	while(1){
	vector<string> v;
	int searchFlag=1;
	string res="";
	string str="";
	char s;
	cin>>s;
	if(s==27){
		break;
	}
	char ch;
	str=s;
	cout<<str;
    while(1){
    	ch=getchar();
    	if(breakCheck(ch) && ch!=10 && ch!=127){
    		cout<<ch;
    		str=str+ch;
    	}
    	else if(ch==127){
    		int n=str.length();
    		str=str.substr(0,n-1);
    		cout<<"\033[2K";
    		cout << "\x1B["<<console_size-1<<";1H";	    
    	    cout<<":"<<str;	
    	}
    	else break;
    }
    if(ch==27){
    	break;
    }
	removeSpace(str,v);
	cout << "\x1B["<<console_size-1<<";1H";
	cout<<"\033[2K";
	cout << "\x1B["<<console_size-2<<";1H";
	cout<<"\033[2K";
	cout << "\x1B["<<console_size<<";1H";
	vector <struct dirent *> en;
		

	if(m.find(v[0])!=m.end()){
	
	switch(m[v[0]]){
		case 0: //copy 
				if(v.size()>2)
				res=copyFile(path,v);
				else{
					res="Invalid Syntax of Copy Command";
				}
				
				break;
		case 1: //move
				if(v.size()>2)
				res=moveFile(path,v);
				else{
					res="Invalid Syntax of Move Command";
				}
				break;
		case 2: //Rename
				if(v.size()==3)	
				res=renameFile(path,v);
				else{
				 res="Invalid Syntax of Rename Command";
				}
				break;
		case 3: //create File
				v[2]=realtiveToAbsolute(v[2]);
				if(v.size()==3 && v[2].find(root) != string::npos)
				res=createFile(path,v);
				else{
					res="Invalid Syntax of Create File Command";
				}
				
				break;
		case 4: //Create Folder 
		        v[2]=realtiveToAbsolute(v[2]);
				if(v.size()==3 && v[2].find(root) != string::npos)
				res=createFolder(path,v);
				else{
					res="Invalid Syntax of Create Folder Command";
				}
			
				break;
		case 5: //Remove File
				formatChanging(v);
				v[1]=realtiveToAbsolute(v[1]);
				if(v.size()==3 && v[1].find(root) != string::npos)
				res=removeFile(path,v);
				else{
					res="Invalid Path";
				}
				break;				 		
		case 6: //Remove Folder
				
				formatChanging(v);
				v[1]=realtiveToAbsolute(v[1]);
				if(v.size()==3 && v[1].find(root) != string::npos)
				res=removeFolder(path,v);
				else{
					res="Invalid Path";
				}
				break;
		case 7: //goto
				v[1]=realtiveToAbsolute(v[1]);
				DIR *dir;
				if (v[1].find(root) != string::npos && opendir(v[1].c_str())!=NULL) {
				back_dir.push(string(realpath(cwd,NULL)));
				path =v[1].c_str();	
				strcpy(cwd,v[1].c_str()); 
				cout << "\x1B[2J\033[;H";
				mouse(cwd);	
				}
				else{
					res="Invalid Path";
				}
				break;

		case 8:	//search 
				cout << "\x1B[2J\033[;H";
				searchFile(cwd,v[1].c_str());
				cout << "\x1B["<<console_size<<";1H"<<"Search Completed";		
				searchFlag=0;
				break;
		case 9: //Snapshot
				v[1]=realtiveToAbsolute(v[1]);
				if(v.size()==3 && v[1].find(root) != string::npos ){
				string strSnap="";
				if(printSnapShot(v[1].c_str(),strSnap)){
				snapFile(v[1].c_str(),strSnap,v[2]);
				res="Snapshot Printing Done!!";
				}
				else{
					res="Folder Doesnot exists";
				}

				}
				else{
					res="Invalid Syntax for Snapshot Command";
				}
				break;
		}
		if(searchFlag){
		cout << "\x1B[2J\033[;H";
		gotoloc(path,en);
		prints(en,path,0,console_size-5,console_width);
		}		
		cout<<"\x1B["<<console_size-1<<";1H";
		cout << "\x1B["<<console_size<<";1H"<<res;
		cout<<"\x1B["<<console_size-1<<";1H:";
	}
	else{

		cout << "\x1B[2J\033[;H";
		gotoloc(path,en);
		prints(en,path,0,console_size-5,console_width);
		cout << "\x1B["<<console_size-1<<";1H";
		cout<<"\033[2K";
		cout << "\x1B["<<console_size-2<<";1H";
		cout<<"\033[2K";
		cout << "\x1B["<<console_size<<";1H";
		cout<<"Invalid Command";
		cout << "\x1B["<<console_size-1<<";1H:";
		
	}
	
	
	}
	cout << "\x1B[2J\033[;H";	
	mouse(path);
}
void mouse(const char* cwd){
	int m=0,flag=1;
	struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
	console_size = size.ws_row;
	console_width =size.ws_col;
	cout << "\x1B[2J";
	vector <struct dirent *> ent;
	gotoloc(cwd,ent);
	prints(ent,cwd,0,console_size-5,console_width);
	char c;
	int top=0, end, end_mouse=console_size-4;
	int et=console_size-4;
	end=ent.size();
	
	cout << "\x1B[1;1H";

	while(1){
	c=getchar();
	if(c=='q'){settingConicalMode();
	cout << "\x1B[2J\033[;H";
	cout<<"Crafted By Kshitij Paliwal :P \n"; 
		
	exit(1);}	
	if(m>=0 && m<end ){	
		if(c==65 && m>=0){
			printf("\033[1A");
			if(m!=0)
			m--;
			else if(m==0){
				if(top!=0 && end_mouse<=end){
				top--;
				end_mouse--;
				cout << "\x1B[2J\033[;H";
				cout << "\x1B[1;1H";
				prints(ent,cwd,top,end_mouse-1,console_width);
				cout << "\x1B[1;1H";
			}
		}
			
		}

		if(c==66 && m<end-1){
			if(m!=et-1 && m+top!=end-1){
			printf("\033[1B");
			m++;
			}
			else if(m==et-1 && end_mouse<end){
				top++;
				end_mouse++;
				cout << "\x1B[2J\033[;H";
				cout << "\x1B[1;1H";
				prints(ent,cwd,top,end_mouse-1,console_width);
				cout<< "\x1B["<<et<<";1H";
			}
		}

		if(c==10){
			cout<<"\x1B[2J\033[;H";
			check(cwd,top+m,ent);
		}
		if(c==104){
			cout<<"\x1B[2J\033[;H";
			mouse(root.c_str());
		}
		if(c==':'){
			commandMode(cwd);
			prints(ent,cwd,top,end_mouse-1,console_width);
		}
		if(c==127){
			if(root.compare(realpath(cwd,NULL))!=0){
			back_dir.push(string(realpath(cwd,NULL)));	
			cout << "\x1B[2J\033[;H";
			cout << "\x1B[1;1H";	
 			mouse((string(cwd)+"/"+"..").c_str());}
 		}
		if(c==68 && back_dir.size()>1 ){
			string s1=back_dir.top();
			s1=realpath(s1.c_str(),NULL);
			forward_dir.push(cwd);
			back_dir.pop();
			cwd=s1.c_str();
			cout << "\x1B[2J\033[;H";
			cout << "\x1B[1;1H";
			mouse(cwd);
		}
		if(c==67 && forward_dir.size()>0){
			string s1 = forward_dir.top();
			s1=realpath(s1.c_str(),NULL);
			forward_dir.pop();
			back_dir.push(cwd);
			cwd=s1.c_str();
			cout << "\x1B[2J\033[;H";
			cout << "\x1B[1;1H";
			mouse(cwd);
		}
	}

  }
		return;	 
}
int main()
{	settingNonConicalMode();
	getcwd( cwd, PATH_MAX );
  	root=string(cwd);
  	back_dir.push(root);
  	setRoot(root );
	cout << "\x1B[2J";
	cout << "\x1B[1;1H";
	cout<<"\e[3h";
	
	mouse(cwd);
	settingConicalMode();
	return 0;
}