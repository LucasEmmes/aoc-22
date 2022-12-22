#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	int total_length = 0;
	for (size_t i = 0; i < argc; i++) {
		total_length += strlen(argv[i]);
	}
	char* newstring = (char*) malloc(sizeof(char)*total_length);

	int c = 0;
	for (size_t i = 0; i < argc; i++) {
		
	}

	free(newstring);
	return 0;
}
