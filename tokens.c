#include "tokens.h"

int matches_obracket(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '[';
}

int matches_cbracket(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == ']';
}

int matches_obrace(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '{';
}

int matches_cbrace(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '}';
}

int matches_oparen(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '(';
}

int matches_cparen(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == ')';
}

int matches_plus(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '+';
}

int matches_minus(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '-';
}

int matches_star(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '*';
}

int matches_semicolon(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == ';';		
}

int matches_underscore(const char *str)
{
	if (!strlen(str)) return 0;
	return *str == '_';
}

int matches_bang(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '!';
}

int matches_func(const char * str)
{
	if (strlen(str) != strlen("func")) return 0;
	return !strcmp(str, "func");
}

int matches_comma(const char *str)
{
	if (!strlen(str)) return 0;
	return *str == ',';
}


int matches_fslash(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '/';
}

int matches_bslash(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '\'';
}

int matches_and(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '&';
}

int matches_or(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '|';
}

int matches_at(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '@';
}

int matches_quote(const char * str)
{
	if (!strlen(str)) return 0;
	return *str == '\"';
}

int matches_apostrophe(const char *str)
{
	if (!strlen(str)) return 0;
	return *str == '\'';
}

int is_alpha(const char * str)
{
	if (!strlen(str)) return 0;
	return IS_ALPHA(*str);
}

int is_digit(const char * str)
{
	if (!strlen(str)) return 0;
	return IS_DIGIT(*str);
}
