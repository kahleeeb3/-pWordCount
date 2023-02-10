# pWordCount
 A Pipe‐based WordCount Tool

## Description
This project aims at providing you with an opportunity to design and implement a C
program, where two processes are cooperating through Unix Pipes. Pipes is one of the
first interprocess communication mechanisms or IPCs in early Unix systems. Unix pipes
offer simple yet efficient ways of communicating among collaborating processes. In this
project, we focus on ordinary pipes that enable two processes to communicate in a
producer-consumer fashion. In other words, a producer process writes to one end of a
pipe (i.e., write-end) and a consumer process reads from the other end (i.e., read-end).

Your program should create two cooperating processes, which communicate through
Unix ordinary pipes. After the first process opens and reads a text file, the process sends
the file content to the second process. The second process is in charge of counting the
number of words and reporting the result back to the first process. 
