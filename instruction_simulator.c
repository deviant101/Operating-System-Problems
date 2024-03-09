#include <stdio.h>

// declared global enum for opcodes
typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
} OpCode;

// instruction structure
typedef struct {
    OpCode op;
    int operand1;
    int operand2;
    int result;
} Instruction;

// Function prototypes
void FetchDecodeExecute(Instruction* program, int* pc);
void printInstruction(Instruction instr);

int main() {
    // Define program memory
    int size=5;
    Instruction program[5] = {
        {ADD, 1, 2, 0},    // ADD 1, 2
        {SUB, 3, 4, 0},    // SUB 3, 4
        {MUL, 5, 6, 0},   // MULT 5, 6
        {DIV, 7, 8, 0},   // DIV 7, 8
        {MOD, 9, 10, 0},  // MOD 9, 10
    };

    int instr_no = 0;
    while(instr_no<5){
        FetchDecodeExecute(program,&instr_no);
    }

    return 0;
}

void FetchDecodeExecute(Instruction* program, int* pc) {
    // Fetch
    Instruction instr = program[*pc];

    // Print current instruction
    printf("Current Instruction: ");
    printInstruction(instr);

    // Decode and execute
    switch (instr.op) {
        case ADD:
            instr.result = instr.operand1+instr.operand2;
            printf("ADD operation performed. Result: %d\n\n", instr.result);
            break;
        case SUB:
            instr.result = instr.operand1-instr.operand2;
            printf("SUB operation performed. Result: %d\n\n", instr.result);
            break;
        case MUL:
            instr.result = instr.operand1*instr.operand2;
            printf("MUL operation performed. Result: %d\n\n", instr.result);
            break;
        case DIV:
            if (instr.operand2 != 0){
                instr.result = instr.operand1/instr.operand2;
                printf("DIV operation performed. Result: %d\n\n", instr.result);
            }
            else
                printf("Error: Division by zero\n\n");
            break;
        case MOD:
            instr.result = instr.operand1 % instr.operand2;
            printf("MOD operation performed. Result: %d\n\n", instr.result);
            break;
        default:
            printf("Invalid opcode\n\n");
    }
    (*pc)++;
}

// Function to print instruction details
void printInstruction(const Instruction instr) {
    switch (instr.op) {
        case ADD:
            printf("ADD %d, %d\n",instr.operand1,instr.operand2);
            break;
        case SUB:
            printf("SUB %d, %d\n",instr.operand1,instr.operand2);
            break;
        case MUL:
            printf("MUL %d, %d\n",instr.operand1,instr.operand2);
            break;
        case DIV:
            printf("DIV %d, %d\n",instr.operand1,instr.operand2);
            break;
        case MOD:
            printf("MOD %d, %d\n",instr.result);
            break;
        default:
            printf("Invalid opcode\n");
    }
}
