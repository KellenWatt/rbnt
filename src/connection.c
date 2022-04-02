#include <ruby.h>

#include "rbnt/connection.h"
#include "rbnt/util/reader.h"
#include "rbnt/util/writer.h"

static VALUE cConnection;

static VALUE cTCPSocket;

#define iv_connection "@connection"
#define iv_host "@host"
#define iv_port "@port"

// default: localhost:1735
// ip/hostname, port = 1735
VALUE Connection_initialize(int argc, VALUE* argv, VALUE self) {
    VALUE host;
    VALUE port;
    rb_scan_args(argc, argv, "02", &host, &port);
    if(NIL_P(host)) {
        host = rb_str_new_cstr("localhost");
    } 
    if(NIL_P(port)){
        port = INT2NUM(1735);
    }
    rb_iv_set(self, iv_host, host);
    rb_iv_set(self, iv_port, port);
    VALUE args[2] = {host, port};
    rb_iv_set(self, iv_connection, rb_class_new_instance(2, args, cTCPSocket));
    return self;
}

VALUE Connection_get_host(VALUE self) {
    return rb_iv_get(self, iv_host);
}

VALUE Connection_get_port(VALUE self) {
    return rb_iv_get(self, iv_port);
}

VALUE Connection_get_connection(VALUE self) {
    return rb_iv_get(self, iv_connection);
}

VALUE Connection_read_bytes(VALUE self, VALUE count) {
    VALUE connection = Connection_get_connection(self);
    return read_bytes(connection, NUM2UINT(count));
}

VALUE Connection_read_boolean(VALUE self) {
    VALUE connection = Connection_get_connection(self);
    return read_bool(connection);
}

VALUE Connection_read_number(VALUE self) {
    VALUE connection = Connection_get_connection(self);
    return read_double(connection);
}

VALUE Connection_read_string(VALUE self) {
    VALUE connection = Connection_get_connection(self);
    return read_string(connection);
}

VALUE Connection_read_raw(VALUE self) {
    VALUE connection = Connection_get_connection(self);
    return read_string(connection);
}

VALUE Connection_read_boolean_array(VALUE self) {
    VALUE connection = Connection_get_connection(self);
    return read_bool_array(connection);
}

VALUE Connection_read_number_array(VALUE self) {
    VALUE connection = Connection_get_connection(self);
    return read_double_array(connection);
}

VALUE Connection_read_string_array(VALUE self) {
    VALUE connection = Connection_get_connection(self);
    return read_string_array(connection);
}

VALUE Connection_write_bytes(VALUE self, VALUE bytes) {
    VALUE connection = Connection_get_connection(self);
    return write_bytes(connection, bytes);
}

VALUE Connection_write_boolean(VALUE self, VALUE boolean) {
    VALUE connection = Connection_get_connection(self);
    return write_bool(connection, boolean);
}

VALUE Connection_write_number(VALUE self, VALUE num) {
    VALUE connection = Connection_get_connection(self);
    return write_double(connection, num);
}

VALUE Connection_write_string(VALUE self, VALUE str) {
    VALUE connection = Connection_get_connection(self);
    return write_string(connection, str);
}

VALUE Connection_write_raw(VALUE self, VALUE str) {
    VALUE connection = Connection_get_connection(self);
    return write_string(connection, str);
}

VALUE Connection_write_boolean_array(VALUE self, VALUE bools) {
    VALUE connection = Connection_get_connection(self);
    return write_bool_array(connection, bools);
}

VALUE Connection_write_number_array(VALUE self, VALUE nums) {
    VALUE connection = Connection_get_connection(self);
    return write_double_array(connection, nums);
}

VALUE Connection_write_string_array(VALUE self, VALUE strs) {
    VALUE connection = Connection_get_connection(self);
    return write_string_array(connection, strs);
}

// While potentially useful in the gneral case, it's use is unclear here
VALUE Connection_write_general(VALUE self, VALUE val) {
    if(RB_INTEGER_TYPE_P(val)) {
        Connection_write_number(self, val);
    }
    switch(TYPE(val)) {
    case T_FLOAT:
        Connection_write_number(self, val);
        break;
    case T_TRUE:
    case T_FALSE:
        Connection_write_boolean(self, val);
        break;
    case T_STRING:
        Connection_write_string(self, val);
        break;
    case T_ARRAY:;
        VALUE elem = rb_ary_entry(val, 0);
        if(RB_INTEGER_TYPE_P(val)) {
            Connection_write_number_array(self, val);
        }
        switch(TYPE(elem)) {
        case T_FLOAT:
            Connection_write_number_array(self, val);
            break;
        case T_TRUE:
        case T_FALSE:
            Connection_write_boolean_array(self, val);
            break;
        case T_STRING:
            Connection_write_string_array(self, val);
            break;
        default:
            rb_raise(rb_eArgError, "Arrays can only contain bools, numbers, or strings");
        }
        break;
    default:;
        VALUE r = rb_funcall(val, rb_intern("to_s"), 0);
        Connection_write_raw(self, r);
    }
}

void Init_connection() {
    rb_require("socket");
    cTCPSocket = rb_const_get(rb_cObject, rb_intern("TCPSocket"));

    VALUE cNetworkTable = rb_define_class("NetworkTable", rb_cObject);
    cConnection = rb_define_class_under(cNetworkTable, "Connection", rb_cObject);

    rb_define_method(cConnection, "initialize", Connection_initialize, -1);
    rb_define_method(cConnection, "host", Connection_get_host, 0);
    rb_define_method(cConnection, "port", Connection_get_port, 0);
    rb_define_method(cConnection, "connection", Connection_get_connection, 0);

    rb_define_method(cConnection, "read_boolean", Connection_read_boolean, 0);
    rb_define_method(cConnection, "read_number", Connection_read_number, 0);
    rb_define_method(cConnection, "read_string", Connection_read_string, 0);
    rb_define_method(cConnection, "read_raw", Connection_read_raw, 0);
    rb_define_method(cConnection, "read_boolean_array", Connection_read_boolean_array, 0);
    rb_define_method(cConnection, "read_number_array", Connection_read_number_array, 0);
    rb_define_method(cConnection, "read_string_array", Connection_read_string_array, 0);

    rb_define_method(cConnection, "write_boolean", Connection_write_boolean, 1);
    rb_define_method(cConnection, "write_number", Connection_write_number, 1);
    rb_define_method(cConnection, "write_string", Connection_write_string, 1);
    rb_define_method(cConnection, "write_raw", Connection_write_raw, 1);
    rb_define_method(cConnection, "write_boolean_array", Connection_write_boolean_array, 1);
    rb_define_method(cConnection, "write_number_array", Connection_write_number_array, 1);
    rb_define_method(cConnection, "write_string_array", Connection_write_string_array, 1);

    /* rb_define_method(cConnection, "write", Connection_write_general, 1); */
    /* rb_define_alias(cConnection, "<<", "write"); */
}
