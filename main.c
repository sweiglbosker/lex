#include <stdio.h>

#include "dfa.h"
#include "nfa.h"
#include "regex.h"
#include "parser.h"

int main(void) {
	char *str = "bz*[a-z](l)";

	RegexScanner *rs = regex_scanner_new(str);

	RegexTokenList *rtl = regex_scanner_scan(rs); 

	regex_token_list_print(rtl);

	RegexTokenList **ptl = &rtl;

	printf("parsing...\n");
	RegexParseTree *pt = ParseRE(ptl);

	regexparsetree_print(pt);
}
