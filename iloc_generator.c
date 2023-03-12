#include "iloc_generator.h"

int generateLabel()
{
  static int label_counter = 0;
  return label_counter++;
}

int generateRegister()
{
  static int register_counter = 0;
  return register_counter++;
}

IlocOperation generate_empty_operation() 
{
    IlocOperation operation;
    operation.label = -1;
    operation.op1 = -1;
    operation.op2 = -1;
    operation.out1 = -1;
    operation.out2 = -1;
    return operation;
}

IlocOperation generateNop()
{
    IlocOperation operation = generate_empty_operation();
    operation.type = OP_NOP;
    return operation;
}

IlocOperation generateBinaryOpWithOneOut(IlocOperationType type, int op1, int op2, int out) 
{
    IlocOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op1;
    operation.op2 = op2;
    operation.out1 = out;
    return operation;
}

IlocOperation generateBinaryOpWithTwoOuts(IlocOperationType type, int op1, int op2, int out1, int out2) 
{
    IlocOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op1;
    operation.op2 = op2;
    operation.out1 = out1;
    operation.out2 = out2;
    return operation;
}

IlocOperation generateUnaryOpWithOneOut(IlocOperationType type, int op, int out) 
{
    IlocOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op;
    operation.out1 = out;
    return operation;
}

IlocOperation generateUnaryOpWithTwoOuts(IlocOperationType type, int op, int out1, int out2) 
{
    IlocOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op;
    operation.out1 = out1;
    operation.out2 = out2;
    return operation;
}

char* generateCode(IlocOperationList* operationList) 
{
    // TODO
}

IlocOperationList* createIlocList()
{
    IlocOperationList* list = malloc(sizeof(IlocOperationList));
    if (!list) 
    {
        printError("[IlocOperationList] Fail to create ILOC operations list!");
        return NULL;
    }
    list->operation = generateNop();
    list->nextItem = NULL;
    return list;
}

void addOperationToIlocList(IlocOperationList* operationList, IlocOperation operation)
{
    if (operationList == NULL)
    {
        printError("[IlocOperationList] Fail to add operation on null list!");
        return NULL;
    }
    
    IlocOperationList* lastOperation = operationList;
    while(lastOperation->nextItem != NULL)
    {
        lastOperation = lastOperation->nextItem;
    }

    IlocOperationList* newOperation = createIlocList();
    newOperation->operation = operation;

    lastOperation->nextItem = newOperation;
}

void unifyOperationLists(IlocOperationList* operationListOne, IlocOperationList* operationListTwo)
{
    if (operationListOne == NULL)
    {
        printError("[IlocOperationList] First list was null while unifying!");
        return;
    }
    if (operationListTwo == NULL)
    {
        printError("[IlocOperationList] Second list was null while unifying!");
        return;
    }

    IlocOperationList* lastOperationFromFirstList = operationListOne;
    while(lastOperationFromFirstList->nextItem != NULL)
    {
        lastOperationFromFirstList = lastOperationFromFirstList->nextItem;
    }

    lastOperationFromFirstList->nextItem = operationListTwo;
}