
#include <stdio.h>
#include <stdlib.h>


//用到c11标准的匿名结构体和匿名联合(anonymous structures and unions)
struct cst {
	enum {
		TERMINAL_SYMBOL,
		NON_TERMINAL_SYMBOL_SINGLE,
		NON_TERMINAL_SYMBOL_BINARY
	} tag;
	char *symbol;
	union {
		struct {
			struct cst *next;
		} single;
		struct {
			char *operator;
			struct cst *left;
			struct cst *right;
		} binary;
	};
};


struct cst *make_tetminal_symbol(char *symbol) {
	struct cst *a = malloc(sizeof(struct cst));
	a->tag = TERMINAL_SYMBOL;
	a->symbol = symbol;
	return a;
}

struct cst *make_nontetminal_symbol_signal(char *symbol, struct cst *next) {
	struct cst *a = malloc(sizeof(struct cst));
	a->tag = NON_TERMINAL_SYMBOL_SINGLE;
	a->symbol = symbol;
	a->single.next = next;
	return a;
}

struct cst *make_nontetminal_symbol_binary(char *symbol, char *operator, struct cst *left, struct cst *right)
{
	struct cst *a = malloc(sizeof(struct cst));
	a->tag = NON_TERMINAL_SYMBOL_BINARY;
	a->symbol = symbol;
	a->binary.operator = operator;
	a->binary.left = left;
	a->binary.right = right;
	return a;
}


void top_down_core(struct cst *c) 
{
	if (c->tag == TERMINAL_SYMBOL) {
		return;
	}

	if (c->tag == NON_TERMINAL_SYMBOL_SINGLE) {
		printf("%s ", c->single.next->symbol);
		top_down_core(c->single.next);
	}

	if (c->tag == NON_TERMINAL_SYMBOL_BINARY) {
		printf("%s ", c->binary.left->symbol);
		printf("%s ", c->binary.operator);
		printf("%s ", c->binary.right->symbol);
		top_down_core(c->binary.left);
		top_down_core(c->binary.right);	
	}
}

void top_down(struct cst *c)
{
	printf("%s ", c->symbol);
	top_down_core(c);
}


//相当于后序遍历
void bottom_up(struct cst *c)
{
	if (c->tag == TERMINAL_SYMBOL) {
		printf("%s ", c->symbol);
	}

	if (c->tag == NON_TERMINAL_SYMBOL_SINGLE) {
		bottom_up(c->single.next);
		printf("%s ", c->symbol);
	}

	if (c->tag == NON_TERMINAL_SYMBOL_BINARY) {
		bottom_up(c->binary.left);
		printf("%s ", c->binary.operator);
		bottom_up(c->binary.right);
		printf("%s ", c->symbol);
	}
}

int main(int argc, char **argv)
{


	struct cst *a = make_tetminal_symbol("a");
	struct cst *b = make_tetminal_symbol("b");
	struct cst *c = make_tetminal_symbol("c");
	struct cst *d = make_tetminal_symbol("d");


	struct cst *F1 = make_nontetminal_symbol_signal("F1", b);
	struct cst *T3 = make_nontetminal_symbol_signal("T3", F1);
	struct cst *E3 = make_nontetminal_symbol_signal("E3", T3);


	struct cst *F3 = make_nontetminal_symbol_signal("F3", c);
	struct cst *T4 = make_nontetminal_symbol_signal("T4", F3);

	struct cst *F2 = make_nontetminal_symbol_signal("F2", d);

	struct cst *T2 = make_nontetminal_symbol_binary("T2", "x", T4, F2);

	struct cst *E2 = make_nontetminal_symbol_binary("E2", "+", E3, T2);

	struct cst *F4 = make_nontetminal_symbol_signal("F4", a);

	struct cst *T1 = make_nontetminal_symbol_signal("T1", F4);


	struct cst *E1 = make_nontetminal_symbol_binary("E1", "+", E2, T1);
	printf("TopDown: ");
	top_down(E1);
	printf("\n");
	printf("BottomUp: ");
	bottom_up(E1);
	printf("\n");
	return 0;
}

