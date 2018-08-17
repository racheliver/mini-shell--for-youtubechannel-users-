user name: rachelive
name: racheli 

***********what this program do************
This software is a terminal (shell/bash) simulator with linux command ,
This shell print the user and the host in this format: user@current dir>
then waiting for command, and try to run the command that the user write.




**********Description of the function*******************
#DisplayPrompt:print the user and the host in this format: user@current dir> 

#execvpFunction: create array from the input and send the array to execvp.

  

********** How to Compile this program*************
in order to compile in linux needed to use gcc command
worthwhile compile by using the file "makefile" to compile all files.
using the command : "make"





***********How to run this program***********
command line: "./<the name of the file that the makefile created>"
(in this case "./prog").





**********Description of program files*************
#ex1.c -hold the main and the functions 
#makefile - this file compiling the file type .c and creating an "out" file that contain the program.





**********Description What input we needed*************
the input need to be real command.
	(you can write not real command - the program print: "command not found").
	for exit write the word done and press enter.






***********An explanation of the output we get************
if the command not exist - the output will: "command not found".
	else the output will the out that we given from the orginal sheel
	for example - for the command: ls -l :
	 the orginal sheel display list of the files that there are in the folder
	and this program display the same.
