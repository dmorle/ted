#define TEDLANG_SRC

#include <assert.h>

#include <tedlang/core/eval.h>

te_scope_st global_scope = {
	NULL,
	0,
	0,
	0,
	NULL
};

void te_init()
{

}

void* te_seterr(const char* err, ...)
{

}

te_obj_st** te_get_lval(te_scope_st* pscope, const te_ast_st* past)
{
}

te_obj_st* eval_empty(te_scope_st* pscope, const te_ast_noexpr_st* pempty)
{
}

te_obj_st* eval_var(te_scope_st* pscope, const te_ast_var_st* pvar)
{
}

te_obj_st* eval_null(te_scope_st* pscope, const te_ast_null_st* pnull)
{
}

te_obj_st* eval_bool(te_scope_st* pscope, const te_ast_bool_st* pbool)
{
}

te_obj_st* eval_int(te_scope_st* pscope, const te_ast_int_st* pint)
{
}

te_obj_st* eval_str(te_scope_st* pscope, const te_ast_str_st* pstr)
{
}

te_obj_st* eval_arr(te_scope_st* pscope, const te_ast_arr_st* parr)
{
}

te_obj_st* eval_seq(te_scope_st* pscope, const te_ast_seq_st* pseq)
{
}

te_obj_st* eval_for(te_scope_st* pscope, const te_ast_for_st* pfor)
{
}

te_obj_st* eval_while(te_scope_st* pscope, const te_ast_while_st* pwhile)
{
}

te_obj_st* eval_break(te_scope_st* pscope, const te_ast_break_st* pbreak)
{
}

te_obj_st* eval_continue(te_scope_st* pscope, const te_ast_continue_st* pcontinue)
{
}

te_obj_st* eval_branch(te_scope_st* pscope, const te_ast_branch_st* pbranch)
{
}

te_obj_st* eval_return(te_scope_st* pscope, const te_ast_return_st* preturn)
{
}

te_obj_st* eval_call(te_scope_st* pscope, const te_ast_call_st* pcall)
{
}

te_obj_st* eval_fn(te_scope_st* pscope, const te_ast_fn_st* pfn)
{
}

te_obj_st* eval_imp(te_scope_st* pscope, const te_ast_imp_st* pimp)
{
}

te_obj_st* eval_module(te_scope_st* pscope, const te_ast_module_st* pmodule)
{
}

te_obj_st* eval_assign(te_scope_st* pscope, const te_ast_assign_st* passign)
{
}

te_obj_st* eval_iadd(te_scope_st* pscope, const te_ast_iadd_st* piadd)
{
}

te_obj_st* eval_isub(te_scope_st* pscope, const te_ast_isub_st* pisub)
{
}

te_obj_st* eval_imul(te_scope_st* pscope, const te_ast_imul_st* pimul)
{
}

te_obj_st* eval_idiv(te_scope_st* pscope, const te_ast_idiv_st* pidiv)
{
}

te_obj_st* eval_imod(te_scope_st* pscope, const te_ast_imod_st* pimod)
{
}

te_obj_st* eval_iexp(te_scope_st* pscope, const te_ast_iexp_st* piexp)
{
}

te_obj_st* eval_eq(te_scope_st* pscope, const te_ast_eq_st* peq)
{
}

te_obj_st* eval_ne(te_scope_st* pscope, const te_ast_ne_st* pne)
{
}

te_obj_st* eval_lt(te_scope_st* pscope, const te_ast_lt_st* plt)
{
}

te_obj_st* eval_gt(te_scope_st* pscope, const te_ast_gt_st* pgt)
{
}

te_obj_st* eval_le(te_scope_st* pscope, const te_ast_le_st* ple)
{
}

te_obj_st* eval_ge(te_scope_st* pscope, const te_ast_ge_st* pge)
{
}

te_obj_st* eval_and(te_scope_st* pscope, const te_ast_and_st* pand)
{
}

te_obj_st* eval_or(te_scope_st* pscope, const te_ast_or_st* por)
{
}

te_obj_st* eval_add(te_scope_st* pscope, const te_ast_add_st* padd)
{
}

te_obj_st* eval_sub(te_scope_st* pscope, const te_ast_sub_st* psub)
{
}

te_obj_st* eval_mul(te_scope_st* pscope, const te_ast_mul_st* pmul)
{
}

te_obj_st* eval_div(te_scope_st* pscope, const te_ast_div_st* pdiv)
{
}

te_obj_st* eval_mod(te_scope_st* pscope, const te_ast_mod_st * pexp)
{
}

te_obj_st* eval_exp(te_scope_st* pscope, const te_ast_exp_st* pexp)
{
}

te_obj_st* eval_idx(te_scope_st* pscope, const te_ast_idx_st* pidx)
{
}

te_obj_st* te_eval(te_scope_st* pscope, const te_ast_st* past)
{
	// polymorphism in C!
	switch (past->ast_ty)
	{
	case AST_EMPTY:     return eval_empty    (pscope, past);
	case AST_VAR:       return eval_var      (pscope, past);
	case AST_NULL:      return eval_null     (pscope, past);
	case AST_BOOL:      return eval_bool     (pscope, past);
	case AST_INT:       return eval_int      (pscope, past);
	case AST_STR:       return eval_str      (pscope, past);
	case AST_ARR:       return eval_arr      (pscope, past);
	case AST_SEQ:       return eval_seq      (pscope, past);
	case AST_FOR:       return eval_for      (pscope, past);
	case AST_WHILE:     return eval_while    (pscope, past);
	case AST_BREAK:     return eval_break    (pscope, past);
	case AST_CONTINUE:  return eval_continue (pscope, past);
	case AST_BRANCH:    return eval_branch   (pscope, past);
	case AST_RETURN:    return eval_return   (pscope, past);
	case AST_CALL:      return eval_call     (pscope, past);
	case AST_FN:        return eval_fn       (pscope, past);
	case AST_IMP:       return eval_imp      (pscope, past);
	case AST_MODULE:    return eval_module   (pscope, past);
	case AST_ASSIGN:    return eval_assign   (pscope, past);
	case AST_IADD:      return eval_iadd     (pscope, past);
	case AST_ISUB:      return eval_isub     (pscope, past);
	case AST_IMUL:      return eval_imul     (pscope, past);
	case AST_IDIV:      return eval_idiv     (pscope, past);
	case AST_IMOD:      return eval_imod     (pscope, past);
	case AST_IEXP:      return eval_iexp     (pscope, past);
	case AST_EQ:        return eval_eq       (pscope, past);
	case AST_NE:        return eval_ne       (pscope, past);
	case AST_LT:        return eval_lt       (pscope, past);
	case AST_GT:        return eval_gt       (pscope, past);
	case AST_LE:        return eval_le       (pscope, past);
	case AST_GE:        return eval_ge       (pscope, past);
	case AST_AND:       return eval_and      (pscope, past);
	case AST_OR:        return eval_or       (pscope, past);
	case AST_ADD:       return eval_add      (pscope, past);
	case AST_SUB:       return eval_sub      (pscope, past);
	case AST_MUL:       return eval_mul      (pscope, past);
	case AST_DIV:       return eval_div      (pscope, past);
	case AST_MOD:       return eval_mod      (pscope, past);
	case AST_EXP:       return eval_exp      (pscope, past);
	case AST_IDX:       return eval_idx      (pscope, past);
	default:
		assert(false);
		return NULL;  // tmp
	}
}
