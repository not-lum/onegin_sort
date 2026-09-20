#include "sort_utils.h"

#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>


int strcmp_letters_only(const void *ptr_a, const void *ptr_b) {
    assert(ptr_a != NULL);
    assert(ptr_b != NULL);

    char *a = *(char **)ptr_a;
    char *b = *(char **)ptr_b;

    size_t c = 0;

    while (true) {
        if (a[c] == '\0' && b[c] != '\0') {
            return 1;
        } else if (a[c] != '\0' && b[c] == '\0') {
            return -1;
        } else if (a[c] == '\0' && b[c] == '\0') {
            return 0;
        } else if (!isalpha(a[c])) {
            a++;
            continue;
        } else if (!isalpha(b[c])) {
            b++;
            continue;
        }

        if (a[c] != b[c])
            return (a[c] - b[c]);

        c++;
    }
}


int strcmp_letters_only_rev(const void *ptr_a, const void *ptr_b) {
    assert(ptr_a != NULL);
    assert(ptr_b != NULL);

    char *a = *(char **)ptr_a;
    char *b = *(char **)ptr_b;

    int64_t a_ind = strlen(a) - 1;
    int64_t b_ind = strlen(b) - 1;

    while (a_ind >= 0 && b_ind >= 0) {
        if (!isalpha(a[a_ind])) {
            a_ind--;
            continue;
        } else if (!isalpha(b[b_ind])) {
            b_ind--;
            continue;
        }

        if (a[a_ind] != b[b_ind])
            return (a[a_ind] - b[b_ind]);

        a_ind--;
        b_ind--;
    }

    if (a_ind == -1 && b_ind == -1) {
        return 0;
    } else if (a_ind == -1) {
        for (int64_t i = b_ind; i >= 0; b_ind--) {
                if (isalpha(b_ind))
                    return -1;
        }

        return 0;
    } else {
        for (int64_t i = a_ind; i >= 0; a_ind--) {
                if (isalpha(a_ind))
                    return 1;
        }

        return 0;
    }
}


static void copy_element(uint8_t *src, uint8_t *dst, size_t size) { // todo: replace with memcpy
    assert(src != NULL);
    assert(dst != NULL);
    assert(size > 0);
    assert(size <= 1024);

    for (size_t i = 0; i < size; i++) {
        *(dst + i) = *(src + i);
    }
}


static void swap_elements(uint8_t *a, uint8_t *b, size_t size) {
    assert(a != NULL);
    assert(b != NULL);
    assert(size > 0);
    assert(size <= 1024);

    uint8_t tmp[size] = {};

    copy_element(a, tmp, size); // tmp = a
    copy_element(b, a, size); // a = b
    copy_element(tmp, b, size); // b = tmp
}


void myqsort(void *data_void, size_t data_len, size_t type_size,
             int (*compare)(const void*, const void*)) {
    assert(data_void != NULL);
    assert(compare != NULL);
    assert(type_size > 0);
    assert(type_size <= 1024);

    uint8_t *data = (uint8_t *)data_void;

    if (data_len < 2)
        return;

    if (data_len == 2) {
        if ((*compare)(data, data + type_size) > 0)
            swap_elements(data, data + type_size, type_size);
        return;
    }

    uint8_t separator_element[type_size] = {};
    copy_element(data + ((data_len - 1) / 2) * type_size, separator_element, type_size);

    size_t left_idx = 0;
    size_t right_idx = data_len - 1;
    bool end_loop = false;

    while (left_idx != right_idx) {
        if ((*compare)(separator_element, data + left_idx * type_size) <= 0) { // data[left_idx] >= separator_element
            while (true) {
                if (left_idx == right_idx) {
                    end_loop = true;
                    break;
                }

                if ((*compare)(separator_element, data + right_idx * type_size) >= 0) { // data[right_idx] <= separator_element
                    break;
                }

                right_idx--;
            }

            if (end_loop)
                break;

            swap_elements(data + left_idx * type_size, data + right_idx * type_size, type_size);
        }

        left_idx++;
    }

    if (left_idx != 0) {
        myqsort(data_void, left_idx, type_size, compare);
    }
    if (right_idx != data_len - 1 && right_idx != 0) {
        myqsort(data + right_idx * type_size, data_len - right_idx, type_size, compare);
    }
}

