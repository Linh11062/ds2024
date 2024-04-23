#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        FILE *file = fopen("file3.txt", "rb");
        if (file == NULL) {
            fprintf(stderr, "Error: Failed to open file\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *buffer = (char *)malloc(file_size);
        fread(buffer, 1, file_size, file);

        MPI_Bcast(&file_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        MPI_Bcast(buffer, file_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        fclose(file);
        free(buffer);
    } else {
        long file_size;
        MPI_Bcast(&file_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);

        char *buffer = (char *)malloc(file_size);
        MPI_Bcast(buffer, file_size, MPI_CHAR, 0, MPI_COMM_WORLD);

        FILE *file = fopen("received_file.txt", "wb");
        if (file == NULL) {
            fprintf(stderr, "Error: Failed to create file\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fwrite(buffer, 1, file_size, file);
        fclose(file);

        free(buffer);
    }

    MPI_Finalize();
    return 0;
}