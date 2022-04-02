#include <ruby.h>
#include <stdint.h>

#include "rbnt/message.h"
#include "rbnt/util/reader.h"
#include "rbnt/util/type_wrap.h"

#include <stdio.h>

VALUE read_message_type(VALUE socket) {
    read_bytes(socket, 1);
}

void validate_type(VALUE type, uint32_t expected) {
    if(NUM2INT(type) != expected) {
        rb_raise(rb_eRuntimeError);
    }
}

VALUE Message_m_make_keep_alive(VALUE self, VALUE connection) {
    return rb_str_new(char[1]{MESSAGE_KEEP_ALIVE}, 1);
}

VALUE Message_m_read_keep_alive(VALUE self, VALUE connection) {
    VALUE type = read_message_type(connection);
    validate_type(type, MESSAGE_KEEP_ALIVE);
    return Qtrue;
}

VALUE Message_m_make_client_hello(VALUE self, VALUE name) {
    VALUE out = rb_str_new(char[1]{MESSAGE_CLIENT_HELLO}, 1);
    name = wrap_r_string(name);
    return rb_str_append(out, name);
}

VALUE Message_m_read_client_hello(VALUE self, VALUE connection) {
    VALUE type = read_message_type(connection);
    validate_type(type, MESSAGE_CLIENT_HELLO);
    printf("Warning: Client Hello (0x01) from server not supported\n");
    return Qtrue;
}

VALUE Message_m_read_protocol_version_unsupported(VALUE self, VALUE connection) {
    VALUE type = read_message_type(connection);
    validate_type(type, MESSAGE_PROTOCOL_VERSION_UNSUPPORTED);
    VALUE bytes = read_bytes(connection, 2);
    uint16_t version = *((uint16_t*)StringValuePtr(bytes));
    return rb_ary_new_from_args(2, Qtrue, INT2NUM(version));
}

VALUE Message_m_read_server_hello_complete(VALUE self, VALUE connection) {
    VALUE type = read_message_type(connection);
    validate_type(type, MESSAGE_SERVER_HELLO_COMPLETE);
    return Qtrue;
}

VALUE Message_m_read_server_hello(VALUE self, VALUE connection) {
    VALUE type = read_message_type(connection);
    validate_type(type, MESSAGE_SERVER_HELLO);
    VALUE flags = read_bytes(connection, 1);
    uint8_t c_flags = *((uint8_t*)StringValuePtr(flags));
    VALUE name = read_string(connection);

    return rb_ary_new_from_args(3, Qtrue, INT2NUM(c_flags), name);
}

VALUE Message_m_make_client_hello_complete(VALUE self) {
    return rb_str_new(char[1]{MESSAGE_CLIENT_HELLO_COMPLETE}, 1);
}

VALUE Message_m_read_client_hello_complete(VALUE self, VALUE connection) {
    VALUE type = read_message_type(connection);
    validate_type(type, MESSAGE_CLIENT_HELLO_COMPLETE);
    printf("Warning: Client Hello Complete (0x05) from server not supported\n");
    return Qtrue;
}

VALUE Message_m_make_entry_assignemnt(VALUE self, VALUE name, VALUE type, VALUE id, VALUE sn, VALUE flags, VALUE val) {
    VALUE out = rb_str_new(char[1]{MESSAGE_ENTRY_ASSIGNMENT}, 1);
    rb_sprintf("%"PRIsVALUE"%c%c%c%c%c%c", 
               name,
               )

    /* write_message_type(connection, MESSAGE_ENTRY_ASSIGNMENT); */
    /* write_string(connection, name); */
    /*  */
    /* write_bytes(connection, ) */
}


