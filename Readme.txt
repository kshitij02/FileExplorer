File Explorer 
A functional terminal based file explorer

Contains 2 Mode :-
1.Normal Mode
2.Command Mode

1.Normal Mode: 
1.Read and display list of files and directories in the current folder- 
a.The following attributes are visible for each file 
i.File Name 
ii.File size 
iii.Ownership (User & Group) & Permissions 
iv.Last modified 
b. User is able to navigate up & down the file list using  corresponding arrow keys in vertical overflow.  
  
2. Open files & directories- 
a. When enter is pressed- 
i.Directory​ -Navigate into the directory and show the files & directories inside directories as specified in point 1. 
ii.Files​ -File explorer opens files using the corresponding default application.

3. Traversal- 
a. Go Back-On pressing left arrow key the file explorer goes back to the previously visited directory. 
b. Go Forward- On pressing right arrow key the file explorer goes forward to the next directory.
c. Up one level- On pressing backspace go up one level. 
d. Home - On pressing ‘h’ key,go to the home folder.

4. To exit application press 'q'
2. Command Mode​ : 
The application enters the command mode whenever the “:” (colon) key  
is pressed.  
>Copy‘:copy <source_file> <destination_directory>’ 
>Move‘:move <source_file> <destination_directory>’ 
>Rename ‘:rename <old_filename> <new_filename>’ 
> Create file ‘:create_file <file_name> <destination_path>’ 
>Create directory ‘:create_dir <dir_name> <destination_path>’ 
>Delete directory: ‘:delete_dir <directory_path>’  
> Goto ‘:goto <location>’ 
Absolute/Relative path of directory w.r.t. application root can be given. 
> Search a file or folder given full filename. 
	Command ​ ‘:search <filename>’ Search for the given filename under the current directory recursively. 
	 Output Is absolute paths shown in the normal mode. 
> Snapshotting the filesystem and dump into a file 
	Command ‘​ :snapshot <folder> <dumpfile>​ ’ 
	Given a base directory this command recursively crawl the directory and store the output in dumpfile. 
> On pressing ‘ESC’ key the application goes back to Normal Mode. 
>The file/directory path can be even relative to the root from where the application is  
started​ .


Assumptions :- 
 1. File names doesn't contains spaces 
 2. One file /folder is copied/moved at a time for best response.
 3. In command mode arrow keys are not to be used or any such key which results in genration of ASCII value of ESC key. 