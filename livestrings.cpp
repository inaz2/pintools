// livestrings.cpp
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include "pin.H"

const int MIN_STR_LENGTH = 4;
int nullfd;

int IsValidPointer(char *addr)
{
    if (write(nullfd, addr, 1) < 0) {
        return 0;
    }
    return 1;
}

int GetPrintableLength(char *addr)
{
    int i = 0;
    while (IsValidPointer(&addr[i]) && isprint(addr[i])) {
        i++;
    }
    return i;
}

VOID RecordMemRead(VOID *ip, char *addr)
{
    int i;
    int len = GetPrintableLength(addr);
    if (len >= MIN_STR_LENGTH) {
        for (i=0; i<len; i++) {
            fprintf(stderr, "%c", addr[i]);
        }
        fprintf(stderr, "\n");
    }
}

VOID RecordMemWrite(VOID *ip, char *addr)
{
    int i;
    int len = GetPrintableLength(addr);
    if (len >= MIN_STR_LENGTH) {
        for (i=0; i<len; i++) {
            fprintf(stderr, "%c", addr[i]);
        }
        fprintf(stderr, "\n");
    }
}

VOID Instruction(INS ins, VOID *v)
{
    UINT32 memOperands = INS_MemoryOperandCount(ins);

    for (UINT32 memOp = 0; memOp < memOperands; memOp++) {
        if (INS_MemoryOperandIsRead(ins, memOp)) {
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)RecordMemRead, IARG_INST_PTR, IARG_MEMORYOP_EA, memOp, IARG_END);
        }
        if (INS_MemoryOperandIsWritten(ins, memOp)) {
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)RecordMemWrite, IARG_INST_PTR, IARG_MEMORYOP_EA, memOp, IARG_END);
        }
    }
}

int main(int argc, char *argv[])
{
    PIN_Init(argc, argv);
    nullfd = open("/dev/null", O_WRONLY);
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_StartProgram();
    return 0;
}
