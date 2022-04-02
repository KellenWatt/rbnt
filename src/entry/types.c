#include <ruby.h>
#include <rbnt/entry/types.h>

VALUE mTypes;

void Init_types() {
    VALUE mNetworkTable = rb_define_module("NetworkTable");
    mTypes = rb_define_module_under(mNetworkTable, "Types");

	rb_define_const(mTypes, "BOOLEAN", RB_BOOLEAN_TYPE);
	rb_define_const(mTypes, "DOUBLE", RB_DOUBLE_TYPE);
	rb_define_const(mTypes, "STRING", RB_STRING_TYPE);
	rb_define_const(mTypes, "RAW", RB_RAW_TYPE);
	rb_define_const(mTypes, "BOOL_ARY", RB_BOOLEAN_ARRAY_TYPE);
	rb_define_const(mTypes, "DOUBLE_ARY", RB_DOUBLE_ARRAY_TYPE);
	rb_define_const(mTypes, "STRING_ARY", RB_STRING_ARRAY_TYPE);
	/* rb_define_const(mTypes, "RPC", RB_NT_RPC); */
}
