#pragma once

#include <ruby.h>
#include <stdint.h>

#include "rbnt/util/leb128.h"

VALUE write_bytes(VALUE socket, VALUE bytes) {
    return rb_funcall(socket, rb_intern("send"), 1, bytes);
}

VALUE write_c_bytes(VALUE socket, uint8_t* bytes, long count) {
    VALUE msg = rb_utf8_str_new(bytes, count);
    return NUM2UINT(rb_funcall(socket, rb_intern("send"), 1, msg));
}

uint32_t write_c_uleb(VALUE socket, uint32_t num) {
    uint8_t buffer[5];
    uint8_t count = encode_uleb128(num, buffer);
    return write_c_bytes(socket, buffer, count);
}

VALUE write_uleb(VALUE socket, VALUE num) {
    return UINT2NUM(write_c_uleb(socket, NUM2UINT(num)));
}

// sends a literal 1 or 0, not 0x01 or 0x00
uint32_t write_c_bool(VALUE socket, bool b) {
    uint8_t bytes[1] = {b != 0};
    return write_c_bytes(socket, bytes, 1);
}

VALUE write_bool(VALUE socket, VALUE boolean) {
    return UINT2NUM(write_c_bool(socket, boolean != Qfalse && boolean != Qnil));
}

uint32_t write_c_double(VALUE socket, double num) {
    return write_c_bytes(socket, (uint8_t*)(&(num)), 8);
}

VALUE write_double(VALUE socket, VALUE num) {
    return UINT2NUM(write_c_double(socket, NUM2DBL(num)));
}

uint32_t write_c_string(VALUE socket, const char* msg, uint32_t size) {
    uint32_t size_written = write_c_uleb(socket, size);
    uint32_t msg_size = write_c_bytes(socket, (char*)msg, size);
    return size_written + msg_size;
}

VALUE write_string(VALUE socket, VALUE msg) {
    // char* message = StringValuePtr(msg);
    // long size = RSTRING_LEN(msg);
    return UINT2NUM(write_bytes(socket, msg));
}

VALUE write_bool_array(VALUE socket, VALUE bools) {
    int count = RARRAY_LEN(bools);
    uint32_t size = write_c_uleb(socket, count);
    for(int i=0; i<count; i++) {
        write_bool(socket, rb_ary_entry(bools, i));
    }
    return INT2NUM(size + count);
} 

VALUE write_double_array(VALUE socket, VALUE nums) {
    int count = RARRAY_LEN(nums);
    uint32_t size = write_c_uleb(socket, count);
    for(int i=0; i<count; i++) {
        write_double(socket, rb_ary_entry(nums, i));
    }
    return INT2NUM(size + count * 8);
}

VALUE write_string_array(VALUE socket, VALUE strs) {
    int count = RARRAY_LEN(strs);
    uint32_t size = write_c_uleb(socket, count);
    for(int i=0; i<count; i++) {
        size += NUM2UINT(write_string(socket, rb_ary_entry(strs, i)));
    }
    return INT2NUM(size);
}
