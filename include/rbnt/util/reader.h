#pragma once

#include <ruby.h>
#include <stdint.h>

#include "rbnt/util/leb128.h"

VALUE read_bytes(VALUE socket, uint32_t bytes) {
    return rb_funcall(socket, rb_intern("recv"), 1, INT2NUM(bytes));
}

uint32_t read_c_uleb(VALUE socket) {
    uint8_t bytes[5] = {0};
    int i = 0;
    VALUE byte;
    do {
        byte = read_bytes(socket, 1);
        bytes[i] = StringValuePtr(byte)[0];
    } while(bytes[i] & 0x80);
    return decode_uleb128(bytes);
}

VALUE read_uleb(VALUE socket) {
    return INT2NUM(read_c_uleb(socket));
}

bool read_c_bool(VALUE socket) {
    VALUE ret = read_bytes(socket, 1);
    char* bytes = StringValuePtr(ret);
    return bytes[0] == 0x00;
}

VALUE read_bool(VALUE socket) {
    return read_c_bool(socket) ? Qtrue : Qfalse;    
}

double read_c_double(VALUE socket) {
    VALUE ret = read_bytes(socket, 8);
    char* bytes = StringValuePtr(ret);
    return *(double*)(bytes);
}

VALUE read_double(VALUE socket) {
    return rb_float_new(read_c_double(socket));
}

VALUE read_string(VALUE socket) {
    uint32_t length = read_c_uleb(socket);
    return read_bytes(socket, length);
}

VALUE read_bool_array(VALUE socket) {
    VALUE size_byte = read_bytes(socket, 1);
    uint8_t size = StringValuePtr(size_byte)[0];
    VALUE out = rb_ary_new2(size);
    VALUE bool_bytes = read_bytes(socket, size);
    char* bytes = StringValuePtr(bool_bytes);
    VALUE byte;

    for(int i=0; i<size; i++) {
        byte = (bytes[i] ? Qtrue : Qfalse);
        rb_ary_push(out, byte);
    }
    return out;
}

VALUE read_double_array(VALUE socket) {
    VALUE size_byte = read_bytes(socket, 1);
    uint8_t size = StringValuePtr(size_byte)[0];

    VALUE out = rb_ary_new2(size);
    VALUE dbl_bytes = read_bytes(socket, 8 * size);
    char* bytes = StringValuePtr(dbl_bytes);
    double* nums = (double*)bytes;

    for(int i=0; i<size; i++) {
        rb_ary_push(out, rb_float_new(nums[i]));
    }
    return out;
}

VALUE read_string_array(VALUE socket) {
    VALUE size_byte = read_bytes(socket, 1);
    uint8_t size = StringValuePtr(size_byte)[0];
    VALUE out = rb_ary_new2 (size);
    VALUE str;
    for(int i=0; i<size; i++) {
        str = read_string(socket);
        rb_ary_push(out, str);
    }
    return out;
}

// RPC reading is delegated to its own file.
