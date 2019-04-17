#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc < 4) {
		printf("Error: Not enough arguments!\n");
		printf("Usage: ./mytail -n [number of lines] [filename]\n");
		return 1;
	}

	int n = atoi(argv[2]);
	n = n + 1;

	char *filename = argv[3];
	FILE *file = fopen(filename, "r");
	if (file == NULL) return 1;

	struct stat buf;
	int stat_error = stat(filename, &buf);
	if (stat_error != 0) return 1;

	int file_size = buf.st_size;

	fseek(file, 0, SEEK_SET);	
	long original_position = ftell(file);

	int newline_ctr = 0;
	char c;
	fseek(file, -1, SEEK_END);
	int loc = ftell(file);
	while (1) {
		if (fread(&c, 1, 1, file) == 0)
            break;
        fseek(file, -1, SEEK_CUR);

		loc = ftell(file);
//		printf("%c at %d\n", c, loc);


        if (c == '\n') {
            newline_ctr++;
//            printf("Found newline #%d w/ n=%d\n", newline_ctr, n);

			if (newline_ctr == n) {
				fseek(file, 1, SEEK_CUR);
				break;
			}
        }

		if (loc == original_position)
			break;

        fseek(file, -1, SEEK_CUR);

		loc = ftell(file);
	}

//	printf("Done seeking!\n");

	while (n > 0) {
		if (fread(&c, 1, 1, file) == 0)
			break;

		loc = ftell(file);
//		printf("%c at %d\n", c, loc);
		printf("%c", c);

		if (c == '\n') {
			n--;
			if (n == 0)
				break;
		}

	}

/*
	while (1) {
		if (feof(file) != 0)
			break;
		if (ferror(file) != 0)
			break;

		c = fgetc(file);
		if (c == '\n') {
		       	newline_ctr++;
			printf("\nNewlines counted: %d\n", newline_ctr); 
		}

		printf("%c", c);
	}
*/
fclose(file);
	return 0;
}
