/*
 * $Header: /home/ncvs/php_extension/cTemplate/php_cTemplate.cpp,v 1.1 2006/08/07 15:45:09 vanilla Exp $
 */

/* {{{ include files */
extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "php_cTemplate.h"
}

#include "cObj.h"
/* }}} */

/* {{{ static variable */
int le_cTemplate;
int le_cTemplateDict;
static zend_class_entry *ctemplate_class_entry_ptr;
static zend_class_entry *ctemplate_dict_class_entry_ptr;
/* }}} */

/* {{{ function_entry */
zend_function_entry cTemplate_functions[] = {
    PHP_FE (ctemplate_reload, NULL)
    PHP_FE (ctemplate_clearcache, NULL)
    {NULL, NULL, NULL}
};

static zend_function_entry ctemplate_class_functions[] = {
    PHP_FE (ctemplate, NULL)
    PHP_FALIAS (expand, ctemplate_expand, NULL)
    PHP_FALIAS (dump, ctemplate_dump, NULL)
    PHP_FALIAS (state, ctemplate_state, NULL)
    PHP_FALIAS (template_file, ctemplate_template_file, NULL)
    PHP_FALIAS (reloadifchanged, ctemplate_reloadifchanged, NULL)
    PHP_FALIAS (writeheaderentries, ctemplate_writeheaderentries, NULL)
    {NULL, NULL, NULL}
};

static zend_function_entry ctemplate_dict_class_functions[] = {
    PHP_FE (ctemplate_dict, NULL)
    PHP_FALIAS (set, d_SetValue, NULL)
    PHP_FALIAS (setescaped, d_SetEscapedValue, NULL)
    PHP_FALIAS (setglobal, d_SetGlobalValue, NULL)
    PHP_FALIAS (addsection, d_AddSectionDictionary, NULL)
    PHP_FALIAS (show, d_ShowSection, NULL)
    PHP_FALIAS (addinclude, d_AddIncludeDictionary, NULL)
    PHP_FALIAS (filename, d_SetFilename, NULL)
    PHP_FALIAS (dump, d_Dump, NULL)
    PHP_FALIAS (dumptostring, d_DumpToString, NULL)
    PHP_FALIAS (setannotateoutput, d_SetAnnotateOutput, NULL)
    {NULL, NULL, NULL}
};
/* }}} */

/* {{{ module entry */
zend_module_entry cTemplate_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "cTemplate",
    cTemplate_functions,
    PHP_MINIT(cTemplate),
    PHP_MSHUTDOWN(cTemplate),
    NULL,
    NULL,
    PHP_MINFO(cTemplate),
#if ZEND_MODULE_API_NO >= 20010901
    "$Revision: 1.1 $",
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

/* {{{ resource allocate function */
void * _return_resource (zval *wrapper, int rsrc_type TSRMLS_DC)
{
    void *obj = NULL;
    zval **handle;
    int type;

    if (Z_TYPE_P(wrapper) != IS_OBJECT)
    {
        php_error (E_ERROR, "Wrapper is not an object");
    }

    if (zend_hash_find (Z_OBJPROP_P(wrapper), "id", sizeof ("id"), (void **) &handle) == FAILURE)
    {
        php_error (E_ERROR, "Underlying object missing1");
    }

    obj = zend_list_find (Z_LVAL_PP (handle), &type);
    if (!obj || (type != rsrc_type))
    {
        php_error (E_ERROR, "Underlying object missing2");
    }
    return obj;
}

static void _free_ctemplate_resource (zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    cTemplate *obj = static_cast <cTemplate *> (rsrc->ptr);

    if (obj)
    {
        delete obj;
        rsrc->ptr = NULL;
    }
}

static void _free_ctemplate_dict_resource (zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    cTemplateDict *obj = static_cast <cTemplateDict *> (rsrc->ptr);

    if (obj)
    {
        delete obj;
        rsrc->ptr = NULL;
    }
}

void _add_resource (zval *wrapper, void *obj, int rsrc_type TSRMLS_DC)
{
    zval *handle = NULL;

    if (rsrc_type == le_cTemplate)
        object_init_ex (wrapper, ctemplate_class_entry_ptr);
    else if (rsrc_type == le_cTemplateDict)
        object_init_ex (wrapper, ctemplate_dict_class_entry_ptr);

    MAKE_STD_ZVAL (handle);
    ZEND_REGISTER_RESOURCE (handle, obj, rsrc_type);
    zend_hash_update (Z_OBJPROP_P (wrapper), "id", sizeof ("id"), &handle, sizeof (zval *), NULL);
}

/* }}} */

/* {{{ INIT */
extern "C" {
#ifdef COMPILE_DL_CTEMPLATE
ZEND_GET_MODULE(cTemplate)
#endif
}

PHP_MINIT_FUNCTION(cTemplate)
{
    zend_class_entry ctemplate_ce;
    zend_class_entry ctemplate_dict_ce;
    
    le_cTemplate = zend_register_list_destructors_ex (_free_ctemplate_resource, NULL, "ctemplate", module_number);
    le_cTemplateDict = zend_register_list_destructors_ex (_free_ctemplate_dict_resource, NULL, "ctemplate_dict", module_number);

    INIT_CLASS_ENTRY (ctemplate_ce, "ctemplate", ctemplate_class_functions);
    INIT_CLASS_ENTRY (ctemplate_dict_ce, "ctemplate_dict", ctemplate_dict_class_functions);

    ctemplate_class_entry_ptr = zend_register_internal_class (&ctemplate_ce TSRMLS_CC);
    ctemplate_dict_class_entry_ptr = zend_register_internal_class (&ctemplate_dict_ce TSRMLS_CC);

    REGISTER_LONG_CONSTANT ("DO_NOT_STRIP", DO_NOT_STRIP, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("STRIP_BLANK_LINES", STRIP_BLANK_LINES, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("STRIP_WHITESPACE", STRIP_WHITESPACE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("NUM_STRIPS", NUM_STRIPS, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("TS_UNUSED", TS_UNUSED, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("TS_EMPTY", TS_EMPTY, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("TS_ERROR", TS_ERROR, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("TS_READY", TS_READY, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("TS_RELOAD", TS_RELOAD, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("HTML_ESCAPE", 0, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("XML_ESCAPE", 1, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT ("JAVASCRIPT_ESCAPE", 2, CONST_CS | CONST_PERSISTENT);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(cTemplate)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(cTemplate)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "cTemplate support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
