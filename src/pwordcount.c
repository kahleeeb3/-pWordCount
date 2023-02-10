/*
* COMP 7500: Project 2 - A Pipe‐based WordCount Tool
* Caleb Powell
* Auburn University
*
* This source code creates two cooperating processes, 
* which communicate through Unix ordinary pipes. 
* After the first process opens and reads a text file,
* the process sends the file content to the second
* process. The second process is in charge of counting
* the number of words and reporting the result back to
* the first process.
*
* How to compile?
* make
*
* How to run?
* Case 1: Less than 2 arguments. Sample usage is printed
* $./pwordcount
* Please enter a file name.
* Usage: ./pwordcount <file_name>
*
* Case 2: File is not a txt. Sample usage is printed
* $./pwordcount input.docx
* File must have a .txt extension
* Usage: ./pwordcount <file_name>
*
* Case 3: File does not exist. Sample usage is printed
* $./pwordcount notafile.txt
* Can't open file: No such file or directory
* Usage: ./pwordcount <file_name>
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>	  //POSIX functions
#include <sys/wait.h> // wait function
#include <pwordcount.h>
#include <signal.h> // for pausing the process

int main(int argc, char *argv[])
{
	check_input(&argc); // check for valid user input
	char write_msg[BUFFER_SIZE]; // store the write content string
	char read_msg[BUFFER_SIZE];	 // store the read content string
	
	int fd1[2]; // declare first file descriptor
	int fd2[2]; // declare second file descriptor
	
	create_pipe(fd1); // create pipe for sending file content
	create_pipe(fd2); // create pipe for sending word count

	pid_t child_pid = fork(); // fork the process

	// Error
	if (child_pid < 0)
	{
		fprintf(stderr, "Fork failed");
		return 1;
	}

	// Parent Process
	if (child_pid > 0)
	{
		int status = 0; // wait status
		int offset = 0; // point of last chunk
		int count = 0; // word count recv from P2
		
		// open file
		printf("Process 1 is reading file “%s” now ...\n",argv[1]);
		FILE *fp = get_file_pointer(argv[1]);
		const int fsz = get_file_size(fp);

		// send chunks
		close(fd1[READ_END]); // writing: close read-descriptor.
		while (chunk(fp, &fsz, write_msg, &offset) != 0) // while(chunks left):
		{
			printf("Process 1 starts sending data to Process 2 ...\n");
			write(fd1[WRITE_END], write_msg, strlen(write_msg) + 1); // write chunk
		}
		fclose(fp); // close the file
		close(fd1[WRITE_END]); // done writing: close write-descriptor.
		waitpid(child_pid, &status,0); // wait for child to finish

		// get the count
		close(fd2[WRITE_END]); // reading: close write-descriptor.
		read(fd2[READ_END], &count, sizeof(count)); // read count from P2
		close(fd2[READ_END]); // done reading: close read-descriptor.
		printf("Process 1: The total number of words is %d.\n",count);

	}

	// Child Process
	else
	{
		int word_count = 0; // words counted
		close(fd1[WRITE_END]); // reading: close write-descriptor.
		while (read(fd1[READ_END], read_msg, BUFFER_SIZE) > 0) // while(stuff to read):
		{
			printf("Process 2 finishes receiving data from Process 1 ...\n");
			printf("Process 2 is counting words now ...\n");
			count_spaces(&word_count, read_msg); // count number of spaces in read_msg
		}
		close(fd1[READ_END]); // done reading: close read-descriptor.
		word_count += 1; // word_count = num_spaces+1
		
		printf("Process 2 is sending the result back to Process 1 ...\n");
		close(fd2[READ_END]); // writing: close read-descriptor.
		write(fd2[WRITE_END], &word_count, sizeof(word_count));
		close(fd2[WRITE_END]); // done writing: close write-descriptor.

	}

	return 0;
}
