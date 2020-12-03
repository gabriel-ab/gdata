#include "String.h"
#include <string.h>

void String_rename(String str, const char* text) {
    str->size = strlen(text);
    
    _Bool less_mem = str->size < str->alloc_size*0.2;
    _Bool more_mem = str->size > str->alloc_size;

    if (more_mem || less_mem) {
        str->alloc_size = str->size +1;
        str->data = realloc(str->data, str->alloc_size);        
    }
    strcpy(str->data, text);
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
    String_rename(str, string);
    return str;
}

void StringDelete(String *str) {
    free((*str)->data);
    free(*str);
}

Array String_split(String string, const char *pattern) {
    char * token = strtok(string->data, pattern);
    String results[string->size];
    size_t count = 0;

    while (token != NULL) {
        results[count++] = StringCreate(token);
        token = strtok(NULL, pattern);
    }

    Array array = ArrayCreateEmpty(String, count);
    memcpy(array->data, results, sizeof(String)*count);
    array->valueDestructor = (void*)StringDelete;
    return array;
}

String String_copy(String string) {
    String str = calloc(1, sizeof(struct string));
    str->data = malloc(string->alloc_size);
    strcpy(str->data, string->data);
    return str;
}

String String_join(String a, String b) {
    String result = String_copy(a);
    String_append(result, b->data);
    return result;
}

_Bool String_equals(String a, String b) {
    return !strcmp(a->data, b->data);
}