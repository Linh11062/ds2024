#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_LINE_LENGTH 1024
#define MAX_PAIRS 1000

typedef struct {
    char key[MAX_LINE_LENGTH];
    int value;
} KeyValuePair;

void process_line(const char* line, KeyValuePair* key_value_pairs, int* count) {
    char buffer[MAX_LINE_LENGTH];
    strcpy(buffer, line);

    char* token = strtok(buffer, " \t\n");
    while (token != NULL) {
        KeyValuePair* pair = &(key_value_pairs[*count]);
        strcpy(pair->key, token);
        pair->value = 1;
        (*count)++;
        token = strtok(NULL, " \t\n");
    }
}

void reduce(KeyValuePair* input_pairs, int input_count, KeyValuePair* output_pairs, int* output_count) {
    for (int i = 0; i < input_count; i++) {
        KeyValuePair* input_pair = &(input_pairs[i]);

        int found = 0;
        for (int j = 0; j < *output_count; j++) {
            KeyValuePair* output_pair = &(output_pairs[j]);
            if (strcmp(input_pair->key, output_pair->key) == 0) {
                output_pair->value += input_pair->value;
                found = 1;
                break;
            }
        }

        if (!found) {
            KeyValuePair* new_pair = &(output_pairs[*output_count]);
            strcpy(new_pair->key, input_pair->key);
            new_pair->value = input_pair->value;
            (*output_count)++;
        }
    }
}

void map_reduce(const char* input_file) {
    FILE* file = fopen(input_file, "r");
    if (file == NULL) {
        printf("Failed to open the input file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    KeyValuePair intermediate_key_value_pairs[MAX_PAIRS];
    int intermediate_count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        process_line(line, intermediate_key_value_pairs, &intermediate_count);
    }

    fclose(file);

    KeyValuePair grouped_key_value_pairs[MAX_PAIRS];
    int grouped_count = 0;

    reduce(intermediate_key_value_pairs, intermediate_count, grouped_key_value_pairs, &grouped_count);

    for (int i = 0; i < grouped_count; i++) {
        KeyValuePair* pair = &(grouped_key_value_pairs[i]);
        printf("%s: %d\n", pair->key, pair->value);
    }
}

int main() {
    const char* input_file = "file4.txt";
    clock_t start = clock();

    map_reduce(input_file);

    clock_t end = clock();
    double elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %.2f seconds\n", elapsed_time);

    return 0;
}
