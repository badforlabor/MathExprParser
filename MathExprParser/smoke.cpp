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



double te_interp(const char *expression, int *error) {
	using namespace mathexpr;
	FMathExpr One;
	int n = One.Parse(false, expression);
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
struct te_expr {

};
struct te_variable {

};
te_expr *te_compile(const char *expression, const te_variable *variables, int var_count, int *error) {

	using namespace mathexpr;
	FMathExpr One;
	int n = One.Parse(false, expression);
	*error = n;

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


	int i;
	for (i = 0; i < sizeof(cases) / sizeof(test_case); ++i) {
		const char *expr = cases[i].expr;
		const double answer = cases[i].answer;

		int err;
		const double ev = te_interp(expr, &err);
		lok(!err);
		lfequal(ev, answer);

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
		lequal(err, e);
		lok(r != r);

		te_expr *n = te_compile(expr, 0, 0, &err);
		lequal(err, e);
		lok(!n);

		if (err != e) {
			printf("FAILED: %s. err=%d, e=%d\n", expr, err, e);
		}

		const double k = te_interp(expr, 0);
		lok(k != k);
	}
}
