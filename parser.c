#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "regex.h"

static const char *RULE_NAME[] = {
	[RULE_TERMINAL] = "RULE_TERMINAL",
	[RULE_RE] = "RULE_RE",
	[RULE_RER] = "RULE_RER", 
	[RULE_CAT] = "RULE_CAT",
	[RULE_CATR] = "RULE_CATR",
	[RULE_E] = "RULE_E",
	[RULE_L] = "RULE_L",
};

static void AppendChild(RegexParseTree *parent, RegexParseTree *child) {
	RegexParseTree *p = parent->child;

	if (!p) {
		parent->child = child;
		return;
	}

	while (p->neighbor) {
		p = p->neighbor;
	}

	p->neighbor = child;
}

RegexParseTree *regexparsetree_new(RegexParserRule r) {
	RegexParseTree *t = calloc(1, sizeof(RegexParseTree));
	t->r = r;

	return t;
}

void regexparsetree_free(RegexParseTree *t) {
	RegexParseTree *child, *neighbor;

	child = t->child; 
	neighbor = t->neighbor;
	free(t);

	if (child)
		regexparsetree_free(child);

	if (neighbor)
		regexparsetree_free(neighbor);

}

void regexparsetree_print(RegexParseTree *t) {
	RegexParseTree *child, *neighbor;

	child = t->child;
	neighbor = t->neighbor;

	printf("rule: %s\n", RULE_NAME[t->r]);
	if (t->r == RULE_TERMINAL) {
		printf("token: ");
		regextoken_print(t->token);
		putchar('\n');
	};

	if (child)
		regexparsetree_print(child);

	if (neighbor)
		regexparsetree_print(neighbor);
}

RegexParseTree *ParseTerminalSymbol(RegexTokenList **ptl) {
	RegexParseTree *t = regexparsetree_new(RULE_TERMINAL);	

	t->token = (*ptl)->t;
	*ptl = (*ptl)->next; 	/* advance token list */

	return t;
}

RegexParseTree *ParseRE(RegexTokenList **ptl) {
	RegexParseTree *t = regexparsetree_new(RULE_RE);

	AppendChild(t, ParseCAT(ptl));
	AppendChild(t, ParseRER(ptl));

	return t;
}

RegexParseTree *ParseRER(RegexTokenList **ptl) {
	RegexParseTree *t = regexparsetree_new(RULE_RER);

	if ((*ptl) == NULL) {
		return t;
	}

	if ((*ptl)->t->kind == TOKEN_UNION) {
		AppendChild(t, ParseTerminalSymbol(ptl));
		AppendChild(t, ParseRER(ptl));
	} 
		
	return t;
}

RegexParseTree *ParseCAT(RegexTokenList **ptl) {
	RegexParseTree *t = regexparsetree_new(RULE_CAT);

	AppendChild(t, ParseE(ptl));
	AppendChild(t, ParseCATR(ptl));

	return t;
}

RegexParseTree *ParseCATR(RegexTokenList **ptl) {
	RegexParseTree *t = regexparsetree_new(RULE_CATR);

	if ((*ptl) == NULL)
		return t;

	if ((*ptl)->t->kind == TOKEN_END) { /* TODO: is ')' ub? also this kinda feels ambiguous as it stands */
		return t;
	}

	AppendChild(t, ParseE(ptl));
	AppendChild(t, ParseCATR(ptl));

	return t;
}

RegexParseTree *ParseE(RegexTokenList **ptl) {
	RegexParseTree *t = regexparsetree_new(RULE_E);

	AppendChild(t, ParseL(ptl));

	if ((*ptl) == NULL) {
		return t;
	}

	switch ((*ptl)->t->kind) {
	case TOKEN_PLUS:
	case TOKEN_STAR:
	case TOKEN_QUESTION:
		AppendChild(t, ParseTerminalSymbol(ptl));
		break;
	default:
		break;
	}

	return t;
}

RegexParseTree *ParseL(RegexTokenList **ptl) {
	RegexParseTree *t = regexparsetree_new(RULE_L);

	if ((*ptl)->t->kind == TOKEN_LPAREN) { // we don't really need to parens in parse tree...
		AppendChild(t, ParseTerminalSymbol(ptl));
		AppendChild(t, ParseRE(ptl));
		AppendChild(t, ParseTerminalSymbol(ptl));

		return t;
	}

	AppendChild(t, ParseTerminalSymbol(ptl));
	
	return t;
}
