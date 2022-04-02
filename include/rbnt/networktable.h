#pragma once

#include <ruby.h>

// This class is designed to operate very similarly to Hash. 
// However, it does not support any sort of operation directly on the values 
// (of the key-value pair) or iteration, mapping, or reducing in general. This is mostly because those
// operations don't really make sense in the context.
// The only exceptions are iteration over keys, as that can still be useful, even with
// pseudo-unstructured values.

//class NetworkTable
//  extern C
//      VALUE data # The actual Hash storing the data locally. 
//                 # Purely here so nobody can ever mess with it directly from Ruby. Additionally, the rb_data_type_t struct will not be exported.
//                 # Data consistency and feedback is too critical to leave to good faith
//                 # key: id
//                 # value: Entry
//      VALUE keymap # Hash containing the id for each key name
//  end
//
//  # The name of the client sent to the server on introduction - random String if not set
//  @@client_name 
//
//  # The TCP associated with the table
//  @connection
//  # Table name
//  @name
//  # minimum time between updates
//  @update_period
//
//  def initialize(name, host, port, update_period = 20ms, default=nil, &default_proc) # one or the other, but not both defaults
//
//  def update_period
//  def update_period=
//
//  # See Hash for the behaviour of these.
//  def default
//  def default=
//  def default_proc
//  def default_proc= 
//  
//  # lookup. If the name hasn't been seen before, return default
//  def [](name)
//  # update. If the name hasn't been seen before, send create request. Otherwise, send update, if last update was older than update period.
//  # Entry type is inferred from the value or specified in the assignment. Currently if type different from storage, raise exception and do 
//  # not update. In the future, a 
//  # different type might update the table entry (namely by deleting the original, then immediately creating a new one)
//  def []=(name, type=nil, val)  alias store
//
//  def set_update_callback(name, &callback)
//  def update_callback(name) # nil if no callback
//
//  # Hash-like methods
//  def clear! # clears the table, both locally and remotely
//  def delete!(name) # deletes local and remote
//  def each_key
//  def empty?
//  def keys
//  def fetch(name, default, &default_proc) # same as [], but overrides the default behaviour; error if no default
//  def key?(name) alias has_key?, include?, member? 
//  def size alias length
//
//  def to_a # dups of everything. No access to underlying values
//  def to_h alias to_hash # dups of everything
//  
//  def translate_key(name) # convert key to entry id  
//end

struct network_table_data {
    // A hash containing entries, associated with the IDs
    VALUE data;
    // A hash mapping String names to the entry IDs
    VALUE keymap;
    // communication thread
    VALUE thread;
    // write buffer
    VALUE buffer;
    // buffer mutex
    VALUE buffer_lock;
};

size_t network_table_size(void* data) {
    return sizeof(network_table_data);
}

void network_table_mark(void* data) {
    rb_gc_mark(((struct network_table_data*)data)->data);
    rb_gc_mark(((struct network_table_data*)data)->keymap);
    rb_gc_mark(((struct network_table_data*)data)->thread);
    rb_gc_mark(((struct network_table_data*)data)->buffer);
    rb_gc_mark(((struct network_table_data*)data)->buffer_lock);
}

struct rb_data_type_t network_table_type = {
    .wrap_struct_name = "network_table",
    .function = {
        .dmark = network_table_mark,
        .dfree = RUBY_DEFAULT_FREE,
        .dsize = network_table_size,
    },
    .data = NULL,
    .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

VALUE network_table_alloc(VALUE self) {
    struct network_table_data* data;
    return TypedData_Make_Struct(self, struct network_table_data, &network_table_type, &data);
}

// errors if already instantiated
VALUE NetworkTable_m_set_client_name(VALUE self, VALUE name);
VALUE NetworkTable_m_client_name(VALUE self);

VALUE NetworkTable_initialize(int argc, VALUE* argv, VALUE self);
VALUE NetworkTable_name(VALUE self);
VALUE NetworkTable_update_period(VALUE self);
VALUE NetworkTable_set_update_period(VALUE self, VALUE period);
VALUE NetworkTable_default(VALUE self);
VALUE NetworkTable_set_default(VALUE self, VALUE defval);
VALUE NetworkTable_default_proc(VALUE self);
VALUE NetworkTable_set_default_proc(VALUE self);
VALUE NetworkTable_lookup(VALUE self, VALUE name);
VALUE NetworkTable_update(int argc, VALUE* argv, VALUE self);
VALUE NetworkTable_update_callback(VALUE self);
VALUE NetworkTable_set_update_callback(VALUE self);

VALUE NetworkTable_clear(VALUE self);
VALUE NetworkTable_delete(VALUE self, VALUE name);
VALUE NetworkTable_each_key(VALUE self);
VALUE NetworkTable_empty?(VALUE self);
VALUE NetworkTable_keys(VALUE self);
VALUE NetworkTable_fetch(int argc, VALUE* argv, VALUE self);
VALUE NetworkTable_has_key(VALUE self, VALUE name);
VALUE NetworkTable_size(VALUE self);
VALUE NetworkTable_to_a(VALUE self);
VALUE NetworkTable_to_h(VALUE self);

VALUE NetworkTable_id_for(VALUE self, VALUE name);

void Init_networktable();
