#include "file_utils.h"

#include <stdlib.h>
#include <assert.h>
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

FileStatus get_file_size(const char *filename, size_t *out_file_size) {
    assert(filename != NULL);
    assert(out_file_size != NULL);

    struct stat stat_buff;

    if (lstat(filename, &stat_buff) == -1) {
        return FS_STAT_ERROR;
    }

    *out_file_size = stat_buff.st_size;

    return FS_OK;
}


FileStatus load_file_alloc(const char *filename, char **out_buff, size_t *out_buff_len) {
    assert(filename != NULL);
    assert(out_buff != NULL);
    assert(out_buff_len != NULL);

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

    *(out_buff) = file_buff;
    *(out_buff_len) = file_buff_len;

    return FS_OK;
}


FileStatus write_poem(FILE* file_ptr, char *index[], uint16_t lines, const char *header) {
    assert(file_ptr != NULL);
    assert(header != NULL);
    assert(index != NULL);

    if (fprintf(file_ptr, "%s\n================================\n\n", header) < 0)
        return FS_WRITE_ERROR;

    int curr_ind = 0;

    while (curr_ind < lines) {
        if (fprintf(file_ptr, "%s\n", index[curr_ind]) < 0)
                return FS_WRITE_ERROR;

        curr_ind++;
    }

    if (fputs("\n================================\n", file_ptr) == EOF)
        return FS_WRITE_ERROR;

    return FS_OK;
}


FileStatus write_orig_poem(FILE *file_ptr, const char *file_content, uint16_t lines, const char *header) {
    assert(file_ptr != NULL);
    assert(file_content != NULL);
    assert(header != NULL);
    assert(lines > 0);

    if (fprintf(file_ptr, "%s\n================================\n\n", header) < 0)
        return FS_WRITE_ERROR;

    while (lines--) {
        int written_size = fprintf(file_ptr, "%s\n", file_content);
        if (written_size < 0)
            return FS_WRITE_ERROR;

        file_content += written_size;
    }

    if (fputs("\n================================\n\n", file_ptr) == EOF)
        return FS_WRITE_ERROR;

    return FS_OK;
}
