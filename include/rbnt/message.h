#pragma once

#include <ruby.h>

// Keep Alive
// id: 0x00
#define MESSAGE_KEEP_ALIVE 0x00
VALUE Message_m_make_keep_alive(VALUE self);
VALUE Message_m_read_keep_alive(VALUE self, VALUE connection); // return ok

// Client Hello
// id: 0x01
// protocol revision: 0x0300
// identity: String
#define MESSAGE_CLIENT_HELLO 0x01
VALUE Message_m_make_client_hello(VALUE self, VALUE name);
VALUE Message_m_read_client_hello(VALUE self, VALUE connection); // return ok, print error

// Protocol Version Unsupported
// id: 0x02
// protocol revision: 2 bytes
// unsupported by client
#define MESSAGE_PROTOCOL_VERSION_UNSUPPORTED 0x02
VALUE Message_m_read_protocol_version_unsupported(VALUE self, VALUE connection); // return ok, version

// Server Hello Complete
// id: 0x03
// unsupported by client
#define MESSAGE_SERVER_HELLO_COMPLETE 0x03
VALUE Message_m_read_server_hello_complete(VALUE self, VALUE connection); // return ok

// Server Hello
// id: 0x04
// flags: 1 byte
// server identity: String
// unsupported by client
#define MESSAGE_SERVER_HELLO 0x04
VALUE Message_m_read_server_hello(VALUE self, VALUE connection); // return ok, flags, name

// Client Hello Complete
// id: 0x05
#define MESSAGE_CLIENT_HELLO_COMPLETE 0x05
VALUE Message_m_make_client_hello_complete(VALUE self); 
VALUE Message_m_read_client_hello_complete(VALUE self, VALUE connection); // return ok, print error

// Entry Assignment
// id: 0x10
// entry name: String
// entry type: 1 byte
// entry id: 2 bytes
// entry sequence number: 2 bytes
// entry flags: 1 byte
// entry value: Any (based on type)
#define MESSAGE_ENTRY_ASSIGNMENT 0x10
VALUE Message_m_make_entry_assignment(VALUE self, VALUE name, VALUE type, VALUE id, VALUE sn, VALUE flags, VALUE val);
VALUE Message_m_read_entry_assignment(VALUE self, VALUE connection); // return ok, id, Entry

// Entry Update
// id: 0x11
// entry id: 2 bytes
// entry sequence number: 2 bytes
// entry type: 1 byte
// entry value: Any (based on type)
#define MESSAGE_ENTRY_UPDATE 0x11
VALUE Message_m_make_entry_update(VALUE self, VALUE id, VALUE sn, VALUE type, VALUE val);
VALUE Message_m_read_entry_update(VALUE self, VALUE connection); // return ok, id, value

// Entry Flags Update
// id: 0x12
// entry id: 2 bytes
// entry flags: 1 byte
#define MESSAGE_ENTRY_FLAGS_UPDATE 0x12
VALUE Message_m_make_entry_flags_update(VALUE self, VALUE id, VALUE flags);
VALUE Message_m_read_entry_flags_update(VALUE self, VALUE connection); // return ok, id, flags

// Entry Delete
// id: 0x13
// entry id: 2 bytes
#define MESSAGE_ENTRY_DELETE 0x13
VALUE Message_m_make_entry_delete(VALUE self, VALUE id);
VALUE Message_m_read_entry_delete(VALUE self, VALUE connection); // return ok, id

// Clear All Entries
// id: 0x14
// magic value: 0xD06CB27A
#define MESSAGE_CLEAR_ALL_ENTRIES 0x14
VALUE Message_m_make_clear_all_entries(VALUE self);
VALUE Message_m_read_clear_all_entries(VALUE self, VALUE connection); // return ok, print error if magic value isn't exact

// RPC included separately
