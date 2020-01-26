/*
Student:Christian Lockley
School:University of North Florida Spring 2020
Class:COP 4610 – Operating Systems
Professor:Dr. Sanjay Ahuja
*/

#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <array>
#include <ctype.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>

static const char * commands[] = {"cd", "dir", "type", "del", "ren", "copy"};
enum COMMANDS {CD = 0, DIR = 1, TYPE = 2, DEL = 3, REN = 4, COPY = 5};
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

size_t commandToNumber(const char *s) {
	unsigned char ret = 0;
	for (size_t i = 0; i < ARRAY_SIZE(commands); ++i) {
		if (strcasecmp(s, commands[i]) == 0) {
			return i;
		}
	}
	return -1;
}

int main() {
	std::string input;
	std::vector<char const *> command;
	std::deque <char> word;
	std::cout << "Type-C to exit\n";
	while (std::getline(std::cin, input)) {
		for (size_t i = 0; i <= input.length(); ++i) {
			if (isspace(input[i]) && word.size() > 0) {
				std::string tmp (word.begin(), word.end());
				tmp.erase(std::remove_if(tmp.begin(), tmp.end(), ::isspace), tmp.end());
				command.push_back((char const *)strdup(tmp.c_str()));
				word.clear();
			} else if (i == input.length() && word.size() > 0) {
				std::string tmp (word.begin(), word.end());
				tmp.erase(std::remove_if(tmp.begin(), tmp.end(), ::isspace), tmp.end());
				command.push_back((char const *)strdup(tmp.c_str()));
				word.clear();
			} else {
				word.push_back(input[i]);
			}
		}
		command.push_back(NULL);
		if (commandToNumber(command[0]) == CD) {
			std::cout << "Changing directory to " << command[1] << "\n";
			chdir(command.at(1));
			for (int i = 0; i < command.size()-1; ++i) {
				free((void*)command.at(i));
			}
			command.clear();
			continue;
		}
		pid_t cp = fork();
		if (cp == 0) {
			switch (commandToNumber(command[0])) {
				case DIR:
					command[0] = "ls";
					execvp("ls", (char * const*)command.data());
				break;
				case TYPE:
					command[0] = "cat";
					execvp("/bin/cat", (char * const*)command.data());
				break;
				case DEL:
					command[0] = "rm";
					execvp("rm", (char * const*)command.data());
				break;
				case REN:
					command[0] = "mv";
					execvp("mv", (char * const*)command.data());
				break;
				case COPY:
					command[0] = "cp";
					execvp("cp", (char * const*)command.data());
				break;
				default:
					execvp(command[0], (char * const*)command.data());
					break;
			}
			quick_exit(1);
		} else if (cp < 0) {
			abort();
		} else {
			int x = 0;
			for (int i = 0; i < command.size()-1; ++i) {
				free((void*)command.at(i));
			}
			waitpid(cp, &x, 0);
			fflush(NULL);
		}
		command.clear();
	}
}