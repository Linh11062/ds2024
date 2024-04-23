#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

char* longest_path_in_file(const char* file_path) {
    char* longest_path = NULL;
    size_t longest_length = 0;

    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        char* trimmed_line = strtok(line, "\n");
        size_t line_length = strlen(trimmed_line);
        if (line_length > longest_length) {
            longest_length = line_length;
            if (longest_path != NULL) {
                free(longest_path);
            }
            longest_path = strdup(trimmed_line);
        }
    }

    fclose(file);

    return longest_path;
}

int main() {
    const char* file_path = "file5.txt";
    char* longest_path = longest_path_in_file(file_path);
    if (longest_path != NULL) {
        printf("The longest path in %s is: %s\n", file_path, longest_path);
        free(longest_path);
    }

    return 0;
}
