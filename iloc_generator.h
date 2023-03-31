#ifndef ILOC_GENERATOR_HEADER
#define ILOC_GENERATOR_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "print.h"

int generateLabel();
int generateRegister();
IlocOperation generate_empty_operation();
IlocOperation generateNop();
IlocOperation generateUnaryOp(IlocOperationType type);
IlocOperation generateBinaryOpWithOneOut(IlocOperationType type, int op1, int op2, int out);
IlocOperation generateBinaryOpWithTwoOuts(IlocOperationType type, int op1, int op2, int out1, int out2);
IlocOperation generateUnaryOpWithOneOut(IlocOperationType type, int op, int out);
IlocOperation generateUnaryOpWithTwoOuts(IlocOperationType type, int op, int out1, int out2);
IlocOperation generateUnaryOpWithoutOut(IlocOperationType type, int op);
IlocOperation addLabelToOperation(IlocOperation operation, int label);
void generateCode(IlocOperationList* operationList);
IlocOperationList* createIlocList();
IlocOperationList* createIlocListFromOtherList(IlocOperationList* operationList);
void addOperationToIlocList(IlocOperationList* operationList, IlocOperation operation);
void addIlocListToIlocList(IlocOperationList* operationList, IlocOperationList* operationListToCopy);
IlocOperationList* unifyOperationLists(IlocOperationList* operationListOne, IlocOperationList* operationListTwo);
FunctionCallArgument* createFunctionCallArgument(int value);
FunctionCallArgument* addFunctionCallArgument(FunctionCallArgument* functionCallArgument, int value);

#endif