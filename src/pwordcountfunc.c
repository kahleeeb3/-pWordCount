/*
* COMP 7500: Project 2 - A Pipe‐based WordCount Tool
* Caleb Powell
* Auburn University
*
* This source code contains important functions used
* in the pwordcount.c source code. Functions use 
* explained below.
* 
*/

#include <stdio.h> // for opening files
#include <string.h> // string length function
#include <stdlib.h> // for malloc
#include <pwordcount.h> // header file
#include <unistd.h>	  //POSIX functions
#include <libconfig.h> // config file


/*
* Checks that the total number of parameters passed in the command
* prompt is correct. Exits program if incorrect.
* @param argument_count		number of arguments passed to command
*/
void check_input(int *argument_count){
	if (*argument_count != 2)
	{
		printf("Please enter a file name.\n");
		printf("Usage: ./pwordcount <file_name>\n");
		exit(1); // kill process
	}
}

/*
* Creates a ordinary unix pipe. If pipe fails, program exits.
* @param pipefd[2]		pipe file descriptor. 
*/
void create_pipe(int pipefd[2]){
	if (pipe(pipefd) == -1)
	{
		perror("Creating Pipe Failed");
		exit(1); // kill process
	}
}

/*
* Opens a file for read access. File must be a .txt extension file.
* If some other extension, file does not exist, or cant be opened,
* print message is displayed and program exits.
* @param file_name		String that represents name of file to be opened
*/
FILE * get_file_pointer(const char *file_name){

	// check that file is .txt
	int len = strlen(file_name);

  	if (len >= 4 && strcmp(file_name + len - 4, ".txt") != 0) {
    	printf("File must have a .txt extension\n");
		printf("Usage: ./pwordcount <file_name>\n");
		exit(1); // kill process
  	}

	// read the file
	FILE *fp;					// pointer to file descriptor
	fp = fopen(file_name, "r"); // open with read access
	if (fp == NULL)
	{
		perror("Can't open file");
		printf("Usage: ./pwordcount <file_name>\n");
		exit(1); // kill process
	}
	return fp;
}

/*
* Gets the size of a file by jumping to the end of the file pointer
* and recording the number of bytes at that point.
* @param file_pointer		pointer to a system file created using get_file_pointer()
*/
const int get_file_size(FILE *file_pointer){
	fseek(file_pointer, 0, SEEK_END); // go to end of file
	const int size = ftell(file_pointer); // get num of char
	return size;
}

/*
* Stores a chunk of file data into a buffer. Jump to last chunk point 
* (starting from beginning of file) and fills the buffer with as much
* data from the file as possible. If file still has data left, set a 
* new offset point and return a vlaue of 1. Else, return a value of 0.
* @param file_pointer		pointer to a system file
* @param file_size			pointer to the size of the file in integer bytes
* @param buffer				buffer of size BUFFER_SIZE which stores file data
* @param file_offset		last point where data was chunked from the file
*/
int chunk(FILE *file_pointer, const int *file_size, char *buffer, int *file_offset){
	if(*file_offset > *file_size){
		return 0; // stop chunking
	}
	else{
		fseek(file_pointer, *file_offset, SEEK_SET); // jump to the start of offset
		fgets(buffer, BUFFER_SIZE, file_pointer); // starting from offset, fill buffer
		*file_offset += BUFFER_SIZE - 1; // update offset
		return 1; // keep chunking
	}
}

/*
* Counts the number of Space, Tab, and New Line Characters in a given string
* @param word_count		pointer to an integer which stores the current number of space char
* @param array			array of chars to count the number of spaces within
*/
int count_spaces(int *word_count, const char *array){
	int i; // index of the array
	for(i=0; i<=strlen(array); i++){
		if(array[i]==' '||array[i]=='\n'||array[i] == '\t'){
			*word_count += 1;
		}
	}
	return 0;
}


/*
* Reads parameter values from a configuration file using the libconfig library
* @param param_name 	name of the parameter to search for in config file
* @param file_name 		name of the config file that stores parameter values ("config.cfg")
*/
int read_param(char *param_name, char *file_name){
	// INSTALL: sudo yum install libconfig-devel
	// http://simplestcodings.blogspot.com/2012/02/libconfig-to-read-configuration-files.html
	// https://www.hyperrealm.com/libconfig/libconfig_manual.html
	config_t cfg;
    config_setting_t *setting;
	int param_value;
    config_init(&cfg);

    if (!config_read_file(&cfg, file_name)) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(&cfg),
			config_error_line(&cfg),
			config_error_text(&cfg));
        config_destroy(&cfg);
        return -1;
    }

    setting = config_lookup(&cfg, "params");
    if (setting != NULL) {
        config_setting_lookup_int(setting, param_name, &param_value);
    }
	config_destroy(&cfg);
    return param_value;

}