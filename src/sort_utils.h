#pragma once

#include <stdlib.h>

int strcmp_letters_only(const void *ptr_a, const void *ptr_b);
int strcmp_letters_only_rev(const void *ptr_a, const void *ptr_b);
void myqsort(void *data, size_t data_len, size_t type_size, int (*compare)(const void*, const void*));

