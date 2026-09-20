#pragma once

#include <stdint.h>
#include <stdio.h>

typedef enum {
    FS_OK,
    FS_OPEN_ERROR,
    FS_READ_ERROR,
    FS_ALLOC_ERROR,
    FS_CLOSE_ERROR,
    FS_WRITE_ERROR,
    FS_STAT_ERROR
} FileStatus;

const char *fs_error_str(FileStatus error);
FileStatus get_file_size(const char *filename, size_t *out_file_size);
FileStatus load_file_alloc(const char *filename, char **out_buff, size_t *out_buff_len);
FileStatus write_poem(FILE* file_ptr, char *index[], uint16_t lines, const char *header);
FileStatus write_orig_poem(FILE *file_ptr, const char *file_content, uint16_t lines, const char *header);

