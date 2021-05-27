#define TEDLANG_SRC
#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <windows.h>

#include <tedlang/core/eval.h>
#include <tedlang/core/scope.h>
#include <tedlang/core/import.h>
#include <tedlang/parse/parser.h>
#include <tedlang/util/string.h>

#define MAX_IMPDIRS 1024
#define TEDLIB_FOLDER "tedlib\\"

char* pimpdirs[MAX_IMPDIRS] = { NULL };
size_t nimpdirs = 0;

void te_init_impdirs()
{
	// Currently only adding the tedlib directory

	char* impdir = (char*)malloc(sizeof(char) * MAX_PATH);
	if (!impdir)
	{
		te_seterr("Out of memory");
		return;
	}

	DWORD offset = GetModuleFileNameA(NULL, impdir, MAX_PATH);
	if (!offset)
	{
		DWORD err = GetLastError();
		te_seterr("Windows error: %lu", err);
		free(impdir);
		return;
	}

	while (impdir[--offset] != '\\' && impdir[offset] != '/');
	if (++offset + sizeof(TEDLIB_FOLDER) >= MAX_PATH)
	{
		te_seterr("tedlib path buffer overflow");
		return;
	}

	memcpy(impdir + offset, TEDLIB_FOLDER, sizeof(TEDLIB_FOLDER));
	void* ret = te_add_impdir(impdir);

	// giving the memory allocated in impdir to te_add_impdir
	if (!te_add_impdir(impdir))
	{
		free(impdir);
		te_seterr("Unable to initialize standard import directories");
	}
}

char* te_add_impdir(char* impdir)
{
	if (nimpdirs == MAX_IMPDIRS)
		return NULL;
	return pimpdirs[nimpdirs++] = impdir;
}

#define EXT_SZ 8
static char imppth[MAX_PATH + EXT_SZ];

bool file_exists(LPCTSTR szPath)
{
	DWORD dw_attrib = GetFileAttributes(szPath);
	return (
		dw_attrib != INVALID_FILE_ATTRIBUTES && (
			dw_attrib & (
				FILE_ATTRIBUTE_HIDDEN |
				FILE_ATTRIBUTE_NORMAL |
				FILE_ATTRIBUTE_READONLY
			)
		) && !(
			dw_attrib & FILE_ATTRIBUTE_DIRECTORY
		)
	);
}

te_module_et imp_exists(const char* pth, const te_ast_imp_st* pimp)
{
	static char tedext[EXT_SZ] = ".ted";
	static char dllext[EXT_SZ] = ".dll";

	memset(imppth, 0, MAX_PATH);

	// building the import path
	size_t modlvl = 0;
	size_t idx = te_strncpy(imppth, pth, MAX_PATH);
	while (true)
	{
		if (idx >= MAX_PATH - 1)
			return TE_MODULE_NONE;
		imppth[idx++] = '\\';
		idx += te_strncpy(imppth + idx, pimp->imp_pth[modlvl], MAX_PATH - idx);
		if (++modlvl == pimp->length)
			break;
	}

	DWORD fattrib;
	memcpy(imppth + idx, dllext, EXT_SZ);
	fattrib = GetFileAttributes(imppth);
	if (fattrib == INVALID_FILE_ATTRIBUTES)
		goto INVALID_FILE;
	if (fattrib & (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_NORMAL))
		return TE_MODULE_BIN;

	memcpy(imppth + idx, tedext, EXT_SZ);
	fattrib = GetFileAttributes(imppth);
	if (fattrib == INVALID_FILE_ATTRIBUTES)
		goto INVALID_FILE;
	if (fattrib & (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_NORMAL))
		return TE_MODULE_SCRIPT;

	return TE_MODULE_NONE;

INVALID_FILE:
	// TODO: better error message
	te_seterr("Found invalid module on import\n");
	return TE_MODULE_INV;
}

te_module_st* load_script(te_module_st* pmodule)
{
	FILE* pf = fopen(imppth, "rb");
	if (!pf)
		return te_seterr("Unable to open file %s\n", imppth);

	te_tarr_st tk_slice;
	int ret = _te_tarr_new(&tk_slice, 128);
	if (ret < 0)
	{
		fclose(pf);
		return te_seterr("Out of memory\n");
	}

	ret = te_lex_f(pf, &tk_slice);
	fclose(pf);
	if (ret < 0)
	{
		_te_tarr_del(&tk_slice);
		return te_seterr("Unable to lex %s\n", imppth);
	}

	te_ast_st* past;
	ret = te_parse_module(&tk_slice, &past);
	_te_tarr_del(&tk_slice);
	if (ret < 0)
		return te_seterr("Unable to parse %s\n", imppth);

	pmodule->past = past;
	return pmodule;
}

te_module_st* load_bin(te_module_st* pmodule)
{
	// TODO: Determine a dynamic link library structure for tedlang extensions
	return NULL;
}

te_module_st* module_load(te_module_st* pmodule, const te_ast_imp_st* pimp)
{
	te_module_et ty = TE_MODULE_NONE;
	for (size_t i = 0; i < nimpdirs; i++)
		if (pimpdirs[i])
		{
			ty = imp_exists(pimpdirs[i], pimp);
			if (ty)
				break;
		}
	
	switch (ty)
	{
	case TE_MODULE_NONE:
		return te_seterr("Unable to find path for import %s", pimp->imp_pth[pimp->length - 1]);
	case TE_MODULE_INV:
		return NULL;
	case TE_MODULE_SCRIPT:
		return load_script(pmodule);
	case TE_MODULE_BIN:
		return load_bin(pmodule);
	}

	assert(false && "imp_exists returned an invalid value");
}

te_scope_st* module_import(te_scope_st* pscope, const te_module_st* pmodule)
{
	switch (pmodule->ty)
	{
	case TE_MODULE_NONE:
		return te_seterr("Unable to import an uninitialized module");
	case TE_MODULE_INV:
		return NULL;
	case TE_MODULE_SCRIPT:
	{
		te_obj_st* pobj = te_eval(pscope, pmodule->past);
		if (te_haserr())
			return NULL;
		return pscope;
	}
	case TE_MODULE_BIN:
		return pmodule->loadmod(pscope);
	default:
		assert(false);
		return te_seterr("Unrecognized module type");
	}
}

void module_close(te_module_st* pmodule)
{
	// Currently only TE_MODULE_SCRIPT needs deallocation

	if (pmodule->ty == TE_MODULE_SCRIPT)
	{
		_te_ast_del(pmodule->past);
		free(pmodule->past);
	}
}
