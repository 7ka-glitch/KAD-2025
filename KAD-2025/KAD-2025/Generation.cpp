#include "stdafx.h"
#include <vector>
#include <stack>
#include <sstream>
#include "Generation.h"

using namespace std;

namespace GN
{
    void GenerationASM(std::ostream* stream, LT::LexTable& lextable, IT::IdTable& idtable)
    {
        ostream* file = stream;
        *file << BEGIN;
        *file << EXTERN;
        GenConstAndData(idtable, file);
        *file << CODE;
        GenCode(lextable, idtable, file);
        *file << END;
    }

    void GenConstAndData(IT::IdTable& idtable, ostream* file)
    {
        vector <string> result;
        vector <string> con;  con.push_back(CONST);
        vector <string> data;  data.push_back(DATA);

        for (int i = 0; i < idtable.current_size; i++)
        {
            
            if (idtable.table[i]->idtype == IT::IDTYPE::P)
                continue;

            string str = "\t" + string(idtable.table[i]->FullName);

            if (idtable.table[i]->idtype == IT::IDTYPE::L) 
            {
                switch (idtable.table[i]->iddatatype)
                {
                case IT::IDDATATYPE::INT:
                    
                    str += " qword " + itoS(idtable.table[i]->value.vint);
                    break;
                case IT::IDDATATYPE::STR:
                {
                    
                    string val = string(idtable.table[i]->value.vstr.str);

                    
                    if (val == "\"\"" || val.empty())
                    {
                        str += " db 0";
                    }
                    else
                    {
                        
                        str += " db " + val + ", 0";
                    }
                    break;
                }
                }
                con.push_back(str);
            }
            else if (idtable.table[i]->idtype == IT::IDTYPE::V) // Переменные
            {
                switch (idtable.table[i]->iddatatype)
                {
                case IT::IDDATATYPE::INT:
                    
                    str += " qword 0";
                    break;
                case IT::IDDATATYPE::STR:
                    str += " qword 0"; 
                    break;
                }
                data.push_back(str);
            }
        }

        result.insert(result.end(), con.begin(), con.end());
        result.insert(result.end(), data.begin(), data.end());

        for (auto r : result)
            *file << r << endl;
    }

    string GenExpressionCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i)
    {
        string str;
        for (; i < lextable.current_size; i++)
        {
            if (LT_ENTRY(i)->lexema == LEX_SEMICOLON ||
                LT_ENTRY(i)->lexema == LEX_LEFTBRACE ||
                LT_ENTRY(i)->lexema == LEX_RIGHTBRACE ||
                LT_ENTRY(i)->lexema == LEX_RIGHTHESIS) break;

            switch (LT_ENTRY(i)->lexema)
            {
            case LEX_LITERAL:
            case LEX_ID:
            {
                if (IT_ENTRY(i)->idtype == IT::IDTYPE::F)
                {
                    str += GenCallFuncCode(lextable, idtable, i);
                    str += "push rax\n";
                    i--;
                }
                else
                {
                    if (IT_ENTRY(i)->idtype == IT::IDTYPE::L && IT_ENTRY(i)->iddatatype == IT::IDDATATYPE::STR)
                        str += "lea rax, " + string(IT_ENTRY(i)->FullName) + "\npush rax\n";
                    else
                        str += "mov rax, " + string(IT_ENTRY(i)->FullName) + "\npush rax\n";
                }
                break;
            }
            case LEX_OPERATOR:
            {
                char sign = LT_ENTRY(i)->sign;
                if (sign != '+' && sign != '-' && sign != '*' && sign != '/' && sign != '%' && sign != '^')
                {
                    break;
                }

                str += "pop rcx\n";
                str += "pop rax\n";

                switch (LT_ENTRY(i)->sign)
                {
                case '+':
                    str += "add rax, rcx\n";
                    break;
                case '-':
                    str += "sub rax, rcx\n";
                    break;
                case '*':
                    str += "imul rcx\n";
                    break;
                case '/':
                    str += "cdq\n";
                    str += "idiv rcx\n";
                    break;
                case '%':
                    str += "cdq\n";
                    str += "idiv rcx\n";
                    str += "mov rax, rdx\n";
                    break;
                case '^':
                    str += "mov rdx, rcx\n";
                    str += "mov rcx, rax\n";
                    str += "mov rbx, rsp\n";
                    str += "and rsp, -16\n";
                    str += "sub rsp, 20h\n";
                    str += "call powNumber\n";
                    str += "mov rsp, rbx\n";
                    break;
                }

                str += "push rax\n";
                break;
            }
            }
        }
        return str;
    }

    void GenCode(LT::LexTable& lextable, IT::IdTable& idtable, ostream* file)
    {
        string str;
        string currentFuncName = "";
        int loopNum = 0;
        stack<LoopInfo> loopStack;

        for (int i = 0; i < lextable.current_size; i++)
        {
            switch (LT_ENTRY(i)->lexema)
            {
            case LEX_MAIN:
            {
                str = SEPSTR("MAIN") + "main PROC\nsub rsp, 28h\n";
                currentFuncName = "";
                break;
            }
            case LEX_FUNCTION:
            {
                currentFuncName = IT_ENTRY(i + 1)->FullName;
                str = GenFunctionCode(lextable, idtable, i);
                break;
            }
            case LEX_RETURN:
            {
                int exprPos = i + 1;
                PN::polishNotation(exprPos, lextable, idtable);
                str = GenExpressionCode(lextable, idtable, exprPos);
                str += "pop rax\n";
                str += "ret\n";
                i = exprPos - 1;
                break;
            }
            case LEX_ID:
            {
                if (LT_ENTRY(i + 1)->lexema == LEX_LEFTHESIS && IT_ENTRY(i)->idtype != IT::IDTYPE::F)
                    str = GenCallFuncCode(lextable, idtable, i);
                break;
            }
            case LEX_CYCLE:
            {
                loopNum++;
                string startLabel = "cycle" + itoS(loopNum);
                string endLabel = "end_cycle" + itoS(loopNum);

                int exprPos = i + 1;
                PN::polishNotation(exprPos, lextable, idtable);
                str += GenExpressionCode(lextable, idtable, exprPos);

                str += "pop rcx\n";
                str += "push rcx\n";

                str += startLabel + ":\n";
                str += "cmp qword ptr [rsp], 0\n";
                str += "jle " + endLabel + "\n";

                loopStack.push(LoopInfo(startLabel, endLabel));
                i = exprPos - 1;
                break;
            }
            case LEX_RIGHTBRACE:
            {
                if (!loopStack.empty())
                {
                    LoopInfo info = loopStack.top();
                    str += "dec qword ptr [rsp]\n";
                    str += "jmp " + info.startLabel + "\n";
                    str += info.endLabel + ":\n";
                    str += "pop rcx\n";
                    loopStack.pop();
                }
                else if (currentFuncName != "")
                {
                    str += currentFuncName + " ENDP" + SEPSTREMP;
                    currentFuncName = "";
                }
                break;
            }
            case LEX_EQUAL:
            {
                IT::Entry* var = IT_ENTRY(i - 1);
                int exprPos = i + 1;
                PN::polishNotation(exprPos, lextable, idtable);
                str += GenExpressionCode(lextable, idtable, exprPos);
                str += "pop rax\n";
                str += "mov " + string(var->FullName) + ", rax\n";
                i = exprPos - 1;
                break;
            }
            case LEX_WRITE:
            {
                int exprPos = i + 2;
                PN::polishNotation(exprPos, lextable, idtable);
                str += GenExpressionCode(lextable, idtable, exprPos);

                str += "pop rcx\n";
                str += "sub rsp, 20h\n";

                IT::IDDATATYPE paramType = IT::IDDATATYPE::INT;
                int checkPos = i + 2;

                if (LT_ENTRY(checkPos)->lexema == LEX_ID || LT_ENTRY(checkPos)->lexema == LEX_LITERAL)
                {
                    if (LT_ENTRY(checkPos)->idxTI != LT_TI_NULLXDX)
                    {
                        if (IT_ENTRY(checkPos)->iddatatype == IT::IDDATATYPE::STR)
                        {
                            paramType = IT::IDDATATYPE::STR;
                        }
                    }
                }
                else if (LT_ENTRY(checkPos)->lexema == LEX_LEFTHESIS || LT_ENTRY(checkPos)->lexema == LEX_OPERATOR)
                {
                    paramType = IT::IDDATATYPE::INT;
                }

                if (paramType == IT::IDDATATYPE::INT)
                    str += "call write_int\n";
                else
                    str += "call write_str\n";

                str += "add rsp, 20h\n";
                i = exprPos - 1;
                break;
            }
            }

            if (!str.empty())
            {
                *file << str << endl;
                str.clear();
            }
        }
    }

    string GenFunctionCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i)
    {
        string str = "";
        string funcName = IT_ENTRY(i + 1)->FullName;
        str += SEPSTR(funcName) + funcName + string(" PROC");
        i += 3;

        vector<ParamInfo> params;
        int paramCount = 0;
        bool first = true;

        // Собираем параметры
        while (LT_ENTRY(i)->lexema != LEX_RIGHTHESIS)
        {
            if (LT_ENTRY(i)->lexema == LEX_ID)
            {
                if (first) {
                    str += ", ";
                    first = false;
                }
                else str += ", ";
                str += string(IT_ENTRY(i)->FullName) + ": QWORD";
                params.push_back(ParamInfo(string(IT_ENTRY(i)->FullName), paramCount++));

                // Отмечаем параметр как переменную (V) для использования в теле функции
                IT_ENTRY(i)->idtype = IT::IDTYPE::V;
            }
            i++;
        }
        str += "\n";

        // Сохраняем параметры из регистров в переменные
        for (const auto& param : params)
        {
            switch (param.regIndex)
            {
            case 0: str += "mov " + param.name + ", rcx\n"; break;
            case 1: str += "mov " + param.name + ", rdx\n"; break;
            case 2: str += "mov " + param.name + ", r8\n"; break;
            case 3: str += "mov " + param.name + ", r9\n"; break;
            }
        }

        return str;
    }

    string GenCallFuncCode(LT::LexTable& lextable, IT::IdTable& idtable, int& i)
    {
        string str;
        IT::Entry* e = IT_ENTRY(i);
        stack<IT::Entry*> temp;

        i++;
        if (i < lextable.current_size && LT_ENTRY(i)->lexema == LEX_LEFTHESIS) i++;

        // Собираем аргументы
        for (; i < lextable.current_size; i++)
        {
            if (LT_ENTRY(i)->lexema == LEX_SEMICOLON ||
                LT_ENTRY(i)->lexema == LEX_RIGHTBRACE ||
                LT_ENTRY(i)->lexema == LEX_RIGHTHESIS ||
                LT_ENTRY(i)->lexema == '@')
                break;

            if ((LT_ENTRY(i)->lexema == LEX_ID || LT_ENTRY(i)->lexema == LEX_LITERAL) &&
                LT_ENTRY(i)->idxTI != LT_TI_NULLXDX)
                temp.push(IT_ENTRY(i));
        }

        vector<IT::Entry*> args;
        while (!temp.empty()) {
            args.insert(args.begin(), temp.top());
            temp.pop();
        }

        // Выделяем shadow space
        str += "sub rsp, 20h\n";

        // Передаем аргументы в правильном порядке
        for (size_t k = 0; k < args.size(); k++)
        {
            string src;
            if (args[k]->idtype == IT::IDTYPE::L && args[k]->iddatatype == IT::IDDATATYPE::STR)
                src = "lea rax, " + string(args[k]->FullName);
            else
                src = "mov rax, " + string(args[k]->FullName);

            str += src + "\n";

            // Для x64: первые 4 аргумента через RCX, RDX, R8, R9
            if (k == 0) str += "mov rcx, rax\n";
            else if (k == 1) str += "mov rdx, rax\n";
            else if (k == 2) str += "mov r8, rax\n";
            else if (k == 3) str += "mov r9, rax\n";
            else
            {
                // Больше 4 аргументов - через стек (редкий случай)
                str += "push rax\n";
            }
        }

        str += "call " + string(e->FullName) + "\n";
        str += "add rsp, 20h\n";

        // Очищаем стек от дополнительных аргументов (>4)
        if (args.size() > 4)
        {
            str += "add rsp, " + itoS(8 * (args.size() - 4)) + "\n";
        }

        if (i < lextable.current_size && LT_ENTRY(i)->lexema == '@')
        {
            i++;
            i++;
        }

        return str;
    }

    string itoS(int x)
    {
        stringstream r;  r << x;  return r.str();
    }
}