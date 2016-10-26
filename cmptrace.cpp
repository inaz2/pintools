// cmptrace.cpp
#include <cstdio>
#include "pin.H"

VOID print_cmp_mem(VOID *ip, UINT64 * addr, ADDRINT value) {
    fprintf(stderr, "[%016lx] cmp 0x%lx, 0x%lx\n", (UINT64)ip, *addr, value);
}

VOID print_cmp_reg(VOID *ip, ADDRINT lvalue, ADDRINT rvalue) {
    fprintf(stderr, "[%016lx] cmp 0x%lx, 0x%lx\n", (UINT64)ip, lvalue, rvalue);
}

VOID Instruction(INS ins, VOID *v)
{
    if (INS_Opcode(ins) == XED_ICLASS_CMP) {
        if (INS_MemoryOperandCount(ins) == 1) {
            if (INS_OperandIsImmediate(ins, 1)) {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)print_cmp_mem, IARG_INST_PTR, IARG_MEMORYOP_EA, 0, IARG_ADDRINT, INS_OperandImmediate(ins, 1), IARG_END);
            } else if (INS_OperandIsReg(ins, 0)) {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)print_cmp_mem, IARG_INST_PTR, IARG_MEMORYOP_EA, 0, IARG_REG_VALUE, INS_OperandReg(ins, 0), IARG_END);
            } else {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)print_cmp_mem, IARG_INST_PTR, IARG_MEMORYOP_EA, 0, IARG_REG_VALUE, INS_OperandReg(ins, 1), IARG_END);
            }
        } else {
            if (INS_OperandIsImmediate(ins, 1)) {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)print_cmp_reg, IARG_INST_PTR, IARG_REG_VALUE, INS_OperandReg(ins, 0), IARG_ADDRINT, INS_OperandImmediate(ins, 1), IARG_END);
            } else {
                INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)print_cmp_reg, IARG_INST_PTR, IARG_REG_VALUE, INS_OperandReg(ins, 0), IARG_REG_VALUE, INS_OperandReg(ins, 1), IARG_END);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    PIN_Init(argc, argv);
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_StartProgram();
    return 0;
}
