#include <sstream>
#include "../Lexical/Scanner.h"


std::unordered_map<std::string, LexemeType> Scanner::keywords = {
	{"const", LexemeType::Const},
	{"switch", LexemeType::Switch},
	{"case", LexemeType::Case},
	{"break", LexemeType::Break},
	{"return", LexemeType::Return},
	{"default", LexemeType::Default},
	{"int", LexemeType::Int},
	{"bool", LexemeType::Bool},
	{"main", LexemeType::Main},
	{"true", LexemeType::ConstBool},
	{"false", LexemeType::ConstBool}
};

Scanner::Scanner(const std::istream& sourceStream)
{
	InputSourceText(sourceStream);
	curPos = sourceText.begin();
}

void Scanner::InputSourceText(const std::istream& sourceStream)
{
	std::stringstream sb;
	sb << sourceStream.rdbuf();
	auto source = sb.str();
	source.push_back(0);
	sourceText = source;
}



Lexeme Scanner::LookForward(int k)
{
	auto savePos = curPos;
	for (int i = 0; i < k - 1; i++)
		NextScan();
	auto lexeme = NextScan();
	curPos = savePos;
	return lexeme;
}



void Scanner::Scan(std::ostream& out)
{
	while (_lexeme.type != LexemeType::End) {
		SourceText::Iterator savePos;
		NextScan();
		out.width(9);
		out.flags(out.left);
		out << _lexeme.str << LexemeTypeToString(_lexeme.type) << " " << _lexeme.pos.row << ' ' << _lexeme.pos.column << std::endl;
	}

}


Lexeme Scanner::NextScan()
{
	_lexeme.str.clear();
	_lexeme.str.reserve(MAX_LEXEME_SIZE);

	SkipIgnoreChars();
	_lexeme.pos = curPos;

	auto savePos = GetCurPos();
	++curPos;
	auto nextChar = GetCurPos();
	SetCurPos(savePos);

	if (*curPos == 0) {
		_lexeme.type = LexemeType::End;
	}
	else if ('a' <= *curPos && *curPos <= 'z' ||
		'A' <= *curPos && *curPos <= 'Z' || '_' == *curPos)
		HandleStringWord();
	else if (('0' <= *curPos && *curPos <= '9') || (*curPos == '-' && *nextChar != ' ' && *nextChar != '-'))
		HandleDecNum();
	else {
		switch (*curPos)
		{
		case ',':
			NextChar();
			_lexeme.type = LexemeType::Comma;
			break;
		case ';':
			NextChar();
			_lexeme.type = LexemeType::Semi;
			break;
		case ':':
			NextChar();
			_lexeme.type = LexemeType::Colon;
			break;
		case '(':
			NextChar();
			_lexeme.type = LexemeType::OpenPar;
			break;
		case ')':
			NextChar();
			_lexeme.type = LexemeType::ClosePar;
			break;
		case '{':
			NextChar();
			_lexeme.type = LexemeType::OpenBrace;
			break;
		case '}':
			NextChar();
			_lexeme.type = LexemeType::CloseBrace;
			break;
		case '*':
			NextChar();
			_lexeme.type = LexemeType::Mul;
			break;
		case '/':
			NextChar();
			_lexeme.type = LexemeType::Div;
			break;
		case '%':
			NextChar();
			_lexeme.type = LexemeType::Modul;
			break;
		case '+':
			HandleDoubleChar(LexemeType::Add, '+', LexemeType::Inc);
			break;
		case '-':
			HandleDoubleChar(LexemeType::Sub, '-', LexemeType::Dec);
			break;
		case '>':
			HandleDoubleChar(LexemeType::G, '=', LexemeType::GE);
			if (_lexeme.type == LexemeType::G)
				HandleDoubleChar(LexemeType::G, '>', LexemeType::ShiftR);
			break;
		case '<':
			HandleDoubleChar(LexemeType::L, '=', LexemeType::LE);
			if (_lexeme.type == LexemeType::L)
				HandleDoubleChar(LexemeType::L, '<', LexemeType::ShiftL);
			break;
		case '=':
			HandleDoubleChar(LexemeType::Assign, '=', LexemeType::E);
			break;
		case '!':
			HandleDoubleChar(LexemeType::Not, '=', LexemeType::NE);
			break;
		case '|':
			HandleDoubleChar(LexemeType::Err, '|', LexemeType::Or);
			break;
		case '&':
			HandleDoubleChar(LexemeType::Err, '&', LexemeType::And);
			break;
		default:
			NextChar();
			_lexeme.type = LexemeType::Err;
		}
	}

	return _lexeme;
}


void Scanner::SkipIgnoreChars()
{
	while (curPos != sourceText.end())
	{
		switch (*curPos)
		{
		case '/': {
			auto tmpPos = curPos;
			++tmpPos;
			if (*tmpPos != '/')
				return;
			SkipComment();
			break;
		}
		case '\n': case '\r': case '\t': case ' ':
			++curPos;
			break;
		default:
			return;
		}
	}
}

void Scanner::SkipComment()
{
	++curPos;
	while (*curPos != 0 && *curPos != '\n')
		++curPos;
}

void Scanner::HandleStringWord()
{
	NextChar();
	while ('a' <= *curPos && *curPos <= 'z'
		|| 'A' <= *curPos && *curPos <= 'Z'
		|| '0' <= *curPos && *curPos <= '9'
		|| '_' == *curPos)
	{
		if (!NextChar()) {
			HandleErrWord();
			return;
		}
	}
	auto keywordIt = keywords.find(_lexeme.str);
	if (keywordIt != keywords.end())
		_lexeme.type = keywordIt->second;
	else
		_lexeme.type = LexemeType::Id;
}

void Scanner::HandleDecNum()
{
	NextChar();
	if (('0' <= *curPos && *curPos <= '9') || *curPos == '-')
	{
		if (!NextChar())
			return HandleErrWord();

		while ('0' <= *curPos && *curPos <= '9')
			if (!NextChar())
				return HandleErrWord();
	}
	if (*curPos == 'l' || *curPos == 'L')
		NextChar();
	if ('a' <= *curPos && *curPos <= 'z' || 'A' <= *curPos && *curPos <= 'Z' || '_' == *curPos)
		return HandleErrWord();

	_lexeme.type = LexemeType::ConstInt;
}

void Scanner::HandleErrWord()
{
	while ('a' <= *curPos && *curPos <= 'z' || 'A' <= *curPos && *curPos <= 'Z'
		|| '0' <= *curPos && *curPos <= '9' || '_' == *curPos)
	{
		NextChar();
	}
	_lexeme.type = LexemeType::Err;
}

void Scanner::HandleDoubleChar(LexemeType firstLexeme, char nextChar, LexemeType secondLexeme)
{
	NextChar();
	if (*curPos == nextChar)
	{
		NextChar();
		_lexeme.type = secondLexeme;
	}
	else
		_lexeme.type = firstLexeme;
}

bool Scanner::NextChar()
{
	const bool isLexemeOverflow = _lexeme.str.size() > MAX_LEXEME_SIZE;
	if (!isLexemeOverflow)
		_lexeme.str.push_back(*curPos);
	++curPos;
	return !isLexemeOverflow;
}