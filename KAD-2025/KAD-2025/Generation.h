#pragma once
#include "LT.h"
#include "IT.h"
#include "PolishNotation.h"
#include <stack>
#include <string>

#define IT_ENTRY(x)     idtable.table[lextable.table[x]->idxTI]
#define LT_ENTRY(x)     lextable.table[x]

#define SEPSTREMP  "\n;------------------------------\n"
#define SEPSTR(x)  "\n;----------- " + string(x) + " ------------\n"

// ЗАГОЛОВОК x64
#define BEGIN            "includelib kernel32.lib\n" \
                        << "ExitProcess PROTO\n" \

// Внешние функции
#define EXTERN           "EXTRN lenght: PROC\n" \
                       << "EXTRN write_int: PROC\n" \
                       << "EXTRN write_str: PROC\n" \
                       << "EXTRN intToStr: PROC\n" \
                       << "EXTRN powNumber: PROC\n" \
                       << "EXTRN system: PROC\n\n" 

#define CONST ".const\nnulError db 'error divided by zero', 0\nnul qword 0\npauseStr db \"pause\",0\n"
#define DATA            ".data\n"
#define CODE            ".code\n"

#define END             "jmp goodExit\nerrorExit:\nsub rsp, 28h\nlea rcx, nulError\ncall write_str\nadd rsp, 28h\ngoodExit:\nsub rsp, 40h\nlea rcx, pauseStr\nsub rsp, 20h\ncall system\nadd rsp, 20h\nxor rcx, rcx\nsub rsp, 20h\ncall ExitProcess\nmain ENDP\nend"

namespace GN
{
    struct LoopInfo
    {
        string startLabel;
        string endLabel;
        LoopInfo(string s, string e) { startLabel = s; endLabel = e; }
    };

    struct ParamInfo
    {
        string name;
        int regIndex;
        ParamInfo(string n, int r) { name = n; regIndex = r; }
    };

    void GenerationASM(std::ostream* stream, LT::LexTable& lextable, IT::IdTable& idtable);
    void GenConstAndData(IT::IdTable& idtable, ostream* file);
    void GenCode(LT::LexTable& lextable, IT::IdTable& idtable, ostream* file);

    string GenExpressionCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i);
    string GenFunctionCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i);
    string GenCallFuncCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i);

    string itoS(int x);
}