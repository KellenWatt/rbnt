#include <ruby.h>
#include <ruby/thread.h>

#include <time.h>
#include <stdlib.h>

#include "rbnt/networktable.h"
#include "rbnt/entry.h"

static const DEFAULT_NAME_LENGTH = 10;
static const DEFAULT_MSG_BUFFER_CAP = 20;

#define cv_client_name "@@client_name"
#define iv_default "@default"
#define iv_default_proc "@default_proc"
#define iv_update_period "@update_period"
#define iv_table_name "@name"
#define iv_connection "@connection"

static VALUE cTCPSocket;


VALUE generate_random_name(uint8_t size) {
    char* name = malloc(sizeof(*name) * size);
    for(int i =0; i<size; i++) {
        name[i] = (char)(rand() % 94 + ' ');
    }
    VALUE out = rb_str_new(name, size);
    free(name);
    return out;
}

VALUE read_message_buffer(VALUE nt) {
    struct network_table_data* data;
    TypedData_Get_Struct(nt, struct network_table_data, &network_table_type, &data);

    VALUE zero = INT2NUM(0);
    VALUE f_send = rb_intern("send")
    VALUE connection = rb_iv_get(nt, iv_connection);
    VALUE message;
    VALUE size;
    while(true) {
        // act on all messages in the socket buffer


        // send all messages in the buffer
        rb_mutex_lock(data->buffer_lock);
        size = RARRAY_LEN(data->buffer);
        for(int i=0; i<size; i++) {
            rb_funcall(cTCPSocket, f_send, 2, rb_ary_entry(data->buffer, i), zero);
        }
        rb_ary_clear();
        rb_mutex_unlock(data->buffer_lock);

        rb_thread_sleep(rb_iv_get(nt, iv_update_period));
    }
}

// Every <update_period> seconds, update any entries, as demanded by incoming messages.
// Make sure to check sequence numbers for updates.
// Then send the message buffer built up since the last period, if any. Any sn collisions 
// will be dealt with on the server side.
VALUE message_handler(VALUE nt) {
    
}

VALUE NetworkTable_m_set_client_name(VALUE self, VALUE name) {
    rb_cv_set(self, cv_client_name, name);
    return name;
}

VALUE NetworkTable_m_client_name(VALUE self) {
    return rb_cv_get(self, cv_client_name);
}

VALUE NetworkTable_initialize(int argc, VALUE* argv, VALUE self) {
    VALUE name;
    VALUE host;
    VALUE port;
    VALUE update_period;
    VALUE def;

    const char* format = rb_block_given_p() ? "32" : "31&";

    rb_scan_args(argc, argv, format, &name, &host, &port, &update_period, &def);
    
    rb_iv_set(self, iv_table_name, name);
    rb_iv_set(self, v_connection, rb_class_new_instance(cTCPSocket, host, port));
    
    if(NIL_P(update_period)) {
        update_period = rb_float_new(0.02);
    }
    rb_iv_set(self, iv_update_period, update_period);

    rb_iv_set(self, iv_defualt_proc, Qnil);
    rb_iv_set(self, iv_default, Qnil;)
    if(!NIL_P(def)) {
        rb_iv_set(self, rb_block_given_p() ? iv_default_proc : iv_default, def);
    }

    struct network_table_data* data;
    TypedData_Get_Struct(self, struct network_table_data, &network_table_type, &data);

    data->data = rb_hash_new();
    data->keymap = rb_hash_new();
    data->thread = rb_thread_create(/* TODO */, self);
    data->buffer = rb_ary_new_capa(DEFAULT_MSG_BUFFER_CAP);
    data->buffer_lock = rb_mutex_new();

    return self
}

void Init_networktable() {
    srand(time(NULL));

    rb_require("socket");
    cTCPSocket = rb_const_get(rb_cObject, "TCPSocket");

    cNetworkTable = rb_define_class("NetworkTable", rb_cObject);
   
    rb_define_alloc_func(cNetworkTable, network_table_alloc);

    rb_define_module_function(cNetworkTable, "client_name=", NetworkTable_m_set_client_name, 1);
    rb_define_module_function(cNetworkTable, "client_name", NetworkTable_m_client_name, 0);




    NetworkTable_m_set_client_name(cNetworkTable, generate_random_name(DEFAULT_NAME_LENGTH));
}
