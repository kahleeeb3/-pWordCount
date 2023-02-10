# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# -I. is the current directory
# $@ is the target name for the current rule
# $^ is the entire dependencies list
# $< is the first item in the dependencies list

CC=gcc# the compiler we will use

# CONFIG FILE
CONF_FILE = True
ifeq ($(CONF_FILE),True)
	CFLAGS=-lconfig -Wall -I./include
else
	CFLAGS=-Wall -I./include
endif

TARGET = pwordcount# name of the executable file
INC_LIST = $(TARGET).h # list the header file names
OBJ_LIST = $(TARGET).o $(TARGET)func.o # list the object file names

OBJ_DIR=obj# directory for storing object files
SRC_DIR=src# directory for storing source code
INC_DIR=include# directory for storing header files

# add directories to the header and object file names
INCS = $(patsubst %,$(INC_DIR)/%,$(INC_LIST)) # add INC_DIR to INC_LIST
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(OBJ_LIST)) # add OBJ_DIR to OBJ_LIST

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

.PHONY: clean # keeps make from doing something with a file named clean

clean: #remove object files and executable
	rm -f $(OBJ_DIR)/*.o *~ core $(TARGET)

run: # run the executable
	./$(TARGET) input.txt

debug:
	gdb --args $(TARGET) input.txt
