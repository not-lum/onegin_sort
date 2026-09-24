#include "file_utils.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>


const char *fs_error_str(FileStatus error) {
    switch (error) {
        case FS_OK:
            return "ok";
        case FS_ALLOC_ERROR:
            return "cannot allocate memory";
        case FS_OPEN_ERROR:
            return "cannot open file";
        case FS_STAT_ERROR:
            return "cannot obtain file info";
        case FS_READ_ERROR:
            return "cannot read file";
        case FS_WRITE_ERROR:
            return "cannot write to a file";
        case FS_CLOSE_ERROR:
            return "cannot close file";
        default:
            return "unknown error";
    }
}

static FileStatus get_file_size(const char *filename, size_t *out_file_size) {
    assert(filename != NULL);
    assert(out_file_size != NULL);

    struct stat stat_buff = {};

    if (lstat(filename, &stat_buff) == -1) {
        return FS_STAT_ERROR;
    }

    *out_file_size = stat_buff.st_size;

    return FS_OK;
}

static size_t str_count_char(char *str, char ch) {
    size_t res = 0;

    while (*str != '\0') {
        if (*str == ch)
            res++;

        str++;
    }

    return res;
}

static FileStatus load_file_alloc(const char *filename, IndexedFile *out_idx_file) {
    assert(filename != NULL);
    assert(out_idx_file != NULL);

    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL)
        return FS_OPEN_ERROR;

    size_t file_size = 0;

    FileStatus status_gfs = get_file_size(filename, &file_size);
    if (status_gfs != FS_OK) {
        fclose(file_ptr);
        return status_gfs;
    }

    char *file_buff = calloc(file_size + 1, sizeof(char));
    if (file_buff == NULL) {
        fclose(file_ptr);
        return FS_ALLOC_ERROR;
    }

    size_t file_buff_len = fread(file_buff, sizeof(char), file_size, file_ptr);
    if (ferror(file_ptr)) {
        free(file_buff);
        fclose(file_ptr);
        return FS_READ_ERROR;
    }

    if (fclose(file_ptr) == EOF) {
        free(file_buff);
        return FS_CLOSE_ERROR;
    }

    file_buff[file_buff_len] = '\0';

    out_idx_file->content = file_buff;
    out_idx_file->content_len = file_buff_len;

    return FS_OK;
}


static FileStatus index_lines(IndexedFile *idx_file) {
    assert(idx_file != NULL);

    idx_file->index = calloc(str_count_char(idx_file->content, '\n'), sizeof(idx_file->index[0]));
    if (idx_file->index == NULL)
        return FS_ALLOC_ERROR;

    String *index = idx_file->index;

    index[0].addr = idx_file->content;

    idx_file->lines_numb = 1;

    while (true) {
        char *addr = strchr(index[idx_file->lines_numb - 1].addr, '\n'); // load_file_alloc adds '\0' at the end of written data, so strchr won't search for '\n' infinitely
        if (addr == NULL)
            break;

        if (addr == idx_file->content + idx_file->content_len - 1) {
            index[idx_file->lines_numb - 1].len = addr - index[idx_file->lines_numb - 1].addr;
            *addr = '\0';
            break;
        }

        *addr = '\0';
        index[idx_file->lines_numb - 1].len = addr - index[idx_file->lines_numb - 1].addr;
        index[idx_file->lines_numb].addr = addr + 1;
        
        idx_file->lines_numb++;
    }

    return FS_OK;
}


FileStatus create_indexed_file(const char *filename, IndexedFile *out_idx_file) {
    assert(filename != NULL);
    assert(out_idx_file != NULL);

    FileStatus status = load_file_alloc(filename, out_idx_file);

    if (status != FS_OK) {
        return status; 
    }

    status = index_lines(out_idx_file);

    return status;
}

void destroy_indexed_file(IndexedFile *out_idx_file) {
    if (out_idx_file->content != NULL)
        free(out_idx_file->content);

    if (out_idx_file->index != NULL)
        free(out_idx_file->index);
}

FileStatus write_poem(FILE *file_ptr, const IndexedFile *idx_file, const char *header) {
    assert(file_ptr != NULL);
    assert(idx_file != NULL);
    assert(header != NULL);

    if (fprintf(file_ptr, "%s\n================================\n\n", header) < 0)
        return FS_WRITE_ERROR;

    size_t curr_ind = 0;

    while (curr_ind < idx_file->lines_numb) {
        if (fprintf(file_ptr, "%s\n", idx_file->index[curr_ind].addr) < 0)
                return FS_WRITE_ERROR;

        curr_ind++;
    }

    if (fputs("\n================================\n", file_ptr) == EOF)
        return FS_WRITE_ERROR;

    return FS_OK;
}


FileStatus write_orig_poem(FILE *file_ptr, const IndexedFile *idx_file, const char *header) {
    assert(file_ptr != NULL);
    assert(idx_file != NULL);
    assert(header != NULL);

    if (fprintf(file_ptr, "%s\n================================\n\n", header) < 0)
        return FS_WRITE_ERROR;

    size_t lines_numb = idx_file->lines_numb;
    char *line_ptr = idx_file->content;

    while (lines_numb--) {
        int written_size = fprintf(file_ptr, "%s\n", line_ptr);

        if (written_size < 0)
            return FS_WRITE_ERROR;

        line_ptr += written_size;
    }

    if (fputs("\n================================\n\n", file_ptr) == EOF)
        return FS_WRITE_ERROR;

    return FS_OK;
}

