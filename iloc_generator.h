#ifndef ILOC_GENERATOR_HEADER
#define ILOC_GENERATOR_HEADER

#include <stdio.h>
#include "types.h"

int generateLabel();
int generateRegister();
IlocOperation generateNop();
IlocOperation generateBinaryOpWithOneOut(IlocOperationType type, int op1, int op2, int out);
IlocOperation generateBinaryOpWithTwoOuts(IlocOperationType type, int op1, int op2, int out1, int out2);
IlocOperation generateUnaryOpWithOneOut(IlocOperationType type, int op, int out);
IlocOperation generateUnaryOpWithTwoOuts(IlocOperationType type, int op, int out1, int out2);
IlocOperation generateUnaryOpWithoutOut(IlocOperationType type, int op);
void addLabelToOperation(IlocOperation operation, int label);
char* generateCode(IlocOperationList* operationList);
IlocOperationList* createIlocList();
IlocOperationList* createIlocListFromOtherList(IlocOperationList* operationList);
void addOperationToIlocList(IlocOperationList* operationList, IlocOperation operation);
void addIlocListToIlocList(IlocOperationList* operationList, IlocOperationList* operationListToCopy);
IlocOperationList* unifyOperationLists(IlocOperationList* operationListOne, IlocOperationList* operationListTwo);

#endif