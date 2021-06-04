
#include "MathExprParser.h"
#include <cmath>
#include <assert.h>

#ifndef NAN
#define NAN (0.0/0.0)
#endif

#ifndef INFINITY
#define INFINITY (1.0/0.0)
#endif

namespace mathexpr
{	
	static void Assert(bool b)
	{
		assert(b);
	}
	template<class T>
	T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	static bool IsNumeric(char c)
	{
		return c >= '0' && c <= '9';
	}
	static bool IsAlpha(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	static bool IsVariable(char c)
	{
		return IsAlpha(c) || IsNumeric(c) || c == '_';
	}
	static double Add(double a, double b) { return a + b; }
	static double Sub(double a, double b) { return a - b; }
	static double Mul(double a, double b) { return a * b; }
	static double Divide(double a, double b) { return a / b; }
	static double Power(double a, double b) { return pow(a, b); }
	static double FMod(double a, double b) { return fmod(a, b); }
	static double NegateOp(double a) { return -a; }
	static double CommaOp(double a, double b) { (void)a; return b; }
	static double AndOp(double a, double b) { return a && b ? 1 : 0; }
	static double OrOp(double a, double b) { return a || b ? 1 : 0; }
	static double EqualOp(double a, double b) { return a == b ? 1 : 0; }
	static double NotEqualOp(double a, double b) { return a != b ? 1 : 0; }
	static double LessOp(double a, double b) { return a < b ? 1 : 0; }
	static double LequalOp(double a, double b) { return a <= b ? 1 : 0; }
	static double BigOp(double a, double b) { return a > b ? 1 : 0; }
	static double BequalOp(double a, double b) { return a >= b ? 1 : 0; }

	static double Abs(double a) { return abs(a); }
	static double ACos(double a) { return acos(a); }
	static double Asin(double a) { return asin(a); }
	static double Atan(double a) { return atan(a); }
	static double Ceil(double a) { return ceil(a); }
	static double Cos(double a) { return cos(a); }
	static double E() { return 2.71828182845904523536; }
	static double Exp(double a) { return exp(a); }
	static double Floor(double a) { return floor(a); }
	static double Log(double a) { return log(a); }
	static double Log10(double a) { return log10(a); }
	static double Sin(double a) { return sin(a); }
	static double Pi() { return 3.14159265358979323846; }
	static double Pow(double a, double b) { return pow(a, b); }
	static double Sqrt(double a) { return sqrt(a); }
	static double Tan(double a) { return tan(a); }

	static void InitInsideTable(std::map<std::string, FCustomVariable>& InsideTable)
	{
		static const FCustomVariable Table[] = {
			{"abs", Abs,     EExprType::Function1 | EExprType::PureFunction},
			{"acos", ACos,     EExprType::Function1 | EExprType::PureFunction},
			{"asin", Asin,     EExprType::Function1 | EExprType::PureFunction},
			{"atan", Atan,     EExprType::Function1 | EExprType::PureFunction},
			{"ceil", Ceil,     EExprType::Function1 | EExprType::PureFunction},
			{"cos", Cos,     EExprType::Function1 | EExprType::PureFunction},
			{"e", E,     EExprType::Function0 | EExprType::PureFunction},
			{"exp", Exp,     EExprType::Function1 | EExprType::PureFunction},
			{"floor", Floor,     EExprType::Function1 | EExprType::PureFunction},
			{"log", Log,     EExprType::Function1 | EExprType::PureFunction},
			{"log10", Log10,     EExprType::Function1 | EExprType::PureFunction},
			{"sin", Sin,     EExprType::Function1 | EExprType::PureFunction},
			{"pi", Pi,     EExprType::Function0 | EExprType::PureFunction},
			{"pow", Pow,     EExprType::Function2 | EExprType::PureFunction},
			{"sqrt", Sqrt,     EExprType::Function1 | EExprType::PureFunction},
			{"tan", Tan,     EExprType::Function1 | EExprType::PureFunction},
		};

		for (auto& It : Table)
		{
			InsideTable[It.Name] = It;
		}
	}

	FCommonExprPtr NewExpr(EExprType Type, FCommonExprPtr Args1, FCommonExprPtr Args2)
	{
		FCommonExprPtr One(new FCommonExpr());
		One->Type = Type;
		if (Args1 != nullptr)
		{
			One->Parameters.push_back(Args1);
		}
		if (Args2 != nullptr)
		{
			One->Parameters.push_back(Args2);
		}
		One->value = 0;

		return One;
	}
	FCommonExprPtr NewExpr(EExprType Type, FCommonExprPtr Args1)
	{
		return NewExpr(Type, nullptr, nullptr);
	}
	FCommonExprPtr NewExpr(EExprType Type)
	{
		return NewExpr(Type, nullptr, nullptr);
	}

	double ExecValue(FCommonExprPtr Ptr)
	{
#define TE_FUN(...) ((double(*)(__VA_ARGS__))Ptr->function)
#define M(e) ExecValue(Ptr->Parameters[e])
		if (Ptr == nullptr)
		{
			return 0;
		}

		if (Ptr->Type == EExprType::None)
		{
			return 0;
		}
		
		if (Ptr->Type == EExprType::Constant)
		{
			return Ptr->value;
		}

		if (Ptr->Type == EExprType::Variable)
		{
			return *(Ptr->bound);
		}

		auto Func = Ptr->Type;
		if (Func > EExprType::PureFunction)
		{
			Func = Func - EExprType::PureFunction;
		}

		switch(Func)
		{
		case EExprType::Function0:
			return TE_FUN(void)();

		case EExprType::Function1:
			return TE_FUN(double)(M(0));

		case EExprType::Function2:
			return TE_FUN(double, double)(M(0), M(1));

		case EExprType::Function3:
			return TE_FUN(double, double, double)(M(0), M(1), M(2));

		case EExprType::Function4:
			return TE_FUN(double, double, double, double)(M(0), M(1), M(2), M(3));

		case EExprType::Function5:
			return TE_FUN(double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4));

		case EExprType::Function6:
			return TE_FUN(double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5));

		case EExprType::Function7:
			return TE_FUN(double, double, double, double, double, double, double)(M(0), M(1), M(2), M(3), M(4), M(5), M(6));
		}
		Assert(0);

#undef TE_FUN
#undef M

		return 0;
	}


	FMathExpr::FMathExpr()
	{
		InitInsideTable(InsideTable);
	}

	int FMathExpr::Parse(bool bInLogic, const char* Expr)
	{
		return Parse(bInLogic, Expr, std::vector<FCustomVariable>());
	}
	
	int FMathExpr::Parse(bool bInLogic, const char* Expr, const std::vector<FCustomVariable>& InCustomTable)
	{
		bLogic = bInLogic;
		ExprState = FExprState{};

		ExprState.OrigStr = Expr;
		for (auto& It : InCustomTable)
		{
			ExprState.CustomTable[It.Name] = It;
		}

		int Ret = ParseSyntax();
		//if (ExprState.Type == ETokenType::Error)
		//{
		//	return ExprState.CurPos;
		//}

		return Ret;

	}

	double FMathExpr::Exec()
	{
		return ExecValue(RootExpr);
	}

	void FMathExpr::NextToken()
	{
		/*
			token指：
				数字：1, 100, 100.2, 0.2
				变量：v123, _v123, v_123, v_
				操作符：/+-*%^
				逻辑操作符：&& || > <
		*/

		ExprState.Type = ETokenType::None;
		ExprState.value = 0;
		ExprState.TokenStr = "";
		ExprState.MetaValue = FCustomVariable{};

		do 
		{
			if (ExprState.CurPos >= ExprState.OrigStr.size())
			{
				ExprState.Type = ETokenType::End;
				break;
			}

			auto C = ExprState.OrigStr[ExprState.CurPos];
			if (IsNumeric(C) || C == '.')
			{
				// 如果是数字
				char* Cur = &ExprState.OrigStr[ExprState.CurPos];
				char* Old = Cur;
				auto V = strtod(Cur, &Cur);
				int Step = Cur - Old;

				ExprState.TokenStr = ExprState.OrigStr.substr(ExprState.CurPos, Step);
				ExprState.CurPos += Step;
				ExprState.Type = ETokenType::Number;
				ExprState.value = V;
			}
			else if (IsAlpha(C) || C == '_')
			{
				// 如果是变量
				auto Cur = ExprState.CurPos;
				do 
				{
					Cur++;
				} while (Cur < ExprState.OrigStr.size() && (IsVariable(ExprState.OrigStr[Cur])));
				
				ExprState.TokenStr = ExprState.OrigStr.substr(ExprState.CurPos, Cur - ExprState.CurPos);
				ExprState.Type = ETokenType::Variable;
				ExprState.CurPos = Cur;

				// 检查变量是否有效
				// 内置函数
				// 自定义函数
				// 自定义变量
				auto ItInside = InsideTable.find(ExprState.TokenStr);
				if (ItInside != InsideTable.end())
				{
					ExprState.MetaValue = ItInside->second;
				}
				else
				{
					ItInside = ExprState.CustomTable.find(ExprState.TokenStr);
					if (ItInside != ExprState.CustomTable.end())
					{
						ExprState.MetaValue = ItInside->second;
					}
					else
					{
						auto DToken = DynamicToken(ExprState.TokenStr);
						if (DToken.Name.size() > 0)
						{
							ExprState.MetaValue = DToken;
						}
						else
						{
							ExprState.MarkError(__FILE__, __LINE__);
							break;
						}
					}
				}
			}
			else
			{
				int Step = 1;
				switch (C)
				{
				case '+': ExprState.Type = ETokenType::Operand; ExprState.function = Add; break;
				case '-': ExprState.Type = ETokenType::Operand; ExprState.function = Sub; break;
				case '/': ExprState.Type = ETokenType::Operand; ExprState.function = Divide; break;
				case '*': ExprState.Type = ETokenType::Operand; ExprState.function = Mul; break;
				case '^': ExprState.Type = ETokenType::Operand; ExprState.function = Power; break;
				case '%': ExprState.Type = ETokenType::Operand; ExprState.function = FMod; break;

				case '(': ExprState.Type = ETokenType::BracketOpen; break;
				case ')': ExprState.Type = ETokenType::BracketClose; break;

				case ',': ExprState.Type = ETokenType::Comma; break;

				case ' ': case '\t': case '\n': case '\r': break;

				/**************************************************************************/
				/* 逻辑操作符: && || != == > >= < <=                                      */
				/**************************************************************************/
				case '&': 
					if (ExprState.GetNext() == '&')
					{
						Step = 2;
						ExprState.Type = ETokenType::LogicOperand;
						ExprState.function = AndOp;
					}
					else
					{
						ExprState.MarkError(__FILE__, __LINE__);
					}
					break;

				case '|':
					if (ExprState.GetNext() == '|')
					{
						Step = 2;
						ExprState.Type = ETokenType::LogicOperand;
						ExprState.function = OrOp;
					}
					else
					{
						ExprState.MarkError(__FILE__, __LINE__);
					}
					break;

				case '=':
					if (ExprState.GetNext() == '=')
					{
						Step = 2;
						ExprState.Type = ETokenType::LogicOperand;
						ExprState.function = EqualOp;
					}
					else
					{
						ExprState.MarkError(__FILE__, __LINE__);
					}
					break;

				case '!':
					if (ExprState.GetNext() == '=')
					{
						Step = 2;
						ExprState.Type = ETokenType::LogicOperand;
						ExprState.function = NotEqualOp;
					}
					else
					{
						ExprState.MarkError(__FILE__, __LINE__);
					}
					break;

				case '>':
					if (ExprState.GetNext() == '=')
					{
						Step = 2;
						ExprState.Type = ETokenType::LogicOperand;
						ExprState.function = BequalOp;
					}
					else
					{
						Step = 1;
						ExprState.Type = ETokenType::LogicOperand;
						ExprState.function = BigOp;
					}
					break;

				case '<':
					if (ExprState.GetNext() == '=')
					{
						Step = 2;
						ExprState.Type = ETokenType::LogicOperand;
						ExprState.function = LequalOp;
					}
					else
					{
						Step = 1;
						ExprState.Type = ETokenType::LogicOperand;
						ExprState.function = LessOp;
					}
					break;

				default:
					ExprState.MarkError(__FILE__, __LINE__);
					break;
				}

				if (ExprState.Type != ETokenType::Error)
				{
					if (ExprState.Type != ETokenType::None)
					{
						ExprState.TokenStr = ExprState.OrigStr.substr(ExprState.CurPos, Step);
					}

					ExprState.CurPos += Step;
				}
			}

		} while (ExprState.Type == ETokenType::None);
	}

	int FMathExpr::ParseSyntax()
	{
		NextToken();
				
		if (bLogic)
		{
			RootExpr = SyntaxLogic();
		}
		else
		{
			RootExpr = SyntaxCalc();
		}

		if (ExprState.Type == ETokenType::Error)
		{
			return Max(size_t(1), ExprState.CurPos);
		}
		else if (ExprState.Type != ETokenType::End)
		{
			ExprState.MarkError(__FILE__, __LINE__);
			// 错误
			return Max(size_t(1), ExprState.CurPos);
		}

		return 0;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxCalc()
	{
		return SyntaxComma();
	}

	FCommonExprPtr FMathExpr::SyntaxComma()
	{
		// 逗号操作符 a,b，不支持a,b,c
		FCommonExprPtr Ptr;

		Ptr = SyntaxAddSub();
		
		if (ExprState.Type == ETokenType::Comma)
		{
			NextToken();
			Ptr = NewExpr(EExprType::Function2 | EExprType::PureFunction, Ptr, SyntaxAddSub());
			Ptr->function = CommaOp;
		}
		

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxAddSub()
	{
		// 加减操作符 a+b+c
		FCommonExprPtr Ptr;
		
		Ptr = SyntaxMulDivide();
		while (ExprState.Type == ETokenType::Operand && (ExprState.function == Add || ExprState.function == Sub))
		{
			auto Old = ExprState.function;
			NextToken();
			Ptr = NewExpr(EExprType::Function2 | EExprType::PureFunction, Ptr, SyntaxMulDivide());
			Ptr->function = Old;
		}

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxMulDivide()
	{
		// 乘除模操作符 a*b/c%d
		FCommonExprPtr Ptr;

		Ptr = SyntaxPow();
		while (ExprState.Type == ETokenType::Operand && (ExprState.function == Mul || ExprState.function == Divide || ExprState.function == FMod))
		{
			auto Old = ExprState.function;
			NextToken();
			Ptr = NewExpr(EExprType::Function2 | EExprType::PureFunction, Ptr, SyntaxPow());
			Ptr->function = Old;
		}

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxPow()
	{
		// 指数操作符 a^b
		FCommonExprPtr Ptr;

		Ptr = SyntaxMinus();
		while (ExprState.Type == ETokenType::Operand && (ExprState.function == Power))
		{
			auto Old = ExprState.function;
			NextToken();
			Ptr = NewExpr(EExprType::Function2 | EExprType::PureFunction, Ptr, SyntaxMinus());
			Ptr->function = Old;
		}

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxMinus()
	{
		// 正负操作符，-1, +5
		FCommonExprPtr Ptr;

		int Sign = 1;
		int Count = 0;
		while (ExprState.Type == ETokenType::Operand && (ExprState.function == Add || ExprState.function == Sub))
		{
			Count++;
			if (ExprState.function == Sub)
			{
				Sign = -Sign;
			}
			NextToken();
		}

		if (Count > 1)
		{
			// 出现这种情况：--1，++1，这种直接报错了。
			ExprState.MarkError(__FILE__, __LINE__);
			return Ptr;
		}

		if (Sign == 1)
		{
			Ptr = SyntaxBase();
		}
		else
		{
			Ptr = NewExpr(EExprType::Function1 | EExprType::PureFunction, SyntaxBase(), nullptr);
			Ptr->function = NegateOp;
		}

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxBase()
	{
		FCommonExprPtr Ptr;

		switch (ExprState.Type)
		{
		case ETokenType::BracketOpen:
			// 括号里面，是一个完整的表达式
			NextToken();
			if (bLogic)
			{
				Ptr = SyntaxLogic();
			}
			else
			{
				Ptr = SyntaxCalc();
			}
			if (ExprState.Type == ETokenType::BracketClose)
			{
				NextToken();
			}
			else
			{
				ExprState.MarkError(__FILE__, __LINE__);
			}
			break;

		case ETokenType::Number:
			Ptr = NewExpr(EExprType::Constant);
			Ptr->value = ExprState.value;
			NextToken();
			break;

		case ETokenType::Variable:
			if (ExprState.MetaValue.Type == EExprType::None)
			{
				ExprState.MarkError(__FILE__, __LINE__);
			}
			else if (ExprState.MetaValue.Type == EExprType::Constant)
			{
				Ptr = NewExpr(EExprType::Constant);
				Ptr->value = VoidToConst(ExprState.MetaValue.Addr);
				NextToken();
			}
			else if (ExprState.MetaValue.Type == EExprType::Variable)
			{
				Ptr = NewExpr(EExprType::Variable);
				Ptr->function = ExprState.MetaValue.Addr;
				NextToken();
			}
			else
			{
				auto Func = ExprState.MetaValue.Type;
				if (Func > EExprType::PureFunction)
				{
					Func = EExprType((int)Func - (int)EExprType::PureFunction);
				}

				switch (Func)
				{
				case EExprType::Function0:
					// func0()
					Ptr = NewExpr(EExprType::Function0);
					Ptr->function = ExprState.MetaValue.Addr;
					NextToken();

					// 必定是括号
					if (ExprState.Type == ETokenType::BracketOpen)
					{
						NextToken();
						if (ExprState.Type == ETokenType::BracketClose)
						{
							NextToken();
						}
						else
						{
							ExprState.MarkError(__FILE__, __LINE__);
						}
					}
					else
					{
						ExprState.MarkError(__FILE__, __LINE__);
					}

					break;

				case EExprType::Function1:
				case EExprType::Function2:
				case EExprType::Function3:
				case EExprType::Function4:
				case EExprType::Function5:
				case EExprType::Function6:
				case EExprType::Function7:
					Ptr = NewExpr(Func);
					Ptr->function = ExprState.MetaValue.Addr;

					int ArgsCount = (int)Func - (int)EExprType::Function0;

					// 接下来一定是(v1, v2, v3, v4, ...)这种。其中v1是一个表达式（但不是逗号表达式）。

					NextToken();
					if (ExprState.Type == ETokenType::BracketOpen)
					{
						// 函数内部嵌套的是正常的表达式
						for (int i = 0; i < ArgsCount; i++)
						{
							NextToken();
							auto NewPtr = SyntaxAddSub();
							Ptr->Parameters.push_back(NewPtr);

							if (i < (ArgsCount - 1) && ExprState.Type != ETokenType::Comma)
							{
								ExprState.MarkError(__FILE__, __LINE__);
								break;
							}
						}
						if (ExprState.Type == ETokenType::BracketClose)
						{
							NextToken();
							break;
						}
						else
						{
							ExprState.MarkError(__FILE__, __LINE__);
						}
					}
					else
					{
						ExprState.MarkError(__FILE__, __LINE__);
					}

					break;
				}
				
			}
			break;
			
		default:
			Ptr = NewExpr(EExprType::None);
			Ptr->value = NAN;
			ExprState.MarkError(__FILE__, __LINE__);
			break;
		}

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxLogic()
	{
		return SyntaxLogicOr();
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxLogicOr()
	{
		// 逗号操作符 a,b，不支持a,b,c
		FCommonExprPtr Ptr;

		Ptr = SyntaxLogicAnd();

		while (ExprState.Type == ETokenType::LogicOperand && ExprState.function == OrOp)
		{
			NextToken();
			Ptr = NewExpr(EExprType::Function2, Ptr, SyntaxLogicAnd());
			Ptr->function = OrOp;
		}

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxLogicAnd()
	{
		// 逗号操作符 a,b，不支持a,b,c
		FCommonExprPtr Ptr;

		Ptr = SyntaxLogicEqualOrNot();

		while (ExprState.Type == ETokenType::LogicOperand && ExprState.function == AndOp)
		{
			NextToken();
			Ptr = NewExpr(EExprType::Function2, Ptr, SyntaxLogicEqualOrNot());
			Ptr->function = AndOp;
		}

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxLogicEqualOrNot()
	{
		// 逗号操作符 a,b，不支持a,b,c
		FCommonExprPtr Ptr;

		Ptr = SyntaxLogicBigSmall();

		if (ExprState.Type == ETokenType::LogicOperand
			&& (ExprState.function == EqualOp || ExprState.function == NotEqualOp))
		{
			auto Old = ExprState.function;
			NextToken();
			Ptr = NewExpr(EExprType::Function2, Ptr, SyntaxLogicBigSmall());
			Ptr->function = Old;
		}

		return Ptr;
	}

	mathexpr::FCommonExprPtr FMathExpr::SyntaxLogicBigSmall()
	{
		// 逗号操作符 a,b，不支持a,b,c
		FCommonExprPtr Ptr;

		Ptr = SyntaxComma();

		if (ExprState.Type == ETokenType::LogicOperand
			&& (ExprState.function == LessOp 
				|| ExprState.function == LequalOp
				|| ExprState.function == BigOp
				|| ExprState.function == BequalOp))
		{
			auto Old = ExprState.function;
			NextToken();
			Ptr = NewExpr(EExprType::Function2, Ptr, SyntaxComma());
			Ptr->function = Old;
		}

		return Ptr;
	}

}

