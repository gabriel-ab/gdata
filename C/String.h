/* ====  Made by Gabriel-AB  ====
 * Dynamic c String Library v0.1
 * 
 * https://github.com/Gabriel-AB
 */
#ifndef STRING_H
#define STRING_H
#include <string.h>
#include <stdlib.h>

typedef struct string {
    char *data;
    size_t size;
    size_t alloc_size;
}*String;

void String_copy(String str, const char* text) {
    int len = strlen(text);
    str->size = len;
    _Bool should_free_memory = len < str->alloc_size*0.2;

    if (len < str->alloc_size && !should_free_memory) {
        strcpy(str->data, text);

    } else {
        str->alloc_size = len +1;
        char *new_data = calloc(str->alloc_size, sizeof(char));
        strcpy(new_data, text);

        free(str->data);
        str->data = new_data;
    }
}

void String_append(String str, const char *text) {
    int len = strlen(text);
    if (str->alloc_size > str->size + len) {
        strcat(str->data, text);

    } else {
        str->alloc_size = str->size + len +1;

        char *new_data = calloc(str->alloc_size, sizeof(char));
        strcpy(new_data, str->data);
        strcat(new_data, text);

        free(str->data);
        str->data = new_data;
    }
}

String StringCreate(const char *string) {
    String str = calloc(1, sizeof(struct string));
    str->data = malloc(1);
    String_copy(str, string);
    return str;
}

void StringDelete(String str) {
    free(str->data);
    free(str);
}

#endif