#include "../Syntaxes/SyntaxAnalyser.h"
#include "../Exceptions/AnalysisExceptions.h"
#include "../Types/DataType.h"

void SyntaxAnalyser::run()
{
    int flag = 1;
    
    magazine->getTopPtr()->lex.type = LexemeType::End;
    magazine->getTopPtr()->term = true;
    magazine->ptrUp();
   
    magazine->getTopPtr()->lex.type = LexemeType::TypeProgNonTerm;
    magazine->getTopPtr()->term = false;
    magazine->ptrUp();

    Lexeme lex = scanner->NextScan();
    while (flag)
    {
        if (magazine->getTop(-1).term)
        {
            if (magazine->getTop(-1).lex.type == lex.type)
            {
                if (lex.type == LexemeType::End)
                    flag = 0;
                else
                {
                    lex = scanner->NextScan();
                    magazine->ptrDown();
                }
            }
            else
                throw NotExpectedLexemeException(magazine->getTop(-1).lex.str, lex);
        }
        else
            this->analyzeNonTerm(lex);
    }
}

bool SyntaxAnalyser::isTypeData(const Lexeme& lex)
{
    if (lex.type == LexemeType::Bool ||
        lex.type == LexemeType::Int ||
        lex.type == LexemeType::Short ||
        lex.type == LexemeType::Long)
        return true;
    else
        return false;
}

bool SyntaxAnalyser::isComparisonSign(const Lexeme& lex)
{
    if (lex.type == LexemeType::E || lex.type == LexemeType::NE ||
        lex.type == LexemeType::G || lex.type == LexemeType::GE ||
        lex.type == LexemeType::L || lex.type == LexemeType::LE)
        return true;
    else
        return false;
}

bool SyntaxAnalyser::isConst(const Lexeme& lex)
{
    if (lex.type == LexemeType::ConstInt || lex.type == LexemeType::ConstBool)
        return true;
    else
        return false;
}

void SyntaxAnalyser::epsilon()
{
    magazine->ptrDown();
}

void SyntaxAnalyser::analyzeNonTerm(Lexeme lex)
{
    if (lex.str == "return")
        int a = 5;
    switch (magazine->getTop(-1).lex.type)
    {
    case LexemeType::TypeProgNonTerm:                                   // программа
        if (isTypeData(lex) || lex.type == LexemeType::Const)           // * описание
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeProgNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeDescriptionNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::End)                           // * пусто
            epsilon();
        else
            throw ExpectedExpressionException(lex);             
        break;

    case LexemeType::TypeDescriptionNonTerm:                            // описание
        if (lex.type == LexemeType::Const)                              // * константа
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::TypeConstDeclarationNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (isTypeData(lex))                                       // * объявление функции или блок данных
        {
            auto lexNext = scanner->LookForward(1);
            if (lexNext.type == LexemeType::Main)                       // * * обяъвление main()
            {
                magazine->ptrDown();

                magazine->getTopPtr()->lex.type = LexemeType::TypeOperatorNonTerm;  // оператор
                magazine->getTopPtr()->term = false;
                magazine->ptrUp();
                
                magazine->getTopPtr()->lex.type = LexemeType::ClosePar; // )
                magazine->getTopPtr()->term = true;
                magazine->ptrUp();

                magazine->getTopPtr()->lex.type = LexemeType::OpenPar;  // (
                magazine->getTopPtr()->term = true;
                magazine->ptrUp();

                magazine->getTopPtr()->lex.type = LexemeType::Main;     // идентификатор (имя функции: main)
                magazine->getTopPtr()->term = true;
                magazine->ptrUp();

                magazine->getTopPtr()->lex.type = LexemeType::TypeTypeNonTerm;  // тип
                magazine->getTopPtr()->term = false;
                magazine->ptrUp();
            }   
            else if (lexNext.type == LexemeType::Id &&                  // * * объявление функции
                scanner->LookForward(2).type == LexemeType::OpenPar)            
            {
                magazine->ptrDown();

                magazine->getTopPtr()->lex.type = LexemeType::TypeOperatorNonTerm;  // оператор
                magazine->getTopPtr()->term = false;
                magazine->ptrUp();

                magazine->getTopPtr()->lex.type = LexemeType::ClosePar; // )
                magazine->getTopPtr()->term = true;
                magazine->ptrUp();

                if (scanner->LookForward(3).type != LexemeType::ClosePar)
                {
                    magazine->getTopPtr()->lex.type = LexemeType::TypeParamsListDeclNonTerm;  // список формальных парамметров
                    magazine->getTopPtr()->term = false;
                    magazine->ptrUp();
                }

                magazine->getTopPtr()->lex.type = LexemeType::OpenPar;  // (
                magazine->getTopPtr()->term = true;
                magazine->ptrUp();

                magazine->getTopPtr()->lex.type = LexemeType::Id;       // идентификатор
                magazine->getTopPtr()->term = true;
                magazine->ptrUp();

                magazine->getTopPtr()->lex.type = LexemeType::TypeTypeNonTerm;  // тип
                magazine->getTopPtr()->term = false;
                magazine->ptrUp();
            }
            else if (lexNext.type == LexemeType::Id)                    // * * блок данных
            {
                magazine->ptrDown();
                magazine->getTopPtr()->lex.type = LexemeType::TypeDataNonTerm;
                magazine->getTopPtr()->term = false;
                magazine->ptrUp();
            }
            else
                throw NotExpectedLexemeException("объявление функции или переменной", lex);         
        }
        else
            throw NotExpectedLexemeException("константа, объявление / использование функции или переменной", lex);
        break;

    case LexemeType::TypeParamsListDeclNonTerm:                         // список формальных параметров
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofParamsListDeclNonTerm;  // хвост
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Id;               // идентификатор
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeTypeNonTerm;  // тип данных
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();
        break;

    case LexemeType::TypeEndofParamsListDeclNonTerm:                    // хвост списка формальных параметров
        if (lex.type == LexemeType::Comma)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofParamsListDeclNonTerm;  // хвост
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Id;           // идентификатор
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeTypeNonTerm;// тип данных
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Comma;        // ,
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::ClosePar)
            epsilon();
        else
            throw NotExpectedLexemeException("формальный параметр функции или < ) >", lex);

        break;

    case LexemeType::TypeParamsListExprNonTerm:                         // список фактических параметров
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofParamsListExprNonTerm;  // хвост
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;  // выражение
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();
        break;

    case LexemeType::TypeEndofParamsListExprNonTerm:                    // хвост списка фактических параметров
        if (lex.type == LexemeType::Comma)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofParamsListExprNonTerm;  // хвост
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;  // выражение
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Comma;        // ,
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::ClosePar)
            epsilon();
        else
            throw NotExpectedLexemeException("фактический параметр функции или < ) >", lex);
        break;

    case LexemeType::TypeDataNonTerm:                                   // блок данных
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeVarsListNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeTypeNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        break;

    case LexemeType::TypeVarsListNonTerm:                               // список идентификаторов
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofVarsListNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeInitVarNonTerm;// возможная инициализация
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Id;               // идентификатор
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();
        break;

    case LexemeType::TypeEndofVarsListNonTerm:                          // хвост списка идентификаторов
        if (lex.type == LexemeType::Comma)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofVarsListNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeInitVarNonTerm;// возможная инициализация
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Id;           // идентификатор
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Comma;        // ,
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Semi)                          // ;
            epsilon();
        else
            throw NotExpectedLexemeException("символ < , > или < ; >", lex);

        break;

    case LexemeType::TypeInitVarNonTerm:                                // возможная инициализация
        if (lex.type == LexemeType::Assign)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Assign;       // =
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Semi || lex.type == LexemeType::Comma)
            epsilon();
        else
            throw NotExpectedLexemeException("символ < , > или < ; > или инициализация переменной", lex);

        break;

    case LexemeType::TypeConstDeclarationNonTerm:                       // объявление константы
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();        

        magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;  // выражение
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Assign;           // присвоение
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();        

        magazine->getTopPtr()->lex.type = LexemeType::Id;               // идентификатор
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeTypeNonTerm;  // тип данных константы
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Const;            // ключевое слово
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();
        break;

    case LexemeType::TypeTypeNonTerm:
        if (lex.type == LexemeType::Int)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::Int;
            magazine->getTopPtr()->term = true;        
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Short)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::Short;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Long)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::Long;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Bool)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::Bool;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else
            throw InvalidTypeException(lex.str);
        break;

    case LexemeType::TypeOperatorNonTerm:                                   // оператор
        if (lex.type == LexemeType::Semi)                                   // пустой оператор
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::OpenBrace)                         // составной оператор
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::CloseBrace;       // }
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeBodyOperatorCompositNonTerm;  // тело составного оператора
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::OpenBrace;        // {
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::TypeOperatorSimpleNonTerm;  // простой оператор
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        break;

    case LexemeType::TypeOperatorSimpleNonTerm:                             // простой оператор
        if (lex.type == LexemeType::Switch)                                 // оператор switch
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::CloseBrace;       // }
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeCasesNonTerm; // кейсы
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::OpenBrace;        // {
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::ClosePar;         // )
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;  // выражение
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::OpenPar;          // (
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Switch;           // switch
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Return)                            // возврат из функции
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;  // выражение
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Return;           // return
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Id &&
            scanner->LookForward(1).type == LexemeType::Assign)             // присвоение к переменной  TODO:
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;  // выражение
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Assign;           // =
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Id;               // идентификатор переменной
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Id &&
            scanner->LookForward(1).type == LexemeType::OpenPar)            // вызов функции
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeFuncCallNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();            
        }
        else if (lex.type == LexemeType::Id || isConst(lex))                // выражение "выполняемое в пустую"
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Inc || lex.type == LexemeType::Dec)
        {
            int a = 5;
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else
            throw NotExpectedLexemeException("простой оператор (switch, return, \"присвоение\", \"вызов функции\")", lex);

        break;

    case LexemeType::TypeCasesNonTerm:                                      // кейсы
        if (lex.type == LexemeType::Case)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::TypeCasesNonTerm; // 
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeCaseNonTerm;  // 
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Default)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::TypeDefaultNonTerm;// 
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else
            throw NotExpectedLexemeException("оператор case или default или выражение case\'a", lex);
        break;

    case LexemeType::TypeCaseNonTerm:                                       // кейс
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeExprCaseNonTerm;  // выражение кейса
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Colon;                // :
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeConstNonTerm;     // константа
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Case;                 // case
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();
        break;

    case LexemeType::TypeDefaultNonTerm:                                    // оператор default
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeExprCaseNonTerm;  // выражение кейса
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Colon;                // :
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Default;              // default
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();
        break;

    case LexemeType::TypeExprCaseNonTerm:                                   // выражение кейса
        if (lex.type == LexemeType::Break)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprCaseNonTerm;// выражение кейса
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Semi;             // ;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Break;            // break
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Id || isConst(lex) || 
            lex.type == LexemeType::Return)                // оператор
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprCaseNonTerm;// выражение кейса
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeOperatorNonTerm;// оператор
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else
            epsilon();
        break;

    case LexemeType::TypeFuncCallNonTerm:
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::ClosePar;             // )
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();

        if (scanner->LookForward(2).type != LexemeType::ClosePar)           // вызов c параметрами
        {
            magazine->getTopPtr()->lex.type = LexemeType::TypeParamsListExprNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }

        magazine->getTopPtr()->lex.type = LexemeType::OpenPar;              // (
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::Id;                   // идентификатор функции
        magazine->getTopPtr()->term = true;
        magazine->ptrUp();
        break;

    case LexemeType::TypeBodyOperatorCompositNonTerm:                       // тело составного оператора
        if (lex.type == LexemeType::Const)                                  // именованная константа и далее тело оператора
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeBodyOperatorCompositNonTerm;  // тело составного оператора
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeConstDeclarationNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (isTypeData(lex))                                           // блок данных и далее тело оператора
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeBodyOperatorCompositNonTerm;  // тело составного оператора
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeDataNonTerm;  // блок данных
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Id || isConst(lex))                // выражение или константы
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeBodyOperatorCompositNonTerm;  // тело составного оператора
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeOperatorNonTerm;// оператор
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type != LexemeType::CloseBrace)                        // оператор и далее тело оператора
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeBodyOperatorCompositNonTerm;  // тело составного оператора
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeOperatorNonTerm;// оператор
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else
            epsilon();

        break;

    case LexemeType::TypeExprNonTerm:                                       // выражение
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofExprNonTerm; // хвост выражения
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeAndNonTerm;       // AND
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        break;

    case LexemeType::TypeEndofExprNonTerm:                                  // хвост выражения
        if (lex.type == LexemeType::Or)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofExprNonTerm; // хвост выражения
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeAndNonTerm;   // AND
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Or;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Comma || lex.type == LexemeType::Semi || lex.type == LexemeType::ClosePar)
            epsilon();
        else
            throw NotExpectedLexemeException("оператор логического \"или\"", lex);
        break;

    case LexemeType::TypeAndNonTerm:
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofAndNonTerm;  // хвост And
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeComparisonNonTerm;// сравнение
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        break;

    case LexemeType::TypeEndofAndNonTerm:                                   // хвост And
        if (lex.type == LexemeType::And)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofAndNonTerm;  // хвост And
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeComparisonNonTerm;// сравнение
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::And;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Comma || lex.type == LexemeType::Semi || lex.type == LexemeType::ClosePar ||
            lex.type == LexemeType::Or)
            epsilon();
        else
            throw NotExpectedLexemeException("оператор логического \"и\"", lex);
        break;

    case LexemeType::TypeComparisonNonTerm:                                 // сравнение
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofComparisonNonTerm;// хвост сравнения
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeShiftNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        break;

    case LexemeType::TypeEndofComparisonNonTerm:                            // хвост сравнения
        if (isComparisonSign(lex))                                          // если оператор сравнения
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofComparisonNonTerm;// хвост сравнения 
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeShiftNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeComparisonSignNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Comma || lex.type == LexemeType::Semi || lex.type == LexemeType::ClosePar ||
            lex.type == LexemeType::And || lex.type == LexemeType::Or)
            epsilon();
        else
            throw NotExpectedLexemeException("оператор сравнения", lex);
        break;

    case LexemeType::TypeComparisonSignNonTerm:                             // оператор сравнения
        magazine->ptrDown();

        if (lex.type == LexemeType::E)
            magazine->getTopPtr()->lex.type = LexemeType::E;

        else if (lex.type == LexemeType::NE)
            magazine->getTopPtr()->lex.type = LexemeType::NE;

        else if (lex.type == LexemeType::G)
            magazine->getTopPtr()->lex.type = LexemeType::G;

        else if (lex.type == LexemeType::GE)
            magazine->getTopPtr()->lex.type = LexemeType::GE;

        else if (lex.type == LexemeType::L)
            magazine->getTopPtr()->lex.type = LexemeType::L;

        else if (lex.type == LexemeType::LE)
            magazine->getTopPtr()->lex.type = LexemeType::LE;

        else
            throw NotExpectedLexemeException("оператор сравнения", lex);

        magazine->getTopPtr()->term = true;
        magazine->ptrUp();
        break;

    case LexemeType::TypeShiftNonTerm:                                      // сдвиг
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofShiftNonTerm;// хвост сдвига
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeAddSubNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        break;

    case LexemeType::TypeEndofShiftNonTerm:                                 // хвост сдвига
        if (lex.type == LexemeType::ShiftL || lex.type == LexemeType::ShiftR)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofShiftNonTerm;// хвост сдвига 
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeAddSubNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            if (lex.type == LexemeType::ShiftL)
                magazine->getTopPtr()->lex.type = LexemeType::ShiftL;

            else if (lex.type == LexemeType::ShiftR)
                magazine->getTopPtr()->lex.type = LexemeType::ShiftR;

            else
                throw NotExpectedLexemeException("оператор сдвига", lex);

            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Comma || lex.type == LexemeType::Semi || lex.type == LexemeType::ClosePar ||
            lex.type == LexemeType::And || lex.type == LexemeType::Or || isComparisonSign(lex))
            epsilon();
        else
            throw NotExpectedLexemeException("оператор сдвига", lex);
        break;

    case LexemeType::TypeAddSubNonTerm:                                     // слагаемые
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofAddSubNonTerm;// хвост слагаемого
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeMultDivNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        break;

    case LexemeType::TypeEndofAddSubNonTerm:                                // хвост cлагаемого
        if (lex.type == LexemeType::Add || lex.type == LexemeType::Sub)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofAddSubNonTerm;// хвост cлагаемого 
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeMultDivNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            if (lex.type == LexemeType::Add)
                magazine->getTopPtr()->lex.type = LexemeType::Add;
            else
                magazine->getTopPtr()->lex.type = LexemeType::Sub;

            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Comma || lex.type == LexemeType::Semi || lex.type == LexemeType::ClosePar ||
            lex.type == LexemeType::And || lex.type == LexemeType::Or || isComparisonSign(lex) ||
            lex.type == LexemeType::ShiftL || lex.type == LexemeType::ShiftR)
            epsilon();
        else
            throw NotExpectedLexemeException("оператор слагаемых", lex);
        break;

    case LexemeType::TypeMultDivNonTerm:                                     // множители
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofMultDivNonTerm;// хвост множителей
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypeNotNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        break;

    case LexemeType::TypeEndofMultDivNonTerm:                               // хвост множителей
        if (lex.type == LexemeType::Mul || lex.type == LexemeType::Div)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypeEndofMultDivNonTerm;// хвост множителей 
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeNotNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            if (lex.type == LexemeType::Mul)
                magazine->getTopPtr()->lex.type = LexemeType::Mul;
            else
                magazine->getTopPtr()->lex.type = LexemeType::Div;

            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Comma || lex.type == LexemeType::Semi || lex.type == LexemeType::ClosePar ||
            lex.type == LexemeType::And || lex.type == LexemeType::Or || isComparisonSign(lex) ||
            lex.type == LexemeType::ShiftL || lex.type == LexemeType::ShiftR ||
            lex.type == LexemeType::Add || lex.type == LexemeType::Sub)
            epsilon();
        else
            throw NotExpectedLexemeException("оператор множителей", lex);
        break;

    case LexemeType::TypeNotNonTerm:
        magazine->ptrDown();

        if (lex.type == LexemeType::Not)
        {
            magazine->getTopPtr()->lex.type = LexemeType::TypeNotNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Not;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else
        {
            magazine->getTopPtr()->lex.type = LexemeType::TypePostFixNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        break;

    case LexemeType::TypePostFixNonTerm:
        magazine->ptrDown();

        magazine->getTopPtr()->lex.type = LexemeType::TypeEndofPostFixNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();

        magazine->getTopPtr()->lex.type = LexemeType::TypePreFixNonTerm;
        magazine->getTopPtr()->term = false;
        magazine->ptrUp();
        break;

    case LexemeType::TypeEndofPostFixNonTerm:
        if (lex.type == LexemeType::Inc || lex.type == LexemeType::Dec)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypePreFixNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeIncDecSignNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Comma || lex.type == LexemeType::Semi || lex.type == LexemeType::ClosePar ||
            lex.type == LexemeType::And || lex.type == LexemeType::Or || isComparisonSign(lex) ||
            lex.type == LexemeType::ShiftL || lex.type == LexemeType::ShiftR ||
            lex.type == LexemeType::Add || lex.type == LexemeType::Sub ||
            lex.type == LexemeType::Mul || lex.type == LexemeType::Div)
            epsilon();
        else
            throw NotExpectedLexemeException("оператор выражения", lex);
        break;

    case LexemeType::TypeIncDecSignNonTerm:                                 
        magazine->ptrDown();

        if (lex.type == LexemeType::Inc)
            magazine->getTopPtr()->lex.type = LexemeType::Inc;

        else if (lex.type == LexemeType::Dec)
            magazine->getTopPtr()->lex.type = LexemeType::Dec;

        else
            throw NotExpectedLexemeException("оператор инкремент // декремент", lex);

        magazine->getTopPtr()->term = true;
        magazine->ptrUp();
        break;

    case LexemeType::TypePreFixNonTerm:
        if (lex.type == LexemeType::Inc || lex.type == LexemeType::Dec)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::TypePreFixNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeIncDecSignNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::Id || isConst(lex) ||
            lex.type == LexemeType::OpenPar)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::TypeBasicExprNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::ClosePar || lex.type == LexemeType::Semi)
            epsilon();
        else
            throw NotExpectedLexemeException("элементарное выражение", lex);
        break;

    case LexemeType::TypeBasicExprNonTerm:
        if (lex.type == LexemeType::Id && scanner->LookForward(1).type == LexemeType::OpenPar)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::TypeFuncCallNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        /*else if ((lex.type == LexemeType::Id && scanner->LookForward(1).type == LexemeType::Inc) ||
            (lex.type == LexemeType::Id && scanner->LookForward(1).type == LexemeType::Dec))
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::Id;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }*/
        else if (lex.type == LexemeType::Id)
        {
            magazine->ptrDown();
            /*
            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::Assign;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();*/

            magazine->getTopPtr()->lex.type = LexemeType::Id;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (isConst(lex))
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::TypeConstNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::OpenPar)
        {
            magazine->ptrDown();

            magazine->getTopPtr()->lex.type = LexemeType::ClosePar;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::TypeExprNonTerm;
            magazine->getTopPtr()->term = false;
            magazine->ptrUp();

            magazine->getTopPtr()->lex.type = LexemeType::OpenPar;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else
            throw NotExpectedLexemeException("выражение", lex);

        
        break;

    case LexemeType::TypeConstNonTerm:
        if (lex.type == LexemeType::ConstInt)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::ConstInt;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else if (lex.type == LexemeType::ConstBool)
        {
            magazine->ptrDown();
            magazine->getTopPtr()->lex.type = LexemeType::ConstBool;
            magazine->getTopPtr()->term = true;
            magazine->ptrUp();
        }
        else
            throw InvalidTypeException(lex.str);
        break;

    default:
        throw NotExpectedLexemeException("лексема", lex);
        break;
    }

}
