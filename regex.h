#ifndef _REGEX_H
#define _REGEX_H

#include <stdbool.h>
#include "nfa.h"

typedef struct CharacterClass {
	bool negated;
	char min, max;
	struct CharacterClass *next;
} CharacterClass;

typedef enum {
	TOKEN_END,
	TOKEN_LITERAL,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_CHARACTERCLASS,
	TOKEN_PLUS,
	TOKEN_UNION,
	TOKEN_STAR,
	TOKEN_PERIOD,
} RegexTokenKind;

typedef struct {
	RegexTokenKind kind;
	union {
		char val;
		CharacterClass *cc;
	};
} RegexToken;

typedef struct RegexTokenList {
	RegexToken *t;
	struct RegexTokenList *next;
} RegexTokenList;

typedef struct RegexScanner {
	struct {
		char *str;
		int len;
	} regexp;
	int i;
} RegexScanner;

RegexScanner *regex_scanner_new(char *str);
RegexToken *regex_scanner_advance(RegexScanner *r);
RegexTokenList *regex_scanner_scan(RegexScanner *r);

NFA *regex_to_nfa(RegexTokenList *tl);

void regex_token_list_print(RegexTokenList *tl);

#endif /* _REGEX_H */
