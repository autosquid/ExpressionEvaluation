#include <cstdio>
#include <utility>
#include <algorithm>
#include <iostream>
/*
exp -> exp + term | exp - term | term
term -> term * factor | term / factor | factor
factor -> digit | (exp)
*/

using std::pair;

typedef pair<bool, double> RET;

double simple_op(double a, double b, char op)
{
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    return double(a) / b;
}

std::string slice(char* str, int s, int e)
{
    return std::string(str).substr(s, e-s+1);
}


RET try_cal_factor(char* factor, int s, int e);
RET try_cal_term(char* term, int s, int e);
RET try_cal_expr(char* expr, int s, int e);

RET try_cal_expr(char* expr, int s, int e)
{
    RET term_ret = try_cal_term(expr, s, e);
    if (term_ret.first) return term_ret;

    for (int i=s; i<=e; ++i)
    {
        if (expr[i] == '+' || expr[i] == '-')
        {
            RET F_EXPR = try_cal_expr(expr, s, i-1);
            RET S_TERM = try_cal_term(expr, i+1, e);

            if (F_EXPR.first && S_TERM.first)
            {
                return std::make_pair(true, simple_op(F_EXPR.second, S_TERM.second, expr[i]));
            }
        }
    }
    return std::make_pair(false, 0);
}

RET try_cal_term(char* term, int s, int e)
{
    RET factor_ret = try_cal_factor(term, s, e);
    if (factor_ret.first)
        return factor_ret;

    for (int i=s; i<=e; ++i)
    {
        if (term[i] == '*' || term[i] == '/')
        {
            RET F_TERM = try_cal_term(term, s, i-1);
            RET S_FACTOR = try_cal_factor(term, i+1, e);

            if (F_TERM.first && S_FACTOR.first)
            {
                return std::make_pair(true, simple_op(F_TERM.second, S_FACTOR.second, term[i]));
            }
        }
    }

    return std::make_pair(false, 0);
}

RET try_cal_factor(char* factor, int s, int e)
{
    if (factor[s]=='(' and factor[e] == ')')
        return try_cal_expr(factor, s+1, e-1);
    if (s == e  && isdigit(factor[s]))
        return std::make_pair(true, factor[s] - '0');
    return std::make_pair(false, 0);
}


int main()
{
    char expr[256];
    scanf("%s", expr);
    std::cout<<strlen(expr)<<std::endl;
    printf("%lf\n", try_cal_expr(expr, 0, strlen(expr)-1).second);
}

