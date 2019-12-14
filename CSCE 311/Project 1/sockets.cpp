#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <algorithm>


using namespace std;

#define BUF_SIZE 4096

// Method to sort the vector of strings
bool sorting (string a, string b) {
  return a < b;
}

// This is the method child uses to parse the content
vector<string> fileparse (string line, string word){
  vector<string> filelines;
  vector<string> results;

  // Use boost to split the string using punctuation marks as delms
  boost::split(filelines, line, boost::is_any_of(".!?"));

  // Use find to find which lines contains the specified word
  for (int i = 0; i < filelines.size(); ++i) {
    size_t found = filelines[i].find(word);
    if (found != string::npos){
      // Push the lines that contain the specified word into a vector
      results.push_back(filelines[i]);
    }
  }

  return results;
}


int main(int argc, char* argv[])
{
	if(argc != 3) {
		cout << "Usage: <filename> <word>" << endl;
		exit(-1);
	}

	char* fpath = argv[1];
  char* word = argv[2];

	int sv[2]; /* the pair of socket descriptors */

	/* Here I used SOCK_STREAM, but you can also consider SOCK_DGRAM which preserves message boundaries. */
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
		perror("socketpair!");
		exit(1);
	}

	int cid = fork();

	if(cid < 0) {
		perror("fork!");
		exit(-1);
	}

	if (cid > 0) {  /* parent */
		int fd = open(fpath, O_RDONLY);
		if (fd == -1) {
			perror("Open!");
			kill(cid, SIGKILL);
			exit(-1);
		}

		ssize_t ret_in, ret_out;    /* Number of bytes returned by read() and write() */
		char buffer[BUF_SIZE];

		while((ret_in = read (fd, &buffer, BUF_SIZE)) > 0){
            		ret_out = write (sv[0], &buffer, (ssize_t) ret_in);
            		if(ret_out != ret_in){
                		/* Write error */
                		perror("write");
                		kill(cid, SIGKILL);
				exit(-1);
            		}
    		}

		/*Send a special character to signal the end of the file*/
		write (sv[0], "\0", 1);

		wait(NULL); /*To reap the zombine child process*/
	}

  else { /* child */
    cout << "+++++++++ CHILD PROCESS +++++++" << endl;
		ssize_t socket_in;
		char buffer[BUF_SIZE];

    // Lines stores all the lines from the socket stream
    string lines;
		while((socket_in = read (sv[1], &buffer, BUF_SIZE)) > 0){
      for (int i = 0; i < socket_in; i++){
        lines += buffer[i];
      }
      if(buffer[socket_in - 1] == '\0')
        break;
    	}
    // cout << lines << endl; //Works

    // Create a vector of strings to store each result
    vector<string> result;

    // Call the fileparse method to parse the file
    result = fileparse(lines, word);

    // Print out the results
    /*for (int i = 0; i < result.size(); i++){
      cout << result.at(i) << endl;
    }*/ //Works

    // Trim each line to remove unnecessary white spaces
    for (int i = 0; i < result.size(); ++i) {
      boost::trim(result.at(i));
    }

    // Sort the vector alphabetically
    sort(result.begin(), result.end(), sorting);

    for (int i = 0; i < result.size(); i++){
      cout << result.at(i) << endl;
    }

    // Convert the vector of strings to a char array


	}

	return 0;
}
