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

IlocOperation generateUnaryOpWithoutOut(IlocOperationType type, int op)
{
    IlocOperation operation = generate_empty_operation();
    operation.type = type;
    operation.op1 = op;
    return operation;
}

void addLabelToOperation(IlocOperation operation, int label)
{
    operation.label = label;   
}

void convertOperationToCode(IlocOperation operation) 
{
    switch (operation.type)
    {  
        case OP_ADD:
            /* TODO */
            break;
        case OP_SUB:
            /* TODO */
            break;
        case OP_MULT:
            /* TODO */
            break;
        case OP_DIV:
            /* TODO */
            break;
        case OP_CMP_GE:
            /* TODO */
            break;
        case OP_CBR:
            /* TODO */
            break;
        case OP_JUMPI:
            /* TODO */
            break;
        case OP_CMP_LE:
            /* TODO */
        break;
        case OP_CMP_LT:
            /* TODO */
            break;
        case OP_CMP_GT:
            /* TODO */
            break;
        case OP_CMP_NE:
            /* TODO */
            break;
        case OP_CMP_EQ:
            /* TODO */
            break;
        case OP_AND:
            /* TODO */
            break;
        case OP_OR:
            /* TODO */
            break;
        case OP_LOADI_GLOBAL:
            /* TODO */
            break;
        case OP_LOADI_LOCAL:
            /* TODO */
            break;
        case OP_LOADI:
            /* TODO */
            break;
        case OP_STOREAI_GLOBAL:
            /* TODO */
            break;
        case OP_STOREAI_LOCAL:
            /* TODO */
            break;
        case OP_NOP:
            /* TODO */
            break;
        default:
            break;
    }
}

char* generateCode(IlocOperationList* operationList) 
{
    IlocOperationList* nextOperation = operationList;

    while(nextOperation != NULL)
    {
        IlocOperation operation = nextOperation->operation;
        convertOperationToCode(operation);
        nextOperation = nextOperation->nextItem;
    }
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

IlocOperationList* createIlocListFromOtherList(IlocOperationList* operationList)
{
    IlocOperationList* newOperationList = malloc(sizeof(IlocOperationList));
    if (!newOperationList) 
    {
        printError("[IlocOperationList] Fail to create ILOC operations list!");
        return NULL;
    }
    IlocOperationList* operationToCopy = operationList;
    while(operationToCopy != NULL)
    {
        addOperationToIlocList(newOperationList, operationToCopy->operation);
        operationToCopy = operationToCopy->nextItem;
    }
    return newOperationList;
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

void addIlocListToIlocList(IlocOperationList* operationList, IlocOperationList* operationListToCopy)
{
    IlocOperationList* operationToCopy = operationListToCopy;
    while(operationToCopy != NULL)
    {
        addOperationToIlocList(operationList, operationToCopy->operation);
        operationToCopy = operationToCopy->nextItem;
    }
}

IlocOperationList* unifyOperationLists(IlocOperationList* operationListOne, IlocOperationList* operationListTwo)
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

    IlocOperationList* newOperationList = createIlocList();

    IlocOperationList* operationToCopy = operationListOne;
    while(operationToCopy != NULL)
    {
        addOperationToIlocList(newOperationList, operationToCopy->operation);
        operationToCopy = operationToCopy->nextItem;
    }

    IlocOperationList* operationToCopy = operationListTwo;
    while(operationToCopy != NULL)
    {
        addOperationToIlocList(newOperationList, operationToCopy->operation);
        operationToCopy = operationToCopy->nextItem;
    }

    return newOperationList;
}