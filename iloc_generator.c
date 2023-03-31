#include "iloc_generator.h"

int generateLabel()
{
  static int label_counter = 1;
  return label_counter++;
}

int generateRegister()
{
  static int register_counter = 1;
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
    operation.type = -1;
    return operation;
}

IlocOperation generateInvalid()
{
    IlocOperation operation = generate_empty_operation();
    operation.type = OP_INVALID;
    return operation;
}

IlocOperation generateNop()
{
    IlocOperation operation = generate_empty_operation();
    operation.type = OP_NOP;
    return operation;
}

IlocOperation generateUnaryOp(IlocOperationType type) 
{
    IlocOperation operation = generate_empty_operation();
    operation.type = type;
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

IlocOperation addLabelToOperation(IlocOperation operation, int label)
{
    operation.label = label;   
    return operation;
}

void convertOperationToCode(IlocOperation operation) 
{
    if (operation.type != OP_INVALID && operation.label != -1) {
        printf("l%d: ", operation.label);
    }
    switch (operation.type)
    {  
        case OP_ADD:
            printf("add r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_ADD_TO_STACK_POINTER:
            printf("add r0, r%d => r0 \n", operation.op1);
            break;
        case OP_SUB:
            printf("sub r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_MULT:
            printf("mult r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_DIV:
            printf("div r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_NEG:
            printf("rsubI r%d, 0 => r%d \n", operation.op1, operation.out1);
            break;
        case OP_CMP_GE:
            printf("cmp_GE r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_CBR:
            printf("cbr r%d -> l%d, l%d \n", operation.op1, operation.out1, operation.out2);
            break;
        case OP_JUMPI:
            printf("jumpI -> l%d \n", operation.op1);
            break;
        case OP_JUMPI_REGISTER:
            printf("jump -> r%d \n", operation.op1);
            break;
        case OP_CMP_LE:
            printf("cmp_LE r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_CMP_LT:
            printf("cmp_LT r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_CMP_GT:
            printf("cmp_GT r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_CMP_NE:
            printf("cmp_NE r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_CMP_EQ:
            printf("cmp_EQ r%d, r%d -> r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_AND:
            printf("and r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_OR:
            printf("or r%d, r%d => r%d \n", operation.op1, operation.op2, operation.out1);
            break;
        case OP_LOADAI_GLOBAL:
            printf("loadAI rbss, %d => r%d \n", operation.op1, operation.out1);
            break;
        case OP_LOADAI_LOCAL:
            printf("loadAI r0, %d => r%d \n", operation.op1, operation.out1);
            break;
        case OP_LOADI:
            printf("loadI %d => r%d \n", operation.op1, operation.out1);
            break;
        case OP_LOADI_TO_STACK_POINTER:
            printf("addI r%d, 0 => r0 \n", operation.op1);
            break;
        case OP_LOAD_RFP_TO_STACK_POINTER:
            printf("addI rfp, 0 => r0 \n");
            break;
        case OP_LOAD_STACK_POINTER:
            printf("addI r0, 0 => r%d \n", operation.op1);
            break;
        case OP_LOAD_PC:
            printf("addI rpc, 0 => r%d \n", operation.op1);
            break;
        case OP_STOREAI_GLOBAL:
            printf("storeAI r%d => rbss, %d \n", operation.op1, operation.out1);
            break;
        case OP_STOREAI_LOCAL:
            printf("storeAI r%d => r0, %d \n", operation.op1, operation.out1);
            break;
        case OP_NOP:
            printf("nop \n");
            break;
        default:
            break;
    }
}

void generateCode(IlocOperationList* operationList) 
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
    list->operation = generateInvalid();
    list->nextItem = NULL;
    return list;
}

IlocOperationList* createIlocListFromOtherList(IlocOperationList* operationList)
{
    IlocOperationList* newOperationList = createIlocList();
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
        return;
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
        return NULL;
    }
    if (operationListTwo == NULL)
    {
        printError("[IlocOperationList] Second list was null while unifying!");
        return NULL;
    }

    IlocOperationList* newOperationList = createIlocList();

    IlocOperationList* operationToCopy = operationListOne;
    while(operationToCopy != NULL)
    {
        addOperationToIlocList(newOperationList, operationToCopy->operation);
        operationToCopy = operationToCopy->nextItem;
    }

    operationToCopy = operationListTwo;
    while(operationToCopy != NULL)
    {
        addOperationToIlocList(newOperationList, operationToCopy->operation);
        operationToCopy = operationToCopy->nextItem;
    }

    return newOperationList;
}

FunctionCallArgument* createFunctionCallArgument(int value)
{
    FunctionCallArgument* functionCallArgument = malloc(sizeof(FunctionCallArgument));
    if (!functionCallArgument) 
    {
        printf("Fail to create function call argument.");
    }
    functionCallArgument->value = value;

    return functionCallArgument;
}

FunctionCallArgument* addFunctionCallArgument(FunctionCallArgument* functionCallArgument, int value)
{
    FunctionCallArgument* newFunctionCallArgument = createFunctionCallArgument(value);
    newFunctionCallArgument->nextArgument = functionCallArgument;
    return newFunctionCallArgument;
}