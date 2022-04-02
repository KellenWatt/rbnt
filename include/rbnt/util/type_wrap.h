#pragma once
#include <ruby.h>
#include <stdint.h>
#include <stdio.h>

#include "leb128.h"
#include "rbnt/entry/types.h"

VALUE wrap_bytes(uint8_t* bytes, long count) {
    return rb_str_new(bytes, count)
}

VALUE wrap_bool(bool b) {
    return rb_str_new(char[1]{b}, 1);
}

bool extract_bool(VALUE str) {
    return StringValuePtr(str)[0];
}

VALUE wrap_double(double d) {
    uint64_t p = *((uint64_t*)(&d));
    return ULL2NUM(p);
}

double extract_double(VALUE str) {
    return *((double*)StringValuePtr(str));
}

VALUE wrap_c_string(char* s) {
    uint8_t buffer[5] = {0};
    uint8_t count = encode_uleb128(strlen(s), buffer);
    VALUE str = rb_str_new(buffer, count);
    return rb_str_cat2(str, s);
}

VALUE wrap_r_string(VALUE s) {
    uint8_t buffer[5] = {0};
    uint8_t count = encode_uleb128(RSTRING_LEN(s), buffer);
    VALUE str = rb_str_new(buffer, count);
    return rb_str_append(str, s);
}

VALUE extract_string(VALUE str) {
    char* s = StringValuePtr(str);
    long max_size = RSTRING_LEN(str);
    int offset = 0;
    while(offset < max_size && s[0] & 0x80) {
        offset++;
    }
    return rb_string_new(s + offset, decode_uleb128(s));
}


VALUE wrap_value(VALUE type, VALUE val) {
    switch(type) {
    case RB_BOOLEAN_TYPE:
        return rb_str_new(char[1]{val == Qfalse ? false : true}, 1);
    case RB_DOUBLE_TYPE:
        double d = NUM2DBL(val);
        return rb_str_new((char*)(&d), 8);
    case RB_STRING_TYPE:
    case RB_RAW_TYPE:
        uint8_t buffer[5] = {0};
        uint8_t count = encode_uleb128(RSTRING_LEN(val), buffer);
        VALUE tmp = rb_str_new(buffer, count);
        return rb_str_append(tmp, val);
    case RB_BOOLEAN_ARRAY_TYPE:
        
        break;
    case RB_DOUBLE_ARRAY_TYPE:
        
        break;
    case RB_STRING_ARRAY_TYPE:
        
        break;

    }
}
