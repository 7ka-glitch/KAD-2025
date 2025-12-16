#include "stdafx.h"
#include <stack>

#include "LexAnalysis.h"
#include "Error.h"
#include "FST.h"
#include "graphs.h"

using namespace std;

// [FIX] Добавлен аргумент int& i, чтобы управлять позицией чтения извне
bool FindLexema(In::IN& in, char* lexema, int& i);

void LexAnalysis(In::IN& in, LT::LexTable& lex, IT::IdTable& id)
{
	IT::IDDATATYPE	iddatatype = IT::IDDATATYPE::DEF;
	IT::IDTYPE	idtype = IT::IDTYPE::D;
	IT::Entry* IT_ENTRY = NULL;

	IT::len.params = IT::Entry::Param{ 1, vector<IT::IDDATATYPE>{ IT::IDDATATYPE::STR} };
	IT::Add(id, &IT::len);

	IT::intToStr.params = IT::Entry::Param{ 1, vector<IT::IDDATATYPE>{ IT::IDDATATYPE::INT} };
	IT::Add(id, &IT::intToStr);
	ID_RESET

		char lexema[TI_STR_MAXSIZE];
	int currentRow = 1, currentLex = 0, indexIdTable = 2;
	bool lexID = true, lexInt = false, lexComment = false;
	int main = 0;

	// [FIX] Переменная i теперь здесь, а не static внутри FindLexema
	int i = 0;

	stack<string> areaOfVisibility;
	areaOfVisibility.push(GLOBAL);

	// [FIX] Передаем i в функцию
	while (FindLexema(in, lexema, i))
	{
		currentLex++;
		lexID = true; lexInt = false;

		switch (*lexema)
		{
		case LEX_RIGHTBRACE:
		case LEX_SEMICOLON:
		case LEX_COMMA:
		case LEX_LEFTBRACE:
		case LEX_LEFTHESIS:
		case LEX_RIGHTHESIS:
		case LEX_EQUAL:
		{
			// [FIX] Добавлена защита от обработки символов внутри комментария (если вдруг логика пропуска даст сбой)
			if (!lexComment)
			{
				LT::Add(lex, new LT::Entry(*lexema, currentRow, currentLex));
				lexID = false;
				if (*lexema == LEX_RIGHTBRACE && !areaOfVisibility.empty())
					areaOfVisibility.pop();
			}
			break;
		}
		case IN_CODE_VERTICAL_LINE:
			currentRow++;
			currentLex = 0;
			lexID = false;
			if (lexComment)
			{
				lexComment = false;
			}
			break;

		case '#':
		{
			// [FIX] Радикальное решение для комментариев
			// Мы просто пропускаем весь текст до символа новой строки прямо в буфере
			lexID = false; // Сам символ # не является ID
			lexComment = false; // Флаг нам больше не нужен, мы физически пропускаем текст

			// Бежим по тексту, пока не найдем вертикальную черту (конец строки) или конец файла
			while (i < in.size && in.text[i] != IN_CODE_VERTICAL_LINE)
			{
				i++;
			}
			// Следующий вызов FindLexema начнет чтение уже с новой строки (вертикальной черты)
			break;
		}

		case 'f':
		{
			FST::FST fst_function = FST_FUNCTION(lexema);
			if (FST::execute(fst_function) && !lexComment)
			{
				LT::Add(lex, new LT::Entry(LEX_FUNCTION, currentRow, currentLex));
				idtype = IT::IDTYPE::F;
				lexID = false;
			}
			break;
		}
		// ... (остальной код без изменений, так как lexComment теперь сбрасывается корректно) ...
		case 'i':
		{
			FST::FST fst_int = FST_INT(lexema);
			if (FST::execute(fst_int) && !lexComment)
			{
				LT::Add(lex, new LT::Entry(LEX_INT, currentRow, currentLex));
				iddatatype = IT::IDDATATYPE::INT;
				idtype = IT::IDTYPE::V;
				lexID = false;
			}
			break;
		}
		case 'm':
		{
			FST::FST fst_main = FST_MAIN(lexema);
			if (FST::execute(fst_main) && !lexComment)
			{
				main++;
				LT::Add(lex, new LT::Entry(LEX_MAIN, currentRow, currentLex));
				areaOfVisibility.push(lexema);
				lexID = false;
			}
			break;
		}
		case 'p':
		{
			FST::FST fst_param = FST_PARAM(lexema);
			if (FST::execute(fst_param) && !lexComment)
			{
				LT::Add(lex, new LT::Entry(LEX_PARAM, currentRow, currentLex));
				idtype = IT::IDTYPE::P;
				if (IT_ENTRY)
				{
					IT_ENTRY->params.count++;
					IT_ENTRY->params.types.push_back(iddatatype);
				}
				lexID = false;
			}
			break;
		}

		case 'r':
		{
			FST::FST fst_return = FST_RETURN(lexema);
			if (FST::execute(fst_return) && !lexComment)
			{
				LT::Add(lex, new LT::Entry(LEX_RETURN, currentRow, currentLex));
				lexID = false;
			}
			break;
		}
		case 'c':
		{
			FST::FST fst_var = FST_CYCLE(lexema);
			if (FST::execute(fst_var) && !lexComment)
			{
				char str[10];
				static int count = 0;
				sprintf(str, "%d", count++);
				LT::Add(lex, new LT::Entry(LEX_CYCLE, currentRow, currentLex));
				areaOfVisibility.push(strcat(lexema, str));
				lexID = false;
			}
			break;
		}
		case 's':
		{
			FST::FST fst_string = FST_STR(lexema);
			if (FST::execute(fst_string) && !lexComment)
			{
				LT::Add(lex, new LT::Entry(LEX_STR, currentRow, currentLex));
				iddatatype = IT::IDDATATYPE::STR;
				idtype = IT::IDTYPE::V;
				lexID = false;
			}
			break;
		}
		case 'h':
		{
			FST::FST fst_literal_i16 = FST_LITERAL_I16(lexema);
			if (FST::execute(fst_literal_i16) && !lexComment)
			{
				lexID = false; lexInt = true;
				BaseToDecimal(lexema, 16);
			}
			break;
		}
		case 'w':
		{
			FST::FST fst_write = FST_WRITE(lexema);
			if (FST::execute(fst_write) && !lexComment)
			{
				LT::Add(lex, new LT::Entry(LEX_WRITE, currentRow, currentLex));
				lexID = false;
			}
			break;
		}
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
		{
			FST::FST fst_literal_minus_i = FST_LITERAL_MINUS_I(lexema);
			if (FST::execute(fst_literal_minus_i) && !lexComment)
			{
				lexID = false; lexInt = true;
			}
			if (lexID)
			{
				FST::FST fst_operator = FST_OPERATOR(lexema);
				if (FST::execute(fst_operator) && !lexComment)
				{
					LT::Add(lex, new LT::Entry(LEX_OPERATOR, *lexema, currentRow, currentLex));
					lexID = false;
				}
			}
			break;
		}
		case IN_CODE_QUOTES:
		{
			FST::FST fst_literal_s = FST_LITERAL_S(lexema);
			if (FST::execute(fst_literal_s) && !lexComment)
			{
				lexID = false;
				int IsLiteralString = IT::IsLiteralString(id, lexema);
				if (IsLiteralString + 1)
					LT::Add(lex, new LT::Entry(LEX_LITERAL, currentRow, currentLex, IsLiteralString));
				else
				{
					LT::Add(lex, new LT::Entry(LEX_LITERAL, currentRow, currentLex, indexIdTable++));

					LITERAL_STR
						IT::Add(id, new IT::Entry(lex.current_size - 1, iddatatype, idtype, lexema));
					ID_RESET
				}
			}
			break;
		}
		default:
		{
			FST::FST fst_literal_i = FST_LITERAL_I(lexema);
			if (FST::execute(fst_literal_i) && !lexComment) // Добавил !lexComment
			{
				lexID = false; lexInt = true;
			}
			break;
		}
		}

		if (lexInt && !lexComment)
		{
			int IsLiteralInt = IT::IsLiteralInt(id, lexema);
			if (IsLiteralInt + 1)
				LT::Add(lex, new LT::Entry(LEX_LITERAL, currentRow, currentLex, IsLiteralInt));
			else
			{
				LT::Add(lex, new LT::Entry(LEX_LITERAL, currentRow, currentLex, indexIdTable++));

				LITERAL_INT
					IT::Add(id, new IT::Entry(lex.current_size - 1, iddatatype, idtype, lexema));
				ID_RESET
			}
		}

		if (lexID && !lexComment)
		{
			FST::FST fst_id = FST_ID(lexema);
			if (FST::execute(fst_id))
			{
				int isId = IT::IsId(id, lexema, areaOfVisibility);
				if (isId + 1)
				{
					if (idtype != IT::IDTYPE::D)
					{
						if (!strcmp(id.table[isId]->areaOfVisibility, areaOfVisibility.top().c_str()))
							throw ERROR_THROW_IN(131, currentRow, currentLex);
						if (iddatatype == IT::IDDATATYPE::DEF)
							throw ERROR_THROW_IN(121, currentRow, currentLex);

						LT::Add(lex, new LT::Entry(LEX_ID, currentRow, currentLex, indexIdTable++));
						IT::Add(id, new IT::Entry(lex.current_size - 1, areaOfVisibility.top(), lexema, iddatatype, idtype));

						ID_RESET
					}
					else
						LT::Add(lex, new LT::Entry(LEX_ID, currentRow, currentLex, isId));
				}
				else
				{
					if (iddatatype == IT::IDDATATYPE::DEF)
						throw ERROR_THROW_IN(121, currentRow, currentLex);
					if (idtype == IT::IDTYPE::D)
						throw ERROR_THROW_IN(132, currentRow, currentLex);

					LT::Add(lex, new LT::Entry(LEX_ID, currentRow, currentLex, indexIdTable++));
					IT::Add(id, new IT::Entry(lex.current_size - 1, areaOfVisibility.top(), lexema, iddatatype, idtype));

					if (idtype == IT::IDTYPE::F)
					{
						areaOfVisibility.push(lexema);
						IT_ENTRY = id.table[id.current_size - 1];
					}

					ID_RESET
				}
			}
			else
				throw ERROR_THROW_IN(120, currentRow, currentLex);
		}
	}

	if (main == 0)
		throw ERROR_THROW(133);
	if (main > 1)
		throw ERROR_THROW(134);
	LT::Add(lex, new LT::Entry('$', currentRow, currentLex));
}

// [FIX] Убрана static int i, теперь позиция передается по ссылке извне
bool FindLexema(In::IN& in, char* lexema, int& i)
{
	bool s_literal = false;
	int indexLexema = 0;
	// Цикл использует переданную i
	for (; i < in.size && (in.text[i] != IN_CODE_SPACE || s_literal); i++)
	{
		if (indexLexema >= TI_STR_MAXSIZE - 1)
			throw ERROR_THROW(125)

			lexema[indexLexema++] = in.text[i];
		if (in.text[i] == IN_CODE_QUOTES)
			s_literal = !s_literal;
	}
	lexema[indexLexema] = TI_STR_DEFAULT;

	// Важно: если мы остановились на пробеле, нужно его перешагнуть, 
	// чтобы следующий вызов не застрял на нем.
	// Но если это конец файла, перешагивать нельзя.
	if (i < in.size && in.text[i] == IN_CODE_SPACE) {
		i++;
	}

	// Возвращаем true, если удалось считать хоть что-то
	return indexLexema > 0 || (i < in.size);
}

void BaseToDecimal(char* lexema, int base)
{
	int number = 0;
	int k;
	for (int i = 1; lexema[i] != '\0'; i++)
	{
		if (lexema[i] <= '9' && lexema[i] >= '0') k = lexema[i] - '0';
		else if (lexema[i] >= 'A' && lexema[i] <= 'F') k = lexema[i] - 'A' + 10;
		else if (lexema[i] >= 'a' && lexema[i] <= 'f') k = lexema[i] - 'a' + 10;
		else continue;
		number = base * number + k;
	}
	sprintf(lexema, "%d", number);
}
