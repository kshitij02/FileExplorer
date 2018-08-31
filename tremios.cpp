#include <stdlib.h>
#include<termios.h>
#include <sys/ioctl.h>
#include "functions.h"

stack <string> back_dir;
stack <string> forward_dir;
char cwd[PATH_MAX];
char *root;
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
	//char * cwd="/home/ksh";
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  	
}

void settingConicalMode(){
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
void check(const char *cwd,int m,vector <struct dirent *>&ent){
	DIR *dir;
	
    struct stat stat_buf;
    //cout<<m;
    chdir(cwd);
    stat(ent[m]->d_name, &stat_buf);
    string str=(string(cwd)+"/"+string(ent[m]->d_name));
    	cout<<"\x1B[2J\033[;H";
    	if(ent[m]->d_type==DT_DIR){
    	back_dir.push(string(cwd));
    	mouse(str.c_str());
    }
    	else{
    	 pid_t pro;
    	 pro=fork();
    	 if(pro==0){
    	 	execl("/usr/bin/xdg-open","xdg-open",ent[m]->d_name,NULL);
    	 	exit(1);
    	 }
    	 else{
    	 	chdir("..");
    	 	mouse(cwd);
    	 }	
    	}
    
    	//cout << "\x1B[1;1H";

  	//closedir (dir);
  	//chdir("..");
} 
void snapFile(const char* path ,string &strSnap){
	chdir(path);
	ofstream snapFile;
	string fileName = string(path)+"/snapShot.txt";
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
//	settingConicalMode();

	while(1){
	vector<string> v;
	int searchFlag=1;
	string str="";
	char s;
	cin>>s;
	//cout<<s;
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
    		//cout<<str;
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
    //cout<<str;
    if(ch==27){
    	break;
    }
	removeSpace(str,v);
	//auto x =m.find(v[0]);
	//cout<<v[0];
	cout << "\x1B["<<console_size-1<<";1H";
	cout<<"\033[2K";
	cout << "\x1B["<<console_size-2<<";1H";
	cout<<"\033[2K";
	cout << "\x1B["<<console_size<<";1H";
	vector <struct dirent *> en;
		

	if(m.find(v[0])!=m.end()){
	
	switch(m[v[0]]){
		case 0: //cout<<"Copy File";
				copyFile(path,v);
				break;
		case 1: //cout<<"Move File";
				moveFile(path,v);
				break;
		case 2: //cout<<"Rename";
				renameFile(path,v);
				break;
		case 3: //cout<<"create file";
				//cout<<v.size()<<v[1]<<" "<<v[2]; 
				createFile(path,v);
				break;
		case 4: //cout<<"create_dir";
				createFolder(path,v);
				break;
		case 5: //cout<<"delete_file";
				formatChanging(v);
				removeFile(path,v);
				break;				 		
		case 6: //cout<<"delete_dir";
				formatChanging(v);
				removeFolder(path,v);
				break;
		case 7: //cout<<"goto";
				path =v[1].c_str();	
				strcpy(cwd,v[1].c_str()); 
				cout << "\x1B[2J\033[;H";
				mouse(cwd);	
				break;

		case 8:	cout << "\x1B[2J\033[;H";
				searchFile(cwd,v[1].c_str());
				searchFlag=0;
				break;
		case 9: // cout<<"snapshot";
				string strSnap="";
				printSnapShot(v[1].c_str(),strSnap);
				snapFile(v[1].c_str(),strSnap);
				break;
	//	default : cout<<"Command Doesn't exists";		
		}
		if(searchFlag){
		cout << "\x1B[2J\033[;H";
		gotoloc(path,en);
		prints(en,path,0,console_size-5,console_width);
		}		
		cout << "\x1B["<<console_size-1<<";1H:";
		//cout<<v[1];
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
	//settingNonConicalMode();
	//char ch =getchar();
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
	//cout<<ent.size()<<endl;
	//for(int i=0;i<ent.size();i++)
	//	cout<<ent[i]->d_name<<endl;
	char c;
	int top=0, end, end_mouse=console_size-4;//10;
	int et=console_size-4;//10;
	end=ent.size();
	cout << "\x1B[1;1H";
	//cout <<console_size;
	while(1){
	c=getchar();
	if(c=='q'){settingConicalMode();
	cout << "\x1B[2J\033[;H";	
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
			//cout<<top+m;
			cout<<"\x1B[2J\033[;H";
			check(cwd,top+m,ent);
		}
		if(c==104){
			cout<<"\x1B[2J\033[;H";
			mouse(root);
		}
		if(c==':'){
			commandMode(cwd );
		//	cout << "\x1B[2J\033[;H";
			//ent.resize(0);
			//gotoloc(cwd,ent);
			//top=0;
			//end_mouse-1=0;
			prints(ent,cwd,top,end_mouse-1,console_width);
		}
		if(c==127){
			mouse((string(cwd)+"/"+"..").c_str());
		}
		if(c==68 && back_dir.size()>1 ){
			//cout<<"back";
			string s1=back_dir.top();
			forward_dir.push(s1);
			cwd=s1.c_str();
			mouse(cwd);
		}
		if(c==67 && forward_dir.size()!=0){
			//cout<<"forward";
			string s1 = forward_dir.top();
			back_dir.push(s1);
			cwd=s1.c_str();
			mouse(cwd);
		}
	}

  }
		return;	 
}
int main()
{	settingNonConicalMode();
	getcwd( cwd, PATH_MAX );
  	root=cwd;
  	back_dir.push(root);
	cout << "\x1B[2J";
	cout << "\x1B[1;1H";
	cout<<"\e[3h";
	mouse(cwd);
	settingConicalMode();
	//cout << "\x1B[2J";
	//cout << "\x1B[1;1H";
//	cout<<"Kuch Nahi dekhhga \n"; // place cursor at home position
	return 0;
}