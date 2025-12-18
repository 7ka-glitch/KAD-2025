#include "stdafx.h"
#include "SemanticAnaliz.h"
#include "Error.h"

using namespace std;

namespace SM
{
	void semAnaliz(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		for (int i = 0; i < lextable.current_size; i++)
		{
			switch (lextable.table[i]->lexema)
			{
			case LEX_OPERATOR:	
			{
				if (lextable.table[i]->sign == '^')
					if (lextable.table[i + 2]->idxTI != LT_TI_NULLXDX) 
						if ((long long)idtable.table[lextable.table[i + 2]->idxTI]->value.vint < 0) throw ERROR_THROW_IN(709, lextable.table[i]->sn, lextable.table[i]->tn);

				if (lextable.table[i]->sign == '/')
					if (idtable.table[lextable.table[i + 1]->idxTI]->value.vint == 0)
						throw ERROR_THROW_IN(700, lextable.table[i + 1]->sn, lextable.table[i + 1]->tn);


				bool isFirstOp = false;
				if (i > 0 && (lextable.table[i - 1]->lexema == LEX_ID || lextable.table[i - 1]->lexema == LEX_LITERAL)) {
					if (i < 2 || (lextable.table[i - 2]->lexema != LEX_OPERATOR)) {
						isFirstOp = true;
					}
				}

				if (isFirstOp) {
					
					int firstOpIdx = lextable.table[i - 1]->idxTI;
					if (firstOpIdx == LT_TI_NULLXDX) break;

					long long accumulator = (long long)idtable.table[firstOpIdx]->value.vint;

					
					for (int k = i; k < lextable.current_size; k++) {
						LT::Entry* curLex = lextable.table[k];

						
						if (curLex->lexema == LEX_SEMICOLON || curLex->lexema == LEX_RIGHTHESIS || curLex->lexema == LEX_COMMA)
							break;

						if (curLex->lexema == LEX_OPERATOR) {
							char op = curLex->sign;
							int nextIdx = lextable.table[k + 1]->idxTI;

							if (nextIdx == LT_TI_NULLXDX) break;
							if (idtable.table[nextIdx]->iddatatype != IT::IDDATATYPE::INT)
								break;
							
							long long nextVal = (long long)idtable.table[nextIdx]->value.vint;

							if (op == '+') accumulator += nextVal;
							else if (op == '-') accumulator -= nextVal;
							else if (op == '*') accumulator *= nextVal;
							else if (op == '/') {
								if (nextVal == 0) throw ERROR_THROW_IN(700, curLex->sn, curLex->tn);
								accumulator /= nextVal;
							}
							else if (op == '^') {
								if (nextVal < 0) throw ERROR_THROW_IN(709, curLex->sn, curLex->tn);
								long long base = accumulator;
								accumulator = 1;
								for (long long exp = 0; exp < nextVal; exp++) {
									accumulator *= base;
									if (accumulator > 2147483647LL || accumulator < -2147483648LL) {
										throw ERROR_THROW_IN(708, curLex->sn, curLex->tn);
									}
								}
							}

							
							if (accumulator > 2147483647LL || accumulator < -2147483648LL) {
								throw ERROR_THROW_IN(708, curLex->sn, curLex->tn);
							}
						}
					}
				}
					
				break;
			}
			case LEX_EQUAL: 
			{
				if(idtable.table[lextable.table[i + 1]->idxTI]->iddatatype== IT::IDDATATYPE::INT)
					if (idtable.table[lextable.table[i + 1]->idxTI]->value.vint > 2147483647 || idtable.table[lextable.table[i + 1]->idxTI]->value.vint < -2147483648)
						throw ERROR_THROW_IN(708, lextable.table[i+1]->sn, lextable.table[i+1]->tn);
				if (i > 0)
				{
					IT::IDDATATYPE lefttype = idtable.table[lextable.table[i - 1]->idxTI]->iddatatype;

					
					for (int k = i + 1; k < lextable.current_size && lextable.table[k]->lexema != LEX_SEMICOLON; k++)
					{
						char l = lextable.table[k]->lexema;

						
						
						if (lextable.table[k]->idxTI != LT_TI_NULLXDX)
						{
							IT::Entry* currentEntry = idtable.table[lextable.table[k]->idxTI];
							IT::IDDATATYPE righttype = currentEntry->iddatatype;

							
							if (lefttype != righttype)
							{
								
								throw ERROR_THROW_IN(701, lextable.table[k]->sn, lextable.table[k]->tn);
							}

							 
							if (k + 1 < lextable.current_size &&
								lextable.table[k + 1]->lexema == LEX_LEFTHESIS &&
								currentEntry->idtype == IT::IDTYPE::F)
							{
								int parenBalance = 1;  
								int funcEndIndex = k + 2; 

								 
								while (funcEndIndex < lextable.current_size)
								{
									char nextLexema = lextable.table[funcEndIndex]->lexema;

									if (nextLexema == LEX_LEFTHESIS)
										parenBalance++;
									else if (nextLexema == LEX_RIGHTHESIS)
										parenBalance--;

									if (parenBalance == 0)
									{
										 
										k = funcEndIndex;
										break;
									}

									funcEndIndex++;

									 
									if (funcEndIndex >= lextable.current_size || lextable.table[funcEndIndex]->lexema == LEX_SEMICOLON)
									{
										break;
									}
								}
							}
						}
 
						if (lefttype == IT::IDDATATYPE::STR)
						{
							if (l == LEX_OPERATOR)  
								throw ERROR_THROW_IN(702, lextable.table[k]->sn, lextable.table[k]->tn);
						}

						 
						if (lefttype == IT::IDDATATYPE::INT)
						{
							if (l == LEX_OPERATOR)
							{
								
								if (lextable.table[k - 1]->idxTI == LT_TI_NULLXDX || lextable.table[k + 1]->idxTI == LT_TI_NULLXDX)
								{
									continue;
								}

								IT::IDDATATYPE leftOp = idtable.table[lextable.table[k - 1]->idxTI]->iddatatype;
								IT::IDDATATYPE rightOp = idtable.table[lextable.table[k + 1]->idxTI]->iddatatype;

								if (leftOp != IT::IDDATATYPE::INT || rightOp != IT::IDDATATYPE::INT)
									throw ERROR_THROW_IN(701, lextable.table[k]->sn, lextable.table[k]->tn);


								
							}
							
							
							if (lextable.table[k]->idxTI != LT_TI_NULLXDX 
								&& (lextable.table[k]->lexema == LEX_ID || lextable.table[k]->lexema == LEX_LITERAL)) 
							{ 
								IT::IDTYPE t = idtable.table[lextable.table[k]->idxTI]->idtype; 
								if (t == IT::IDTYPE::V || t == IT::IDTYPE::L)
									idtable.table[lextable.table[i - 1]->idxTI]->value.vint = idtable.table[lextable.table[k]->idxTI]->value.vint;
							}
						}
					}
				}

				break;
			}

			case LEX_ID:  
			{
				IT::Entry* e;
				e = idtable.table[lextable.table[i]->idxTI];

				if (i && lextable.table[i - 1]->lexema == LEX_FUNCTION) 
				{
					for (int k = i + 1; ; k++)
					{
						char l = lextable.table[k]->lexema;
						if (l == LEX_RETURN)
						{
							int next = lextable.table[k + 1]->idxTI;
							if (next == LT_TI_NULLXDX) break;  

							if (idtable.table[next]->iddatatype != e->iddatatype)
								throw ERROR_THROW_IN(703, lextable.table[k + 1]->sn, lextable.table[k + 1]->tn);
							break;
						}
					}
				}
				if (lextable.table[i + 1]->lexema == LEX_LEFTHESIS && lextable.table[i - 1]->lexema != LEX_FUNCTION)  
				{
					if (e->idtype == IT::IDTYPE::F) 
					{
						int paramscount = 0;
						
						for (int j = i + 1; lextable.table[j]->lexema != LEX_RIGHTHESIS; j++)
						{
							
							if (lextable.table[j]->lexema == LEX_ID && lextable.table[j+1]->lexema != LEX_OPERATOR || lextable.table[j]->lexema == LEX_LITERAL && lextable.table[j+1]->lexema != LEX_OPERATOR)
							{
								paramscount++;
								IT::IDDATATYPE ctype = idtable.table[lextable.table[j]->idxTI]->iddatatype;
								if (paramscount > e->params.count)
									throw ERROR_THROW_IN(705, lextable.table[i]->sn, lextable.table[i]->tn);
								if (ctype != e->params.types[paramscount - 1])
									throw ERROR_THROW_IN(704, lextable.table[j]->sn, lextable.table[j]->tn);
							}
						}
						if (paramscount != e->params.count)
							throw ERROR_THROW_IN(705, lextable.table[i]->sn, lextable.table[i]->tn);
					}
				}
				break;
			}

			case LEX_CYCLE: 
			{
				for (int k = i + 1; lextable.table[k]->lexema != LEX_LEFTBRACE; k++)
				{
					if (lextable.table[k]->idxTI != LT_TI_NULLXDX) 
					{
						if (idtable.table[lextable.table[k]->idxTI]->iddatatype != IT::IDDATATYPE::INT)
							throw ERROR_THROW_IN(706, lextable.table[k]->sn, lextable.table[k]->tn);

						if(idtable.table[lextable.table[k]->idxTI]->value.vint < 0)
							throw ERROR_THROW_IN(706, lextable.table[k]->sn, lextable.table[k]->tn);
					}
				}
				break; 


			}
			}
		}
	}
}