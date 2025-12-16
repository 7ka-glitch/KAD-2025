#include "stdafx.h"
#include "PolishNotation.h"
#include "Error.h"
#include <stack>
#include <queue>

using namespace std;

namespace PN
{

	int priority(char v)
	{
		switch (v)
		{
		case '(':
		case ')': return 1;
		case ',': return 2;
		case '+':
		case '-': return 3;
		case '*':
		case '/': return 4;
		case '^': return 5;
		default: return 0;
		}
	}

	bool polishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		stack<LT::Entry*> stk;
		queue<LT::Entry*> result;
		bool function = false;
		int quantityParm = 0;
		int i = lextable_pos;


		for (; lextable.table[i]->lexema != LEX_SEMICOLON && lextable.table[i]->lexema != LEX_LEFTBRACE && (lextable.table[i]->lexema != LEX_RIGHTHESIS || !stk.empty()); i++)
		{
			switch (lextable.table[i]->lexema)
			{
			case LEX_ID:
			case LEX_LITERAL:
				if (idtable.table[lextable.table[i]->idxTI]->idtype == IT::IDTYPE::F)
				{
					quantityParm = 0;
					function = true;
					result.push(lextable.table[i]);
					break;
				}
				if (function && !quantityParm)
					quantityParm++;

				result.push(lextable.table[i]);
				break;

			case LEX_OPERATOR:
			{
				int p_cur = priority(lextable.table[i]->sign);

				if (stk.empty() || stk.top()->lexema == LEX_LEFTHESIS) {
					stk.push(lextable.table[i]);
				}
				else {
					while (!stk.empty() && stk.top()->lexema == LEX_OPERATOR) {
						int p_top = priority(stk.top()->sign);

						bool isRightAssoc = (lextable.table[i]->sign == '^');

						if (p_top > p_cur || (p_top == p_cur && !isRightAssoc)) {
							result.push(stk.top());
							stk.pop();
						}
						else {
							break;
						}
					}
					stk.push(lextable.table[i]);
				}
			}
			break;
			case LEX_LEFTHESIS:
				stk.push(lextable.table[i]);
				break;

			case LEX_RIGHTHESIS:

				while (!stk.empty() && stk.top()->lexema != LEX_LEFTHESIS)
				{
					result.push(stk.top());
					stk.pop();
				}


				if (!stk.empty() && stk.top()->lexema == LEX_LEFTHESIS)
				{
					stk.pop(); // Выталкиваем '('

					if (function)
					{
						result.push(new LT::Entry('@'));
						result.push(new LT::Entry('0' + quantityParm));
						function = false;
					}
				}

				break;

			case LEX_COMMA:
				if (function)
				{
					quantityParm++;
					if (quantityParm > 9)
						throw ERROR_THROW_IN(707, lextable.table[i]->sn, lextable.table[i]->tn);
				}
				while (stk.top()->lexema != LEX_LEFTHESIS)
				{
					result.push(stk.top());
					stk.pop();
				}
				break;

			}
		}

		while (!stk.empty())
		{
			result.push(stk.top());
			stk.pop();
		}


		for (int j = lextable_pos; j < i; j++)
		{
			if (!result.empty())
			{
				lextable.table[j] = result.front();

				result.pop();
			}
			else
			{

				lextable.table[j] = new LT::Entry('#', lextable.table[j]->sn, lextable.table[j]->tn);
				lextable.table[j]->idxTI = LT_TI_NULLXDX;
			}
		}

		return true;
	}


	int findExpression(LT::LexTable& lextable)
	{
		for (static int i = 0; i < lextable.current_size; i++)
		{
			if (lextable.table[i]->lexema == LEX_EQUAL)
				return ++i;
		}
		return 0;
	}


}