#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "file_utils.h"
#include "sort_utils.h"


typedef enum {
    INDEX_OK,
    INDEX_TOO_MANY_LINES
} IndexStatus;


static IndexStatus index_lines(char *file_content, size_t content_len, char *index[], uint16_t *out_lines_numb) {
    assert(index != NULL);
    assert(file_content != NULL);
    assert(out_lines_numb != NULL);

    index[0] = file_content;
    uint16_t counter = 1;

    while (counter < 10000) {
        char *addr = strchr(index[counter - 1], '\n'); // load_file_alloc adds '\0' at the end of written data, so strchr won't search for '\n' infinitely
        if (addr == NULL)
            break;

        if (addr == file_content + content_len - 1) {
            *addr = '\0';
            break;
        }

        *addr = '\0';
        index[counter] = addr + 1;

        counter++;
    }

    if (counter >= 10000)
        return INDEX_TOO_MANY_LINES;

    *out_lines_numb = counter;

    return INDEX_OK;
}

int main(int argc, char *argv[]) {
    if (argc != 3)  {
        printf("Correct usage:\n%s <file_with_poem> <output_file>\n", argv[0]);
        return 1;
    }

    size_t buff_len = 0;
    char *file_content = NULL;
    FileStatus status = load_file_alloc(argv[1], &file_content, &buff_len);
    if (status != FS_OK) {
        fprintf(stderr, "[ERROR] '%s': %s\n", argv[1], fs_error_str(status));
        return 2;
    }

    char *index[10000] = {};
    uint16_t lines_numb = 0;
    IndexStatus status_ind = index_lines(file_content, buff_len, index, &lines_numb);
    if (status_ind == INDEX_TOO_MANY_LINES) {
        fprintf(stderr, "[ERROR] '%s' has more than 10000 lines\n", argv[1]);
        free(file_content);
        return 3;
    }

    FILE* output_file_ptr = fopen(argv[2], "w");
    if (output_file_ptr == NULL) {
        fprintf(stderr, "[ERROR] '%s': cannot open file\n", argv[2]);
        free(file_content);
        return 4;
    }

    qsort(index, lines_numb, sizeof(index[0]), strcmp_letters_only);

    status = write_poem(output_file_ptr, index, lines_numb, "Onegin sorted via default qsort:");
    if (status != FS_OK) {
        fprintf(stderr, "[ERROR] '%s': %s\n", argv[2], fs_error_str(status));
        fclose(output_file_ptr);
        free(file_content);
        return 5;
    }

    myqsort(index, lines_numb, sizeof(index[0]), strcmp_letters_only_rev);

    status = write_poem(output_file_ptr, index, lines_numb, "Onegin sorted via myqsort and strings compared from right to left:");
    if (status != FS_OK) {
        fprintf(stderr, "[ERROR] '%s': %s\n", argv[2], fs_error_str(status));
        fclose(output_file_ptr);
        free(file_content);
        return 6;
    }

    status = write_orig_poem(output_file_ptr, file_content, lines_numb, "Original Onegin:");
    if (status != FS_OK) {
        fprintf(stderr, "[ERROR] '%s': %s\n", argv[2], fs_error_str(status));
        fclose(output_file_ptr);
        free(file_content);
        return 7;
    }

    if (fclose(output_file_ptr) == EOF) {
        fprintf(stderr, "[ERROR] '%s': cannot close file\n", argv[2]);
        free(file_content);
        return 8;
    }

    free(file_content);

    return 0;
}

