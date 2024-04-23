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

void mapper(const char* line, KeyValuePair* key_value_pairs, int* count) {
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

void reducer(KeyValuePair* key_value_pairs, int count) {
    KeyValuePair word_counts[MAX_PAIRS];
    int word_counts_count = 0;

    for (int i = 0; i < count; i++) {
        KeyValuePair* pair = &(key_value_pairs[i]);

        int found = 0;
        for (int j = 0; j < word_counts_count; j++) {
            KeyValuePair* count_pair = &(word_counts[j]);
            if (strcmp(pair->key, count_pair->key) == 0) {
                count_pair->value += pair->value;
                found = 1;
                break;
            }
        }

        if (!found) {
            KeyValuePair* new_pair = &(word_counts[word_counts_count]);
            strcpy(new_pair->key, pair->key);
            new_pair->value = pair->value;
            word_counts_count++;
        }
    }

    for (int i = 0; i < word_counts_count; i++) {
        KeyValuePair* pair = &(word_counts[i]);
        printf("%s: %d\n", pair->key, pair->value);
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
        mapper(line, intermediate_key_value_pairs, &intermediate_count);
    }

    fclose(file);

    KeyValuePair grouped_key_value_pairs[MAX_PAIRS];
    int grouped_count = 0;

    for (int i = 0; i < intermediate_count; i++) {
        KeyValuePair* pair = &(intermediate_key_value_pairs[i]);

        int found = 0;
        for (int j = 0; j < grouped_count; j++) {
            KeyValuePair* group_pair = &(grouped_key_value_pairs[j]);
            if (strcmp(pair->key, group_pair->key) == 0) {
                strcpy(group_pair->key, pair->key);
                group_pair->value += pair->value;
                found = 1;
                break;
            }
        }

        if (!found) {
            KeyValuePair* new_pair = &(grouped_key_value_pairs[grouped_count]);
            strcpy(new_pair->key, pair->key);
            new_pair->value = pair->value;
            grouped_count++;
        }
    }

    reducer(grouped_key_value_pairs, grouped_count);
}

int main() {
    const char* input_file = "cheet.txt";
    clock_t start = clock();

    map_reduce(input_file);

    clock_t end = clock();
    double elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %.2f seconds\n", elapsed_time);

    return 0;
}