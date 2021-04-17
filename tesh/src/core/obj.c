#define TE_SRC

#include <tesh/core/obj.h>
#include <tesh/core/int.h>
#include <tesh/core/arr.h>
#include <tesh/core/str.h>


void te_incref(te_obj_st* pobj)
{
	pobj->n_ref++;
}

void te_decref(te_obj_st* pobj)
{
	if (!--pobj->n_ref)
		switch (pobj->ty)
		{
		case TE_INT:
			_te_int_dealloc(pobj);
			break;
		case TE_ARR:
			_te_arr_dealloc(pobj);
			break;
		case TE_STR:
			_te_str_dealloc(pobj);
			break;
		}
}

void te_passarg(te_obj_st* pObj)
{
	// TODO: Implementation
}
