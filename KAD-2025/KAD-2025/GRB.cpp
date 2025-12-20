#include "stdafx.h"
#include "GRB.h"
#include <string.h>
#define GRB_ERROR_SERIES 600

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

    Greibach greibach(NS('S'), TS('$'), 7,

        // 1. NS('S') - СТАРТ (Структура программы: main и функции)
        Rule(NS('S'), GRB_ERROR_SERIES + 0, // Неверная структура программы
            3,
            // main { N } S
            Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), NS('S')),
            // t f i ( F ) { N r E ; } S (объявление функции с возвратом)
            Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),
            // Пустой переход (конец программы)
            Rule::Chain()
        ),

       // 2. NS('N') - ОПЕРАТОРЫ 
        Rule(NS('N'), GRB_ERROR_SERIES + 1, // Ошибочный оператор
            7,
            // 1. Объявление: t i ; N
            Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('N')),           
            // 2. Объявление с инициализацией: t i = E ; N
            Rule::Chain(6, TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
            // 3. Присваивание: i = E ; N
            Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),  
            // 4. Вывод: w ( E ) ; 
            Rule::Chain(6, TS('w'), TS('('), NS('E'), TS(')'), TS(';'), NS('N')), 
            // 5. Цикл: ~ E { N } ; N (Цикл по выражению)
            Rule::Chain(7, TS('~'), NS('E'), TS('{'), NS('N'), TS('}'), TS(';'), NS('N')), 
            // 6. Вызов функции (как оператор): i ( W ) ; N
            Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),
            // 7. Пустой переход
            Rule::Chain()
        ),

        // 3. NS('E') - ВЫРАЖЕНИЕ (Арифметическое)
        Rule(NS('E'), GRB_ERROR_SERIES + 2, // Ошибочное выражение
            4,
            Rule::Chain(2, TS('i'), NS('M')),               // id M
            Rule::Chain(2, TS('l'), NS('M')),               // literal M
            Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')), // ( E ) M
            Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')) // call(W) M
        ),

        // 4. NS('M') - ПРОДОЛЖЕНИЕ ВЫРАЖЕНИЯ (Операторы: +, -, *, /, #)
        Rule(NS('M'), GRB_ERROR_SERIES + 3, // Неверный оператор в выражении
            2,
            Rule::Chain(2, TS('#'), NS('E')), // operator # E 
            Rule::Chain()
        ),

        // 5. NS('F') - ОПРЕДЕЛЕНИЕ ПАРАМЕТРОВ ФУНКЦИИ (int param id, ...)
        Rule(NS('F'), GRB_ERROR_SERIES + 4, // Неверное определение параметров
            3,
            Rule::Chain(2, TS('t'), NS('P')),               // int P
            Rule::Chain(4, TS('t'), NS('P'), TS(','), NS('F')), // int P , F
            Rule::Chain()                                   // Без параметров
        ),

        // 6. NS('P') - ОДИН ПАРАМЕТР (param id)
        Rule(NS('P'), GRB_ERROR_SERIES + 4, // Неверное объявление одного параметра
            1,
            Rule::Chain(2, TS('p'), TS('i')) // param id
        ),

        // 7. NS('W') - АРГУМЕНТЫ ВЫЗОВА ФУНКЦИИ (id, literal, ...)
        Rule(NS('W'), GRB_ERROR_SERIES + 5, // Неверные аргументы вызова
            5,
            Rule::Chain(1, TS('i')),                        // id
            Rule::Chain(1, TS('l')),                        // literal
            Rule::Chain(3, TS('i'), TS(','), NS('W')),      // id , W
            Rule::Chain(3, TS('l'), TS(','), NS('W')),      // literal , W
            Rule::Chain()                                   // Нет аргументов
        )

        
    );


    Rule::Chain::Chain(short size, GRBALPHABET s, ...)
    {
        this->size = size;
        nt = new GRBALPHABET[size];

        nt[0] = s;

        va_list args;
        va_start(args, s);
        for (short i = 1; i < size; i++)
        {
           
            nt[i] = (GRBALPHABET)va_arg(args, int);
        }
        va_end(args);
    }

    Rule::Rule(GRBALPHABET nn, int iderror, short size, Chain c, ...)
    {
        this->nn = nn;
        this->iderror = iderror;
        this->size = size;
        this->chains = new Chain[size];

        chains[0] = c;

        va_list args;
        va_start(args, c);
        for (int i = 1; i < size; i++)
        {
            chains[i] = va_arg(args, Chain);
        }
        va_end(args);
    }

    Greibach::Greibach(GRBALPHABET startN, GRBALPHABET stbottomT, short size, Rule r, ...)
    {
        this->startN = startN;
        this->stbottomT = stbottomT;
        this->size = size;
        this->rules = new Rule[size];

        rules[0] = r;

        va_list args;
        va_start(args, r);
        for (int i = 1; i < size; i++)
        {
            rules[i] = va_arg(args, Rule);
        }
        va_end(args);
    }

	short Greibach::getRule(GRBALPHABET nn, Rule& rule)
	{
		short rc = -1, k = 0;

		while (k < size && rules[k].nn != nn)
			k++;

		if (k < size)
			rule = rules[rc = k];

		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size)
			rc = rules[n];
		return rc;
	}

	char* Rule::getCRule(char* b, short nchain)
	{
		char buf[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0;
		chains[nchain].getCChain(buf);
		strcat_s(b, sizeof(buf) + 5, buf);

		return b;
	}

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;

		while (j < size && chains[j].nt[0] != t && chains[j].nt[0] != 0)
			j++;

		rc = (j < size ? j : -1);
		if (rc >= 0)
			pchain = chains[rc];
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; i++)
			b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}

	Greibach getGreibach() { return greibach; }
}