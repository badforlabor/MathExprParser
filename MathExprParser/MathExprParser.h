#pragma once

/**,
 * Auth :   liubo
 * Date :   2021/06/02 18:14
 * Comment: 数学表达式  
 */
 
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace mathexpr
{
	enum class ETokenType
	{
		None,		
		Error,	// 错误
		End,	// 结束
		Comma,	// 逗号
		BracketOpen,	// 左括号
		BracketClose,	// 右括号
		Number,			// 数字
		Variable,		// 变量
		Operand,		// 操作符
		LogicOperand,	// 逻辑操作符
	};
	enum class EExprType
	{
		None,
		Variable = 1,	// 变量
		Constant = 2,	// 固定数值
		
		Function0 = 8,	// 没有参数的函数
		Function1,	// 只有一个参数的函数
		Function2,
		Function3,
		Function4,
		Function5,
		Function6,
		Function7,

		PureFunction = 32,	// 这种可以直接计算出数据。比如sin函数。
	};
	inline EExprType operator|(EExprType a, EExprType b)
	{
		return static_cast<EExprType>(static_cast<int>(a) | static_cast<int>(b));
	}
	inline EExprType operator&(EExprType a, EExprType b)
	{
		return static_cast<EExprType>(static_cast<int>(a) & static_cast<int>(b));
	}
	inline EExprType operator-(EExprType a, EExprType b)
	{
		return static_cast<EExprType>(static_cast<int>(a) - static_cast<int>(b));
	}


	struct FCommonExpr
	{
		EExprType Type = EExprType::None;
		// 这个union有个问题，如果是32位系统，那么三个元素占用的大小不一样。
		union { double value; const double *bound; const void *function; };
		std::vector<std::shared_ptr<FCommonExpr>> Parameters;
	};
	typedef std::shared_ptr<FCommonExpr> FCommonExprPtr;

	struct FCustomVariable
	{
		std::string Name;
		void* Addr = 0;
		EExprType Type = EExprType::None;
	};

	struct FExprState
	{
		std::string OrigStr;
		size_t CurPos = 0;
		ETokenType Type = ETokenType::None;
		
		union { double value; const double *bound; const void *function; };
		std::string TokenStr;

		std::map<std::string, FCustomVariable> CustomTable;

		FCustomVariable MetaValue;

		char GetCurrent() { return CurPos < OrigStr.size() ? OrigStr[CurPos] : 0; }
		char GetNext() { return (CurPos+1) < OrigStr.size() ? OrigStr[CurPos + 1] : 0; }

		void MarkError(const char* FileName, int LineCount) { Type = ETokenType::Error; }
	};

	class FMathExpr
	{
	public:
		virtual ~FMathExpr() {}
		FMathExpr();

		int Parse(bool bInLogic, const char* Expr, const std::vector<FCustomVariable>& InCustomTable);
		int Parse(bool bInLogic, const char* Expr);

		double Exec();

	protected:
		virtual void NextToken();
		virtual int ParseSyntax();

		// 动态识别一个token
		virtual FCustomVariable DynamicToken(const std::string& str) {
			return FCustomVariable();
		}

		FCommonExprPtr SyntaxCalc();
		FCommonExprPtr SyntaxComma();
		FCommonExprPtr SyntaxAddSub();
		FCommonExprPtr SyntaxMulDivide();
		FCommonExprPtr SyntaxPow();
		FCommonExprPtr SyntaxMinus();
		FCommonExprPtr SyntaxBase();

		FCommonExprPtr SyntaxLogic();
		FCommonExprPtr SyntaxLogicOr();
		FCommonExprPtr SyntaxLogicAnd();
		FCommonExprPtr SyntaxLogicEqualOrNot();
		FCommonExprPtr SyntaxLogicBigSmall();

		// 是否是逻辑运算
		bool bLogic = false;
		FExprState ExprState;

		// 内置的
		std::map<std::string, FCustomVariable> InsideTable;

		FCommonExprPtr RootExpr;
	};
}
