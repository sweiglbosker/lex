#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regex.h"


static const char *REGEX_TOKEN_NAME[] = {
	[TOKEN_END] = "END",
	[TOKEN_LITERAL] = "LITERAL",
	[TOKEN_LPAREN] = "LPAREN",
	[TOKEN_RPAREN] = "RPAREN",
	[TOKEN_CHARACTERCLASS] = "CHARACTERCLASS",
	[TOKEN_PLUS] = "PLUS",
	[TOKEN_UNION] = "UNION",
	[TOKEN_STAR] = "*",
	[TOKEN_PERIOD] = ".",
	[TOKEN_QUESTION] = "?"
};

static void characterclass_print(CharacterClass *cc);

static int escaped_char(int c) {
	switch (c) {
	case 'n':
		return '\n';
	case '0':
		return '\0';
	default:
		return c;
	}
}

/* doing this while scanning makes things alot simpler */ 
static CharacterClass *scan_cc(RegexScanner *r) { 
	bool done = false;
	int c = r->regexp.str[r->i];
	bool negated = false;
	CharacterClass *head = NULL;

	if (r->regexp.str[(r->i)] == '^') {
		negated = true;
		r->i++;
	}

	while (!done) {
		if (r->i >= r->regexp.len) {
			fprintf(stderr, "error: missing closing bracket\n");
			return NULL;
		}

		c = r->regexp.str[r->i++]; // 1st char of regexp		
		
		CharacterClass *node = calloc(1, sizeof(CharacterClass));
		node->next = head;
		head = node;

		
		if (c == '^') {
			node->negated = true;
		} else {
			if (c == '\\') {
				assert(r->i < r->regexp.len);
				c = r->regexp.str[r->i++]; // 1st char of regexp		
			} 
			node->min = c;
		}

		c = r->regexp.str[r->i++]; // 2nd char of regexp
		switch(c) {
		case '-':
			assert(r->i + 1 <= r->regexp.len);
			node->max = r->regexp.str[r->i++];
			
			if (node->max < node->min) {
				fprintf(stderr, "error: invalid range (%c-%c)\n", node->min, node->max);
				/* might want to free list here */
				return NULL;
			}
			break;
		default: 
			node->max = node->min;
			break;
		}

		c = r->regexp.str[r->i];
		
		node->negated = negated;
		if (r->i >= r->regexp.len) {
			fprintf(stderr, "error: expected closing brace\n");
			/* might want to free list here */
			return NULL;
		}
		if (c == ']') {
			r->i++;
			break;
		}
	}
	return head;
}

RegexScanner *regex_scanner_new(char *str) {
	RegexScanner *r = calloc(1, sizeof(RegexScanner));

	r->regexp.str = str;
	r->regexp.len = strlen(str);
	r->i = 0;

	return r;
}

RegexToken *regex_scanner_advance(RegexScanner *r) {
	RegexToken *t = calloc(1, sizeof(RegexToken));

	if (r->i > r->regexp.len) {
		free(t);
		return NULL;
	}

	int c = r->regexp.str[r->i++];
	
	switch (c) {
	case '\0': 
		t->kind = TOKEN_END;
		break;
	case '\\':
		if (r->i >= r->regexp.len) {
			free(t);
			return NULL;
		}
		t->kind = TOKEN_LITERAL;
		t->val = escaped_char(r->regexp.str[r->i]);
		break;
	case '|':
		t->kind = TOKEN_UNION;
		break;
	case '+':
		t->kind = TOKEN_PLUS;
		break;
	case '*':
		t->kind = TOKEN_STAR;
		break;
	case '(':
		t->kind = TOKEN_LPAREN;
		break;
	case ')':
		t->kind = TOKEN_RPAREN;
		break;
	case '.':
		t->kind = TOKEN_PERIOD;
		break;
	case '?':
		t->kind = TOKEN_QUESTION;
		break;
	case '[':
		t->kind = TOKEN_CHARACTERCLASS;
		t->cc = scan_cc(r);
		break;
	case ']':
		fprintf(stderr, "error: extraneous closing bracket\n");
		break;
	default:
		t->kind = TOKEN_LITERAL;
		t->val = c;
		break;
	}

	if (!(t->kind)) {
		t->kind = TOKEN_END;
	}

	return t;
}

RegexTokenList *regex_scanner_scan(RegexScanner *r) {
	RegexTokenList *head = calloc(1, sizeof(RegexTokenList));
	RegexTokenList *tail = head;
	RegexTokenList *prev = NULL;


	while (true) {
		tail->t = regex_scanner_advance(r);

		if (tail->t == NULL && prev) {
			free(tail);
			prev->next = NULL;
			break;
		}
		if (tail->t == TOKEN_END) {
			tail->next = NULL;
			break;
		}
		

		tail->next = calloc(1, sizeof(RegexTokenList));
		prev = tail;
		tail = tail->next;
	}

	return head;
}

static void characterclass_print(CharacterClass *cc) {
	while (cc) {
		if (cc->max == cc->min)
			printf("(%c) ", cc->min);
		else {
			if (cc->negated) 
				putchar('!');

			printf("(%c-%c)\n", cc->min, cc->max);
		}

		cc = cc->next;
	}
}

void regextoken_print(RegexToken *t) {
	switch (t->kind) {
	case TOKEN_LITERAL:
		printf("kind: LITERAL, val: %c\n", t->val);
		break;
	case TOKEN_CHARACTERCLASS:
		printf("kind: CHARACTERCLASS, val: \n");
		characterclass_print(t->cc);
		break;
	default:
		printf("kind: %s\n", REGEX_TOKEN_NAME[t->kind]);
	}
}

void regex_token_list_print(RegexTokenList *tl) {
	while (tl->next) {
		regextoken_print(tl->t);
		tl = tl->next;
	}
}
