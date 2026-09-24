#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct String {
    size_t len;
    char *addr;
} String;

typedef struct IndexedFile {
    char *content;
    size_t content_len;
    String *index;
    size_t lines_numb; 
} IndexedFile;

typedef enum {
    FS_OK,
    FS_OPEN_ERROR,
    FS_READ_ERROR,
    FS_ALLOC_ERROR,
    FS_CLOSE_ERROR,
    FS_WRITE_ERROR,
    FS_STAT_ERROR,
} FileStatus;

const char *fs_error_str(FileStatus error);
FileStatus create_indexed_file(const char *filename, IndexedFile *out_idx_file);
void destroy_indexed_file(IndexedFile *out_idx_file);
FileStatus write_poem(FILE *file_ptr, const IndexedFile *idx_file, const char *header);
FileStatus write_orig_poem(FILE *file_ptr, const IndexedFile *idx_file, const char *header);
