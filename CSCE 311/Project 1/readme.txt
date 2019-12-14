This program takes in an input file and then parses is content to print out the sentences that contains the word the user requests.

There are 2 parts to this program:
1.) fork() - A process is forked. Parent process reads the file and child parses the file
2.) Unix Domain Sockets - Parent writes to a socket pair and child reads from the socket pair

Usage: ./a.out <filename> <word>
