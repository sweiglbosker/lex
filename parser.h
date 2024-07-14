#ifndef _REGEX_PARSER_H
#define _REGEX_PARSER_H

#include "regex.h"

typedef enum {
	RULE_TERMINAL,
	RULE_RE, 	/* <CAT> <RER> */
	RULE_RER, 	/* TOKEN_UNION <RER> | ε */
	RULE_CAT,	/* <E> <CATR> */
	RULE_CATR, 	/* <E> <CATR> | ε */
	RULE_E,		/* <l> '+' | <l> '*' | <l> '?' | <l> */
	RULE_L,		/* TOKEN_LITERAL | TOKEN_CHARACTERCLASS | TOKEN_PERIOD | '(' <RE> ')' */
} RegexParserRule;

typedef struct RegexParseTree {
	RegexParserRule r;	/* role used to match symbol */ 

	RegexToken *token;

	struct RegexParseTree *neighbor;
	struct RegexParseTree *child;
} RegexParseTree;

RegexParseTree *regexparsetree_new(RegexParserRule);
void regexparsetree_free(RegexParseTree *);
void regexparsetree_print(RegexParseTree *);

RegexParseTree *ParseTerminalSymbol(RegexTokenList **);

RegexParseTree *ParseRE(RegexTokenList **);
RegexParseTree *ParseRER(RegexTokenList **);
RegexParseTree *ParseCAT(RegexTokenList **);
RegexParseTree *ParseCATR(RegexTokenList **);
RegexParseTree *ParseE(RegexTokenList **);
RegexParseTree *ParseL(RegexTokenList **);

#endif /* _REGEX_PARSER_H */
