/**,
 * Auth :   liubo
 * Date :   2021/06/03 15:11
 * Comment: 冒烟测试  
 */

#include "minctest.h"
#include "MathExprParser.h"

typedef struct {
	const char *expr;
	double answer;
} test_case;

typedef struct {
	const char *expr1;
	const char *expr2;
} test_equ;



double te_interp(const char *expression, int *error, bool logic = false) {
	using namespace mathexpr;
	FMathExpr One;
	int n = One.Parse(logic, expression);
	double ret;
	if (n == 0) {
		ret = One.Exec();
		if (error)
		{
			*error = 0;
		}
	}
	else {
		ret = NAN;
		if (error)
		{
			*error = n;
		}
	}
	return ret;
}
//struct te_expr {
//
//};
//struct te_variable : mathexpr::FCustomVariable {
//
//};
using te_variable = mathexpr::FCustomVariable;
using te_expr = mathexpr::FMathExpr;
te_expr *te_compile(const char *expression, const te_variable *variables, int var_count, int *error) {

	using namespace mathexpr;

	std::vector<FCustomVariable> CustomTable;
	CustomTable.reserve(var_count);
	for (int i = 0; i < var_count; i++)
	{
		CustomTable.push_back(variables[i]);
	}


	static FMathExpr One;
	int n = One.Parse(false, expression, CustomTable);
	*error = n;
	if (n > 0)
	{
		return nullptr;
	}

	return &One;
}
double te_eval(te_expr *expr)
{
	if (expr)
	{
		return expr->Exec();
	}
	return 0;
}

void test_results() {
	test_case cases[] = {
		{"1", 1},
		{"1 ", 1},
		{"(1)", 1},

		{"pi()", 3.14159},
		{"atan(1)*4 - pi()", 0},
		{"e()", 2.71828},

		{"2+1", 2 + 1},
		{"(((2+(1))))", 2 + 1},
		{"3+2", 3 + 2},

		{"3+2+4", 3 + 2 + 4},
		{"(3+2)+4", 3 + 2 + 4},
		{"3+(2+4)", 3 + 2 + 4},
		{"(3+2+4)", 3 + 2 + 4},

		{"3*2*4", 3 * 2 * 4},
		{"(3*2)*4", 3 * 2 * 4},
		{"3*(2*4)", 3 * 2 * 4},
		{"(3*2*4)", 3 * 2 * 4},

		{"3-2-4", 3 - 2 - 4},
		{"(3-2)-4", (3 - 2) - 4},
		{"3-(2-4)", 3 - (2 - 4)},
		{"(3-2-4)", 3 - 2 - 4},

		{"3/2/4", 3.0 / 2.0 / 4.0},
		{"(3/2)/4", (3.0 / 2.0) / 4.0},
		{"3/(2/4)", 3.0 / (2.0 / 4.0)},
		{"(3/2/4)", 3.0 / 2.0 / 4.0},

		{"(3*2/4)", 3.0*2.0 / 4.0},
		{"(3/2*4)", 3.0 / 2.0*4.0},
		{"3*(2/4)", 3.0*(2.0 / 4.0)},

		{"asin(sin(.5))", 0.5},
		{"sin(asin(.5))", 0.5},
		//{"ln exp .5", 0.5},
		//{"exp ln .5", 0.5},

		{"asin(sin(-.5))", -0.5},
		{"asin (sin(-0.5))", -0.5},
		{"asin (sin (-0.5))", -0.5},
		{"asin (sin (-0.5))", -0.5},
		{"asin (sin (-0.5))", -0.5},
		{"asin (sin (-0.5))", -0.5},
		{"(asin (sin (-0.5)))", -0.5},

		{"log10(1000)", 3},
		{"log10(1e3)", 3},
		{"log10(1000)", 3},
		{"log10(1e3)", 3},
		{"log10(1000)", 3},
		{"log10(1e3)", 3},
		{"log10(1.0e3)", 3},
		{"10^5*5e-5", 5},

#ifdef TE_NAT_LOG
		{"log 1000", 6.9078},
		{"log e", 1},
		{"log (e^10)", 10},
#else
		{"log(1000)", 3},
#endif

		//{"ln (e^10)", 10},
		{"100^.5+1", 11},
		{"100 ^.5+1", 11},
		{"100^+.5+1", 11},
		//{"100^--.5+1", 11},
		//{"100^---+-++---++-+-+-.5+1", 11},

		{"100^-.5+1", 1.1},
		//{"100^---.5+1", 1.1},
		//{"100^+---.5+1", 1.1},
		//{"1e2^+---.5e0+1e0", 1.1},
		//{"--(1e2^(+(-(-(-.5e0))))+1e0)", 1.1},

		{"sqrt (100) + 7", 17},
		{"sqrt (100) * 7", 70},
		{"sqrt (100 * 100)", 100},

		{"1,2", 2},
		{"1,2+1", 3},
		//{"1+1,2+2,2+1", 3},
		//{"1,2,3", 3},
		{"(1,2),3", 3},
		{"1,(2,3)", 3},
		{"-(1,(2,3))", -3},

		{"2^2", 4},
		{"pow(2,2)", 4},

		//{"atan2(1,1)", 0.7854},
		//{"atan2(1,2)", 0.4636},
		//{"atan2(2,1)", 1.1071},
		//{"atan2(3,4)", 0.6435},
		//{"atan2(3+3,4*2)", 0.6435},
		//{"atan2(3+3,(4*2))", 0.6435},
		//{"atan2((3+3),4*2)", 0.6435},
		//{"atan2((3+3),(4*2))", 0.6435},

	};

	test_case cases2[] = {
		{"1", 1},
		{"1 && 0", 0},
		{"1 || 0", 1},
		{"1>2 || 0>1", 0},
		{"1>2 || 1>0", 1},
		{"1>2 && 1>0", 0},
		{"1!=2 && 1!=0", 1},
		{"1==2 || 1==0", 0},
		{"1!=2 || 1==0", 1},
		{"(1==2 || 1==0) || (1!=2 || 1==0)", 1},
		{"(1==2 || 1==0) || (1!=2 || 1==0)", 0},
	};

	int i;
	for (i = 0; i < sizeof(cases) / sizeof(test_case); ++i) {
		const char *expr = cases[i].expr;
		const double answer = cases[i].answer;

		int err;
		const double ev = te_interp(expr, &err);
		lok(!err);
		lfequal_expr(ev, answer, expr);

		if (err) {
			printf("FAILED: %s (%d)\n", expr, err);
		}
	}

	printf("\n=============== logic exp test ===============\n");

	for (i = 0; i < sizeof(cases2) / sizeof(test_case); ++i) {
		const char *expr = cases2[i].expr;
		const double answer = cases2[i].answer;

		int err;
		const double ev = te_interp(expr, &err, true);
		lok(!err);
		lfequal_expr(ev, answer, expr);

		if (err) {
			printf("FAILED: %s (%d)\n", expr, err);
		}
	}

}


void test_syntax() {
	test_case errors[] = {
		{"", 1},
		{"1+", 2},
		{"1)", 2},
		{"(1", 2},
		{"1**1", 3},
		{"1*2(+4", 4},
		{"1*2(1+4", 4},
		{"a+5", 1},
		{"!+5", 1},
		{"_a+5", 1},
		{"#a+5", 1},
		{"1^^5", 3},
		{"1**5", 3},
		{"sin(cos5", 8},

		// 以下语法是不支持的。
		{"--(1e2^(+(-(-(-.5e0))))+1e0)", 2},
		{"ln exp .5", 2},
		{"sin 0.5", 3},

		// pi是函数，所以不支持；如果pi是变量，那么是支持的
		{"pi", 2},
		{"e", 1},
	};


	int i;
	for (i = 0; i < sizeof(errors) / sizeof(test_case); ++i) {
		const char *expr = errors[i].expr;
		const int e = errors[i].answer;

		int err;
		const double r = te_interp(expr, &err);
		lfequal_expr(err, e, expr);
		lok(r != r);

		te_expr *n = te_compile(expr, 0, 0, &err);
		lfequal_expr(err, e, expr);
		lok(!n);

		if (err != e) {
			printf("FAILED: %s. err=%d, e=%d\n", expr, err, e);
		}

		const double k = te_interp(expr, 0);
		lok(k != k);
	}
}

static double sum0() {
	return 6;
}
static double sum1(double a) {
	return a * 2;
}
static double sum2(double a, double b) {
	return a + b;
}
static double sum3(double a, double b, double c) {
	return a + b + c;
}
static double sum4(double a, double b, double c, double d) {
	return a + b + c + d;
}
static double sum5(double a, double b, double c, double d, double e) {
	return a + b + c + d + e;
}
static double sum6(double a, double b, double c, double d, double e, double f) {
	return a + b + c + d + e + f;
}
static double sum7(double a, double b, double c, double d, double e, double f, double g) {
	return a + b + c + d + e + f + g;
}


void test_dynamic() {
	using namespace mathexpr;

	double x, f;
	te_variable lookup[] = {
		{"x", &x, EExprType::Variable},
		{"f", &f, EExprType::Variable},
		{"sum0", sum0, EExprType::Function0},
		{"sum1", sum1, EExprType::Function1},
		{"sum2", sum2, EExprType::Function2},
		{"sum3", sum3, EExprType::Function3},
		{"sum4", sum4, EExprType::Function4},
		{"sum5", sum5, EExprType::Function5},
		{"sum6", sum6, EExprType::Function6},
		{"sum7", sum7, EExprType::Function7},
	};

	test_case cases[] = {
		{"x", 2},
		{"f+x", 7},
		{"x+x", 4},
		{"x+f", 7},
		{"f+f", 10},
		{"f+sum0()", 11},
		{"sum0()+sum0()", 12},
		{"sum0()+sum0()", 12},
		{"sum0()+sum0()", 12},
		{"sum0()+(0)+sum0()", 12},
		{"sum1(sum0())", 12},
		{"sum1 (sum0())", 12},
		{"sum1(f)", 10},
		{"sum1 (x)", 4},
		{"sum2 (sum0(), x)", 8},
		{"sum3 (sum0(), x, 2)", 10},
		{"sum2(2,3)", 5},
		{"sum3(2,3,4)", 9},
		{"sum4(2,3,4,5)", 14},
		{"sum5(2,3,4,5,6)", 20},
		{"sum6(2,3,4,5,6,7)", 27},
		{"sum7(2,3,4,5,6,7,8)", 35},
	};

	x = 2;
	f = 5;

	int i;
	for (i = 0; i < sizeof(cases) / sizeof(test_case); ++i) {
		const char *expr = cases[i].expr;
		const double answer = cases[i].answer;

		int err;
		te_expr *ex = te_compile(expr, lookup, sizeof(lookup) / sizeof(te_variable), &err);
		lok(ex);
		auto ret = te_eval(ex);
		lfequal_expr(ret, answer, expr);
	}
}



void test_dynamic2() {
	using namespace mathexpr;

	double x, f;
	te_variable lookup[] = {
		{"x", &x, EExprType::Variable},
		{"f", &f, EExprType::Variable},
		{"z", ConstToVoid(1), EExprType::Constant},
		{"sum0", sum0, EExprType::Function0},
		{"sum1", sum1, EExprType::Function1},
		{"sum2", sum2, EExprType::Function2},
		{"sum3", sum3, EExprType::Function3},
		{"sum4", sum4, EExprType::Function4},
		{"sum5", sum5, EExprType::Function5},
		{"sum6", sum6, EExprType::Function6},
		{"sum7", sum7, EExprType::Function7},
	};

	test_case cases[] = {
		{"x", 2},
		{"z", 1},
		{"x+z+x+z", 6},
		{"f+x", 7},
		{"x+x", 4},
		{"x+f", 7},
		{"f+f", 10},
		{"f+sum0()", 11},
		{"sum0()+sum0()", 12},
		{"sum0()+sum0()", 12},
		{"sum0()+sum0()", 12},
		{"sum0()+(0)+sum0()", 12},
		{"sum1(sum0())", 12},
		{"sum1 (sum0())", 12},
		{"sum1(f)", 10},
		{"sum1 (x)", 4},
		{"sum2 (sum0(), x)", 8},
		{"sum3 (sum0(), x, 2)", 10},
		{"sum2(2,3)", 5},
		{"sum3(2,3,4)", 9},
		{"sum4(2,3,4,5)", 14},
		{"sum5(2,3,4,5,6)", 20},
		{"sum6(2,3,4,5,6,7)", 27},
		{"sum7(2,3,4,5,6,7,8)", 35},
	};

	x = 2;
	f = 5;

	// 动态token
	class FMathExpr2 : public FMathExpr
	{
	public:
		FMathExpr2(const std::vector<FCustomVariable>& InCustomTable)
			:FMathExpr()
		{
			CustomTable = InCustomTable;
		}
	protected:
		virtual FCustomVariable DynamicToken(const std::string& str) override
		{
			for (auto& It : CustomTable)
			{
				if (It.Name == str)
				{
					return It;
				}
			}
			return FCustomVariable{};
		}

		std::vector<FCustomVariable> CustomTable;
	};

	std::vector<FCustomVariable> CustomTable;
	{
		auto variables = lookup;
		auto var_count = sizeof(lookup) / sizeof(te_variable);
		CustomTable.reserve(var_count);
		for (int i = 0; i < var_count; i++)
		{
			CustomTable.push_back(variables[i]);
		}
	}
	FMathExpr2 Expr2(CustomTable);

	int i;
	for (i = 0; i < sizeof(cases) / sizeof(test_case); ++i) {
		const char *expr = cases[i].expr;
		const double answer = cases[i].answer;
		
		int Err = Expr2.Parse(false, expr);

		lok(Err == 0);
		auto ret = Expr2.Exec();
		lfequal_expr(ret, answer, expr);
	}
}