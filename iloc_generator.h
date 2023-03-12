#ifndef ILOC_GENERATOR_HEADER
#define ILOC_GENERATOR_HEADER

#include "types.h"

int generate_label();
int generate_register();
ILOCOperation generate_binary_op_with_one_out(ILOCOperationType type, int op1, int op2, int out);
ILOCOperation generate_binary_op_with_two_out(ILOCOperationType type, int op1, int op2, int out1, int out2);
ILOCOperation generate_unary_op_with_one_out(ILOCOperationType type, int op, int out);
ILOCOperation generate_unary_op_with_two_out(ILOCOperationType type, int op, int out1, int out2);

#endif