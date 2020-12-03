/* 
 * Dynamic Sized String Library v0.2
 * 
 * @author Gabriel-AB
 * https://github.com/Gabriel-AB
 */

#pragma once

#include <stdlib.h>
#include "Array.h"

typedef struct string {
    char *data;
    size_t size;
    size_t alloc_size;
}*String;

void String_rename(String str, const char* text);

void String_append(String str, const char *text);

String StringCreate(const char *string);

void StringDelete(String *str);

Array String_split(String string, const char *pattern);

String String_copy(String string);

String String_join(String a, String b);

_Bool String_equals(String a, String b);