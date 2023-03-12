#include "iloc_generator.h"

int generate_label(void){
  static int label_counter = 0;
  return label_counter++;
}

int generate_register(){
  static int register_counter = 0;
  return register_counter++;
}

ILOCOperation generate_empty_operation() {
    ILOCOperation operation;
    operation.label = -1;
    operation.op1 = -1;
    operation.op2 = -1;
    operation.out1 = -1;
    operation.out2 = -1;
    return operation;
}

ILOCOperation generate_binary_op_with_one_out(ILOCOperationType type, int op1, int op2, int out) {
    ILOCOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op1;
    operation.op2 = op2;
    operation.out1 = out;
    return operation;
}

ILOCOperation generate_binary_op_with_two_out(ILOCOperationType type, int op1, int op2, int out1, int out2) {
    ILOCOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op1;
    operation.op2 = op2;
    operation.out1 = out1;
    operation.out2 = out2;
    return operation;
}

ILOCOperation generate_unary_op_with_one_out(ILOCOperationType type, int op, int out) {
    ILOCOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op;
    operation.out1 = out;
    return operation;
}

ILOCOperation generate_unary_op_with_two_out(ILOCOperationType type, int op, int out1, int out2) {
    ILOCOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op;
    operation.out1 = out1;
    operation.out2 = out2;
    return operation;
}