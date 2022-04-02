#include <ruby.h>
#include <stdint.h>

#include "rbnt/entry.h"
#include "rbnt/util/sequencenumber.h"

// initialize(val, sn: nil, type: nil, flags: [], &update_callback)
VALUE Entry_initialize(int argc, VALUE* argv, VALUE self) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);
    VALUE keys;

    rb_scan_args(argc, argv, "1:&", &(data->data), &keys, &(data->update_callback));
    
    data->last_updated = rb_funcall(cTime, rb_intern("new"), 0);
    
    VALUE sn;
    VALUE type;
    VALUE flags;
    if(!NIL_P(keys)) {
       sn = rb_hash_aref(keys, rb_str_new_literal("sn"));
       type = rb_hash_aref(keys, rb_str_new_literal("type"));
       flags = rb_hash_aref(keys, rb_str_new_literal("flags"));
    } 
    
    data->sequence_number = NUM2USHORT(sn);
    data->type = NUM2CHR(type);
    if(RB_TYPE_P(flags) == T_ARRAY) {
        long size = RARRAY_LEN(flags);
        uint8_t bits = 0;
        for(int i=0; i<size; i++) {
            bits |= NUM2CHR(rb_ary_entry(flags, i));
        }
        data->flags = bits;
    } else if (!NIL_P(flags)) {
        data->flags = NUM2CHR(flags);
    } else {
        data->flags = 0;
    }

    return self;
}

VALUE Entry_data(VALUE self) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);

    return data->data;
}

VALUE Entry_type(VALUE self) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);

    return UINT2NUM(data->type);
}

VALUE Entry_sequence_number(VALUE self) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);

    return UINT2NUM(data->sequence_number);
}

VALUE Entry_less(VALUE self, VALUE entry) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);
    struct entry_data* other;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &other);

    return sn_less(data->sequence_number, other->sequence_number) ? Qtrue : Qfalse;
}

VALUE Entry_greater(VALUE self, VALUE entry) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);
    struct entry_data* other;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &other);

    return sn_greater(data->sequence_number, other->sequence_number) ? Qtrue : Qfalse;
}

VALUE Entry_last_updated(VALUE self) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);
    
    return data->last_updated;
}

VALUE Entry_flags(VALUE self) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);

    return UINT2NUM(data->flags);
}

VALUE Entry_is_flag_set(VALUE self, VALUE flag) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);

    uint8_t cflag = NUM2CHR(flag);

    return (data->flags & cflag) ? Qtrue : Qfalse;
}

VALUE Entry_update_callback(VALUE self) {
    struct entry_data* data;
    TypedData_Get_Struct(self, struct entry_data, &entry_type, &data);

    return data->update_callback;
}


void Init_entry() {
    /* rb_require("date"); */
    /* cDateTime = rb_const_get(rb_cObject, rb_intern("DateTime")); */

    VALUE cNetworkTable = rb_define_class("NetworkTable", rb_cObject);
    VALUE cEntry = rb_define_class_under(cNetworkTable, "Entry", rb_cObject);

    rb_define_alloc_func(cEntry, entry_alloc);
    rb_define_method(cEntry, "initialize", Entry_initialize, -1);
    rb_define_method(cEntry, "data", Entry_data, 0);
    rb_define_method(cEntry, "type", Entry_type, 0);
    rb_define_method(cEntry, "sequence_number", Entry_sequence_number, 0);
    rb_define_method(cEntry, "<", Entry_less, 1);
    rb_define_method(cEntry, ">", Entry_greater, 1);
    rb_define_method(cEntry, "last_updated", Entry_last_updated, 0);
    rb_define_method(cEntry, "flags", Entry_flags, 0);
    rb_define_method(cEntry, "is_flag_set", Entry_is_flag_set, 1);
    rb_define_method(cEntry, "update_callback", Entry_update_callback, 0);
}
