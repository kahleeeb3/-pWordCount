#define BUFFER_SIZE read_param("BUFFER_SIZE","config.cfg")
// #define BUFFER_SIZE 25
#define READ_END 0	   // define the read_end of the file descriptor
#define WRITE_END 1	   // define the write_end of the file descriptor

void check_input(int *argument_count);

void create_pipe(int pipefd[2]);

FILE * get_file_pointer(const char *file_name);

const int get_file_size(FILE *file_pointer);

int chunk(FILE *file_pointer, const int *file_size, char *buffer, int *file_offset);

int count_spaces(int *word_count, const char *array);

int read_param(char *param_name, char *file_name);
