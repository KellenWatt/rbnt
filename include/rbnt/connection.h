#pragma once

#include <ruby.h>

VALUE Connection_get_host(VALUE self);
VALUE Connection_get_port(VALUE self);
VALUE Connection_get_connection(VALUE self);

VALUE Connection_read_bytes(VALUE self, VALUE count);
VALUE Connection_read_boolean(VALUE self);
VALUE Connection_read_number(VALUE self);
VALUE Connection_read_string(VALUE self);
VALUE Connection_read_raw(VALUE self);
VALUE Connection_read_boolean_array(VALUE self);
VALUE Connection_read_number_array(VALUE self);
VALUE Connection_read_string_array(VALUE self);

VALUE Connection_write_bytes(VALUE self, VALUE bytes);
VALUE Connection_write_boolean(VALUE self, VALUE boolean);
VALUE Connection_write_number(VALUE self, VALUE num);
VALUE Connection_write_string(VALUE self, VALUE str);
VALUE Connection_write_raw(VALUE self, VALUE str);
VALUE Connection_write_boolean_array(VALUE self, VALUE bools);
VALUE Connection_write_number_array(VALUE self, VALUE nums);
VALUE Connection_write_string_array(VALUE self, VALUE strs);

void Init_connection();
