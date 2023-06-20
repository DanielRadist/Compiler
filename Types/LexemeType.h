#pragma once
#include <map>
#include <string>

enum class LexemeType : unsigned int
{
	Int = 2,
	Short = 3,
	Long = 4,
	Bool = 5,
	Main = 6,
	Id = 10,

	Const = 18,							// ключевое слово константа
	ConstBool = 19,
	ConstInt = 20,
	//ConstShort = 21,
	//ConstLong = 22,

	Colon = 29,
	Comma = 30,
	Semi = 31,
	OpenPar = 32,
	ClosePar = 33,
	OpenBrace = 34,
	CloseBrace = 35,
	Assign = 40,						// =
	E = 41,								// ==
	NE = 42,							// !=
	G = 43,								// >
	L = 44,								// <
	LE = 45,							// <=
	GE = 46,							// >=
	Add = 47,
	Sub = 48,
	Mul = 49,
	Div = 50,
	Modul = 51,
	Inc = 52,
	Dec = 53,
	Switch = 54,
	Case = 55,
	Break = 56,
	Return = 57,
	Default = 58,

	ShiftL = 59,						// <<
	ShiftR = 60,						// >>

	Or = 61,
	And = 62,
	Not = 63,

	TypeOperatorNonTerm = 80,				// оператор
	TypeOperatorSimpleNonTerm = 81,			// простой оператор
	TypeOperatorCompositNonTerm = 82,		// составной оператор

	TypeOperatorSetNonTerm = 83,			// оператор присваивания
	TypeOperatorSwitchNonTerm = 84,			// оператор switch
	TypeOperatorReturnNonTerm = 85,			// оператор присваивания

	TypeBodyOperatorCompositNonTerm = 86,	// тело составного оператора

	TypeCasesNonTerm = 87,					// кейсы свича
	TypeCaseNonTerm = 88,					// кейс свича
	TypeExprCaseNonTerm = 89,				// выражения кейса
	TypeDefaultNonTerm = 90,				// деволт свича

	TypeProgNonTerm = 100,					// программа
	TypeDescriptionNonTerm = 101,			// описание

	TypeVarsDeclarationNonTerm = 102,		// объявление переменных
	TypeFuncDeclarationNonTerm = 103,		// объявление функции
	TypeMainDeclarationNonTerm = 104,		// объявление функции main
	TypeParamsListDeclNonTerm = 105,		// список формальных параметров у функции
	TypeEndofParamsListDeclNonTerm = 106,
	TypeFuncCallNonTerm = 107,				// вызов функции
	TypeParamsListExprNonTerm = 108,		// список фактических параметров
	TypeEndofParamsListExprNonTerm = 109,

	TypeConstDeclarationNonTerm = 114,		// именованная константа
	TypeDataNonTerm = 115,					// блок данных
	TypeTypeNonTerm = 116,					// тип данных
	TypeVarsListNonTerm = 117,				// список идентификаторов
	//
	TypeInitVarNonTerm = 119,				// возможная инициализация
	TypeEndofVarsListNonTerm = 120,			// хвост списка идентификаторов

	TypeExprNonTerm = 200,					// выражение
	TypeEndofExprNonTerm = 201,

	TypeOrNonTerm = 202,
	TypeEndofOrNonTerm = 203,

	TypeAndNonTerm = 204,
	TypeEndofAndNonTerm = 205,

	TypeComparisonNonTerm = 206,
	TypeEndofComparisonNonTerm = 207,
	TypeComparisonSignNonTerm = 208,

	TypeShiftNonTerm = 209,
	TypeEndofShiftNonTerm = 210,
	TypeSHiftSignNonTerm = 211,

	TypeAddSubNonTerm = 212,
	TypeEndofAddSubNonTerm = 213,
	//TypeAddSubSignNonTerm = 214,

	TypeMultDivNonTerm = 215,
	TypeEndofMultDivNonTerm = 216,
	//TypeMultDivSignNonTerm = 217,

	TypeNotNonTerm = 218,
	TypePostFixNonTerm = 219,
	TypePreFixNonTerm = 220,
	TypeEndofPostFixNonTerm = 221,
	TypeIncDecSignNonTerm = 222,
	TypeBasicExprNonTerm = 223,
	TypeConstNonTerm = 224,

	TypeEndComma = 299,
	End = 300,
	Err = 400
};

inline std::string LexemeTypeToString(LexemeType code) {
	static std::map<LexemeType, std::string> lexicalStrings = {
		{LexemeType::Int, "Int"},
		{LexemeType::Short, "Short"},
		{LexemeType::Long, "Long"},
		{LexemeType::Bool, "Bool"},
		{LexemeType::Main, "Main"},
		{LexemeType::Id, "Id"},

		{LexemeType::Const, "Const"},
		{LexemeType::ConstInt, "ConstInt"},
		{LexemeType::ConstBool, "ConstBool"},
		//{LexemeType::ConstShort, "ConstShort"},
		//{LexemeType::ConstLong, "ConstLong"},

		{LexemeType::Colon, "Colon"},
		{LexemeType::Comma, "Comma"},
		{LexemeType::Semi, "Semi"},
		{LexemeType::OpenPar, "OpenPar"},
		{LexemeType::ClosePar, "ClosePar"},
		{LexemeType::OpenBrace, "OpenBrace"},
		{LexemeType::CloseBrace, "CloseBrace"},
		{LexemeType::Assign, "Assign"},
		{LexemeType::E, "E"},
		{LexemeType::NE, "NE"},
		{LexemeType::G, "G"},
		{LexemeType::L, "L"},
		{LexemeType::LE, "LE"},
		{LexemeType::GE, "GE"},
		{LexemeType::Add, "Plus"},
		{LexemeType::Sub, "Minus"},
		{LexemeType::Mul, "Mul"},
		{LexemeType::Div, "Div"},
		{LexemeType::Modul, "Modul"},
		{LexemeType::Inc, "Inc"},
		{LexemeType::Dec, "Dec"},
		{LexemeType::Switch, "Switch"},
		{LexemeType::Case, "Case"},
		{LexemeType::Break, "Break"},
		{LexemeType::Return, "Return"},
		{LexemeType::Default, "Default"},
		{LexemeType::End, "End"},
		{LexemeType::Err, "Err"},
	};
	
	return lexicalStrings.at(code);
}



