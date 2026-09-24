#include "sort_utils.h"
#include "file_utils.h" 

#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>


int strcmp_letters_only(const void *ptr_a, const void *ptr_b) {
    assert(ptr_a != NULL);
    assert(ptr_b != NULL);

    char *s1 = ((String *)ptr_a)->addr;
    char *s2 = ((String *)ptr_b)->addr;

    size_t c = 0;

    while (true) {
        if (s1[c] == '\0' && s2[c] != '\0') {
            return 1;
        } else if (s1[c] != '\0' && s2[c] == '\0') {
            return -1;
        } else if (s1[c] == '\0' && s2[c] == '\0') {
            return 0;
        } else if (!isalpha(s1[c])) {
            s1++;
            continue;
        } else if (!isalpha(s2[c])) {
            s2++;
            continue;
        }

        if (s1[c] != s2[c])
            return (s1[c] - s2[c]);

        c++;
    }
}


int strcmp_letters_only_rev(const void *ptr_a, const void *ptr_b) {
    assert(ptr_a != NULL);
    assert(ptr_b != NULL);

    char *s1 = ((String *)ptr_a)->addr;
    char *s2 = ((String *)ptr_b)->addr;

    int64_t s1_ind = ((String *)ptr_a)->len - 1;
    int64_t s2_ind = ((String *)ptr_b)->len - 1;


    while (s1_ind >= 0 && s2_ind >= 0) {
        if (!isalpha(s1[s1_ind])) {
            s1_ind--;
            continue;
        } else if (!isalpha(s2[s2_ind])) {
            s2_ind--;
            continue;
        }

        if (s1[s1_ind] != s2[s2_ind])
            return (s1[s1_ind] - s2[s2_ind]);

        s1_ind--;
        s2_ind--;
    }

    if (s1_ind == -1 && s2_ind == -1) {
        return 0;
    } else if (s1_ind == -1) {
        for (int64_t i = s2_ind; i >= 0; s2_ind--) {
                if (isalpha(s2_ind))
                    return -1;
        }

        return 0;
    } else {
        for (int64_t i = s1_ind; i >= 0; s1_ind--) {
                if (isalpha(s1_ind))
                    return 1;
        }

        return 0;
    }
}


static void swap_elements(uint8_t *a, uint8_t *b, size_t size) {
    assert(a != NULL);
    assert(b != NULL);
    assert(size > 0);

    uint8_t tmp = 0;

    for (size_t i = 0; i < size; i++) {
        tmp = *(uint8_t *)(a + i);
        *(uint8_t *)(a + i) = *(uint8_t *)(b + i);
        *(uint8_t *)(b + i) = tmp;
    }
}


void myqsort(void *data_void, size_t data_len, size_t type_size,
             int (*compare)(const void*, const void*)) {
    assert(data_void != NULL);
    assert(compare != NULL);
    assert(type_size > 0);

    uint8_t *data = (uint8_t *)data_void;

    if (data_len < 2)
        return;

    if (data_len == 2) {
        if ((*compare)(data, data + type_size) > 0)
            swap_elements(data, data + type_size, type_size);
        return;
    }

    uint8_t separator_element[type_size] = {};
    memcpy(separator_element, data + ((data_len - 1) / 2) * type_size, type_size); 

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

