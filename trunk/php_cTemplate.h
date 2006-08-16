/*
 * $Header: /home/ncvs/php_extension/cTemplate/php_cTemplate.h,v 1.1 2006/08/07 15:45:09 vanilla Exp $
 */

#ifndef PHP_CTEMPLATE_H
#define PHP_CTEMPLATE_H

extern zend_module_entry cTemplate_module_entry;
#define phpext_cTemplate_ptr &cTemplate_module_entry

#ifdef PHP_WIN32
#define PHP_CTEMPLATE_API __declspec(dllexport)
#else
#define PHP_CTEMPLATE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

extern int le_cTemplate;
extern int le_cTemplateDict;
extern void * _return_resource (zval *wrapper, int rsrc_type TSRMLS_DC);
extern void _add_resource (zval *wrapper, void *obj, int rsrc_type TSRMLS_DC);

PHP_MINIT_FUNCTION(cTemplate);
PHP_MSHUTDOWN_FUNCTION(cTemplate);
PHP_RSHUTDOWN_FUNCTION(cTemplate);
PHP_MINFO_FUNCTION(cTemplate);

PHP_FUNCTION(ctemplate);
PHP_FUNCTION(ctemplate_reload);
PHP_FUNCTION(ctemplate_clearcache);
PHP_FUNCTION(ctemplate_expand);
PHP_FUNCTION(ctemplate_dump);
PHP_FUNCTION(ctemplate_state);
PHP_FUNCTION(ctemplate_template_file);
PHP_FUNCTION(ctemplate_reloadifchanged);
PHP_FUNCTION(ctemplate_writeheaderentries);

PHP_FUNCTION(ctemplate_dict);
PHP_FUNCTION(d_SetValue);
PHP_FUNCTION(d_SetEscapedValue);
PHP_FUNCTION(d_SetGlobalValue);
PHP_FUNCTION(d_AddSectionDictionary);
PHP_FUNCTION(d_ShowSection);
PHP_FUNCTION(d_AddIncludeDictionary);
PHP_FUNCTION(d_SetFilename);
PHP_FUNCTION(d_Dump);
PHP_FUNCTION(d_DumpToString);
PHP_FUNCTION(d_SetAnnotateOutput);

#ifdef ZTS
#define CTEMPLATE_G(v) TSRMG(cTemplate_globals_id, zend_cTemplate_globals *, v)
#else
#define CTEMPLATE_G(v) (cTemplate_globals.v)
#endif

#endif	/* PHP_CTEMPLATE_H */

