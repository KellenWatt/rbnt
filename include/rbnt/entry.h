#pragma once
#include <ruby.h>
#include <stdint.h>

#include "util/sequencenumber.h"
#include "entry/types.h"
#include "entry/flags.h"

struct entry_data {
    VALUE data;
    nt_type type;
    VALUE last_updated;
    uint16_t sequence_number;
    nt_flags flags;
    VALUE update_callback;
};

size_t entry_size(void* data) {
    return sizeof(entry_data);
}

void entry_mark(void* data) {
    rb_gc_mark(((struct entry_data*)data)->data);
    rb_gc_mark(((struct entry_data*)data)->last_updated);
    rb_gc_mark(((struct entry_data*)data)->update_callback);
}

const rb_data_type_t entry_type = {
    .wrap_struct_name = "network_tables_entry",
    .function = {
        .dmark = entry_mark,
        .dfree = RUBY_DEFAULT_FREE,
        .dsize = entry_size,
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

VALUE entry_alloc(VALUE self) {
    struct entry_data* data;

    return TypedData_Make_Struct(self, struct entry_data*, &entry_type, data);
}

VALUE Entry_initialize(int argc, VALUE* argv, VALUE self);
VALUE Entry_data(VALUE self);
VALUE Entry_type(VALUE self);
VALUE Entry_sequence_number(VALUE self);
VALUE Entry_less(VALUE self, VALUE entry);
VALUE Entry_greater(VALue self, VALUE entry);
VALUE Entry_last_updated(VALUE self);
VALUE Entry_flags(VALUE self);
VALUE Entry_is_flag_set(VALUE self, VALUE flag);
VALUE Entry_update_callback(VALUE self);

void Init_entry();

