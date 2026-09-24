#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "file_utils.h"
#include "sort_utils.h"

#define FAIL(x) do { exit_code = (x); goto cleanup; } while (0)

typedef enum {
    EXIT_OK = 0,
    EXIT_INCORRECT_CMD_ARGUMENTS,
    EXIT_CANNOT_INDEX_FILE,
    EXIT_CANNOT_OPEN_OUTPUT_FILE,
    EXIT_CANNOT_WRITE_POEM_FIRST_PART,
    EXIT_CANNOT_WRITE_POEM_SECOND_PART,
    EXIT_CANNOT_WRITE_ORIG_POEM,
    EXIT_CANNOT_CLOSE_OUTPUT_FILE
} ExitCode;

int main(int argc, char *argv[]) {
    ExitCode exit_code = EXIT_OK;
    FILE *output_file_ptr = NULL;
    IndexedFile idx_file = {};

    if (argc != 3)  {
        printf("Correct usage:\n"
               "%s <file_with_poem> <output_file>\n", argv[0]);
        FAIL(EXIT_INCORRECT_CMD_ARGUMENTS);
    }

    FileStatus status = create_indexed_file(argv[1], &idx_file);
    if (status != FS_OK) {
        fprintf(stderr, "[ERROR] '%s': %s\n", argv[1], fs_error_str(status));
        FAIL(EXIT_CANNOT_INDEX_FILE);
    }


    output_file_ptr = fopen(argv[2], "w");
    if (output_file_ptr == NULL) {
        fprintf(stderr, "[ERROR] '%s': cannot open file\n", argv[2]);
        FAIL(EXIT_CANNOT_OPEN_OUTPUT_FILE);
    }

    qsort(idx_file.index, idx_file.lines_numb, sizeof(idx_file.index[0]), strcmp_letters_only);

    status = write_poem(output_file_ptr, &idx_file, "Onegin sorted via default qsort:");
    if (status != FS_OK) {
        fprintf(stderr, "[ERROR] '%s': %s\n", argv[2], fs_error_str(status));
        FAIL(EXIT_CANNOT_WRITE_POEM_FIRST_PART);
    }

    myqsort(idx_file.index, idx_file.lines_numb, sizeof(idx_file.index[0]), strcmp_letters_only_rev);

    status = write_poem(output_file_ptr, &idx_file, "Onegin sorted via myqsort and strings compared from right to left:");
    if (status != FS_OK) {
        fprintf(stderr, "[ERROR] '%s': %s\n", argv[2], fs_error_str(status));
        FAIL(EXIT_CANNOT_WRITE_POEM_SECOND_PART);
    }

    status = write_orig_poem(output_file_ptr, &idx_file, "Original Onegin:");
    if (status != FS_OK) {
        fprintf(stderr, "[ERROR] '%s': %s\n", argv[2], fs_error_str(status));
        FAIL(EXIT_CANNOT_WRITE_ORIG_POEM);
    }

cleanup:
    if (output_file_ptr != NULL && fclose(output_file_ptr) == EOF) {
        fprintf(stderr, "[ERROR] '%s': cannot close file\n", argv[2]);

        if (exit_code == EXIT_OK)
            exit_code = EXIT_CANNOT_CLOSE_OUTPUT_FILE;
    }

    destroy_indexed_file(&idx_file);

    return exit_code;
}

