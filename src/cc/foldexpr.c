#include <u.h>
#include <gc/gc.h>
#include <ds/ds.h>
#include "c.h"

static Const *
mkconst(char *p, int64 v)
{
	Const *c;

	c = gcmalloc(sizeof(Const));
	c->p = p;
	c->v = v;
	return c;
}

static Const *
foldbinop(Node *n)
{
	Const *l, *r;

	l = foldexpr(n->Binop.l);
	r = foldexpr(n->Binop.l);
	if(!l || !r)
		return 0;
	if(isitype(n->type)) {
		switch(n->Binop.op) {
		case '+':
			return mkconst(0, l->v + r->v);
		case '-':
			return mkconst(0, l->v - r->v);
		case '*':
			return mkconst(0, l->v * r->v);
		case '/':
			if(r->v == 0)
				return 0;
			return mkconst(0, l->v / r->v);
		default:
			panic("unimplemented fold binop %d", n->Binop.op);
		}
	}
	panic("unimplemented fold binop");
	return 0;
}

static Const *
foldunop(Node *n)
{
	Const *c;

	c = foldexpr(n->Unop.operand);
	if(!c)
		return 0;

	if(isitype(n->type)) {
		switch(n->Binop.op) {
		case '-':
			return mkconst(0, -c->v);
		default:
			panic("unimplemented fold unop %d", n->Binop.op);
		}
	}
	panic("unimplemented fold unop");
	return 0;
}

Const *
foldexpr(Node *n)
{
	switch(n->t) {
	case NBINOP:
		return foldbinop(n);
	case NUNOP:
		return foldunop(n);
	case NNUM:
		return mkconst(0, n->Num.v);
	default:
		return 0;
	}
}