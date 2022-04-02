#include <ruby.h>
#include <rbnt/entry/flags.h>

void Init_flags() {
    VALUE mNetworkTable = rb_define_module("NetworkTable");
    VALUE mFlags = rb_define_module_under(mNetworkTable, "Flags");

    rb_define_const(mFlags, "PERSISTENT", RB_PERSISTENT_FLAG);
}
