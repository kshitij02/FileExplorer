#include <stdlib.h>
#include<termios.h>
#include <sys/ioctl.h>
#include "functions.h"

char cwd[PATH_MAX];
char *root;
struct termios oldt,newt;
	
using namespace std;
void mouse(const char*);

void removeSpace(string str,vector<string> &v)
{
   string word = "";
   for (char x : str)
   {
       if (x == ' ')
       {
           v.push_back(word); 
           cout<<word;
           word = "";
          
       }
       else
       {
           word = word + x;
       }
   } 
   v.push_back(word);

}

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
    	if(ent[m]->d_type==DT_DIR)
    	mouse(str.c_str());
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
//else {
 // perror ("");
//}

//}*/
void commandMode(const char*  path){
	/*map <string,int> m;
	m.insert({":copy" , 0});
    m.insert({":move" , 1});
    m.insert({":rename" , 2});
    m.insert({":create_file ", 3});
    m.insert({":create_dir" , 4 });
    m.insert({":delete_file" , 5}); 
    m.insert({":delete_dir" , 6 });
    m.insert({":goto_dir" , 7 });
    m.insert({":search" , 8 });
    m.insert({":snapshot" , 9 });
	*/struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
	int console_size = size.ws_row;
	cout << "\x1B["<<console_size<<";1H";
	settingConicalMode();
	string str;
	cin>>str;
	while(str!=";"){
	cin>>str;
	}
	settingNonConicalMode();
	char ch =getchar();
	cout << "\x1B[2J\033[;H";	
	mouse(path);
}
void mouse(const char* cwd){
	int m=0,flag=1;
	struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
	int console_size = size.ws_row;
	cout << "\x1B[2J";
	vector <struct dirent *> ent;
	gotoloc(cwd,ent);
	prints(ent,cwd,0,console_size-5);
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
				prints(ent,cwd,top,end_mouse-1);
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
				prints(ent,cwd,top,end_mouse-1);
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
		if(c==';'){
			commandMode(cwd);
		//	cout << "\x1B[2J\033[;H";
			prints(ent,cwd,top,end_mouse-1);
		}
		if(c==127){
			mouse((string(cwd)+"/"+"..").c_str());
		}
	}

  }
		return;	 
}
int main()
{	settingNonConicalMode();
	getcwd( cwd, PATH_MAX );
  	root=cwd;
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