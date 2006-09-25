/*
 * $Header: /home/ncvs/php_extension/cTemplate/cTemplate.cpp,v 1.3 2006/09/25 13:40:00 vanilla Exp $
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

/* {{{ resource ctemplate (string filename, int strip_option, [string rootdir]) */
/**
 * @brief function to create new ctemplate object
 * @return object obj;
 * @param filename filename of your template.
 * @param strip_option strip option (could be NO_NOT_STRIP, STRIP_BLANK_LINES, STRIP_WHITESPACES)
 * @param rootdir option parameter, directory of your template files
 */
PHP_FUNCTION(ctemplate)
{
    cTemplate * ctemplate;
    Template *tpl = NULL;
    TemplateFromString *tpls = NULL;
    const char *fn = NULL, *dir = NULL, *name = NULL, *text = NULL;
    int fn_len, dir_len, name_len, text_len;
    long flags;

    if (zend_parse_parameters_ex( ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "sl|s", &fn, &fn_len, &flags, &dir, &dir_len) == SUCCESS)
    {
        if (dir != NULL)
            Template::SetTemplateRootDirectory (dir);
        else
            Template::SetTemplateRootDirectory ("./");

        tpl = Template::GetTemplate (fn, (Strip)flags);

        if (tpl == NULL)
        {
            php_error (E_ERROR, "Coult not create ctemplate object");
            RETURN_FALSE;
        }

        ctemplate = new cTemplate;
        ctemplate->t = tpl;
        ctemplate->s = NULL;
        ctemplate->fs = false;
        _add_resource (getThis(), ctemplate, le_cTemplate TSRMLS_CC);
        RETURN_TRUE;
    }
    else if (zend_parse_parameters_ex( ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "ssl", &name, &name_len, &text, &text_len, &flags) == SUCCESS)
    {
        Template::SetTemplateRootDirectory ("./");

        tpls = TemplateFromString::GetTemplate (name, text, (Strip)flags);

        if (tpls == NULL)
        {
            php_error (E_ERROR, "Coult not create ctemplate object");
            RETURN_FALSE;
        }

        ctemplate = new cTemplate;
        ctemplate->t = NULL;
        ctemplate->s = tpls;
        ctemplate->fs = true;
        _add_resource (getThis(), ctemplate, le_cTemplate TSRMLS_CC);
        RETURN_TRUE;
    }
    else
        RETURN_FALSE;
}
/* }}} */

/* {{{ bool = ctemplate_reload() */
/**
 * @brief reload template files if the source files changed.
 */
PHP_FUNCTION(ctemplate_reload)
{
    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    Template::ReloadAllIfChanged();
    RETURN_TRUE;
}
/* }}} */

/* {{{ bool ctemplate_clearcache() */
/**
 * @brief clear all cached template files.
 */
PHP_FUNCTION(ctemplate_clearcache)
{
    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    Template::ClearCache ();
    RETURN_TRUE;
}
/* }}} */

/* {{{ string = ctemplate->expand (cTemplateDict dict) */
/**
 * @class cTemplate
 * @brief expand the template.
 * @param dict cTemplateDict object.
 */
PHP_FUNCTION(ctemplate_expand)
{
    zval *d;
    cTemplate *ctemplate = NULL;
    cTemplateDict *cd = NULL;
    string ret;

    if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "o", &d) == FAILURE)
        RETURN_NULL ();

    ctemplate = (cTemplate *)_return_resource (getThis(), le_cTemplate TSRMLS_CC);
    if (ctemplate == NULL)
    {
        php_error (E_WARNING, "t_Expand: error getting object");
        RETURN_FALSE;
    }

    cd = (cTemplateDict *)_return_resource (d, le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "t_Expand: error getting dict object");
        RETURN_FALSE;
    }

    if (cd->root == true)
    {
        if (ctemplate->fs == true)
            ctemplate->s->Expand (&ret, &(cd->d));
        else
            ctemplate->t->Expand (&ret, &(cd->d));
    }
    else
    {
        if (ctemplate->fs == true)
            ctemplate->s->Expand (&ret, cd->p);
        else
            ctemplate->t->Expand (&ret, cd->p);
    }

    RETURN_STRINGL ((char *)ret.c_str(), ret.length(), 1);
}
/* }}} */

/* {{{ bool ctemplate->dump () */
/**
 * @class cTemplate
 * @brief
 * @retval TRUE
 * @retval FALSE
 */
PHP_FUNCTION(ctemplate_dump)
{
    cTemplate *ctemplate = NULL;

    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    ctemplate = (cTemplate *)_return_resource (getThis(), le_cTemplate TSRMLS_CC);
    if (ctemplate == NULL)
    {
        php_error (E_WARNING, "t_Dump: error getting object");
        RETURN_FALSE;
    }

    if (ctemplate->fs == true)
        ctemplate->s->Dump ("/dev/stdout");
    else
        ctemplate->t->Dump ("/dev/stdout");

    RETURN_TRUE;
}
/* }}} */

/* {{{ int = ctemplate->state () */
/**
 * @class cTemplate
 * @brief
 * @return
 */
PHP_FUNCTION(ctemplate_state)
{
    cTemplate *ctemplate = NULL;
    long state;

    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    ctemplate = (cTemplate *)_return_resource (getThis(), le_cTemplate TSRMLS_CC);
    if (ctemplate == NULL)
    {
        php_error (E_WARNING, "t_state: error getting object");
        RETURN_FALSE;
    }

    if (ctemplate->fs == true)
        state = ctemplate->s->state(); 
    else
        state = ctemplate->t->state();

    RETURN_LONG (state);
}
/* }}} */

/* {{{ string ctemplate->template_file ()*/
/**
 * @class cTemplate
 * @brief
 * @return string
 */
PHP_FUNCTION(ctemplate_template_file)
{
    cTemplate *ctemplate = NULL;

    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    ctemplate = (cTemplate *)_return_resource (getThis(), le_cTemplate TSRMLS_CC);
    if (ctemplate == NULL)
    {
        php_error (E_WARNING, "t_template_file: error getting object");
        RETURN_FALSE;
    }

    if (ctemplate->fs == true)
    {
        RETURN_STRING ("", 1);
    }
    else
    {
        RETURN_STRING ((char *)ctemplate->t->template_file(), 1);
    }
}
/* }}} */

/* {{{ bool = ctemplate->reloadifchanged()*/
/**
 * @class cTemplate
 * @brief
 * @retval TRUE
 * @retval FALSE
 */
PHP_FUNCTION(ctemplate_reloadifchanged)
{
    cTemplate *ctemplate = NULL;
    zend_bool b;

    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    ctemplate = (cTemplate *)_return_resource (getThis(), le_cTemplate TSRMLS_CC);
    if (ctemplate == NULL)
    {
        php_error (E_WARNING, "t_ReloadIfChanged: error getting object");
        RETURN_FALSE;
    }

    if (ctemplate->fs == true)
        b = true;
    else
        b = ctemplate->t->ReloadIfChanged();

    RETURN_BOOL (b);
}
/* }}} */

/* {{{ string ctemplate->writeheaderentries ()*/
/**
 * @class cTemplate
 * @brief
 * @return
 */
PHP_FUNCTION(ctemplate_writeheaderentries)
{
    cTemplate *ctemplate = NULL;
    string ret;

    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    ctemplate = (cTemplate *)_return_resource (getThis(), le_cTemplate TSRMLS_CC);
    if (ctemplate == NULL)
    {
        php_error (E_WARNING, "t_WriteHeaderEntries: error getting object");
        RETURN_FALSE;
    }

    if (ctemplate->fs == true)
        ctemplate->s->WriteHeaderEntries (&ret);
    else
        ctemplate->t->WriteHeaderEntries (&ret);

    RETURN_STRINGL ((char *)ret.c_str(), ret.length(), 1);
}
/* }}} */

/* {{{ object = new ctemplate_dict ()*/
/**
 * @brief
 * @return
 */
PHP_FUNCTION(ctemplate_dict)
{
    cTemplateDict * cd;

    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    cd = new cTemplateDict;

    if (cd != NULL)
    {
        _add_resource (getThis(), cd, le_cTemplateDict TSRMLS_CC);
        RETURN_TRUE;
    }
    RETURN_FALSE;
}
/* }}} */

/* {{{ bool = cTemplateDict->SetValue (string key, string val, [string section]) */
/**
 * @brief
 * @param key
 * @param val
 * @param section
 * @retval TRUE
 * @retval FALSE
 */
PHP_FUNCTION(d_SetValue)
{
    cTemplateDict *cd = NULL;
    const char *key = NULL, *sec = NULL;
    int key_len, sec_len;
    zval *val;

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_SetValue: error getting object");
        RETURN_FALSE;
    }

    if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "sz|s", &key, &key_len, &val, &sec, &sec_len) == FAILURE)
        RETURN_NULL ();

    if (Z_TYPE_P(val) == IS_STRING)
    {
        if (sec != NULL)
        {
            if (cd->root == true)
                cd->d.SetValueAndShowSection (key, Z_STRVAL_P(val), sec);
            else
                cd->p->SetValueAndShowSection (key, Z_STRVAL_P(val), sec);
        }
        else
        {
            if (cd->root == true)
                cd->d.SetValue (key, Z_STRVAL_P(val));
            else
                cd->p->SetValue (key, Z_STRVAL_P(val));
        }
        RETURN_TRUE;
    }
    else if (Z_TYPE_P(val) == IS_LONG)
    {
        if (sec != NULL)
        {
            if (cd->root == true)
            {
                cd->d.SetIntValue (key, Z_LVAL_P(val));
                cd->d.ShowSection (sec);
            }
            else
            {
                cd->p->SetIntValue (key, Z_LVAL_P(val));
                cd->p->ShowSection (sec);
            }
        }
        else
        {
            if (cd->root == true)
                cd->d.SetIntValue (key, Z_LVAL_P(val));
            else
                cd->p->SetIntValue (key, Z_LVAL_P(val));
        }
        RETURN_TRUE;
    }
    else
        RETURN_FALSE;
}
/* }}} */

/* {{{ bool cTemplateDict->SetEscapedValue (string key, string val, int escape_option) */
/**
 * @brief
 * @param key
 * @param val
 * @param escape_option
 * @retval TRUE
 * @retval FALSE
 */
PHP_FUNCTION(d_SetEscapedValue)
{
    cTemplateDict *cd = NULL;
    const char *key = NULL, *val = NULL, *sec = NULL;
    int key_len, val_len, sec_len;
    long e;

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_SetEscapedValue: error getting object");
        RETURN_FALSE;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssl|s", &key, &key_len, &val, &val_len, &e, &sec, &sec_len) == FAILURE)
        RETURN_NULL ();

    if (sec != NULL)
    {
        switch (e)
        {
            case 1:
                if (cd->root == true)
                    cd->d.SetEscapedValueAndShowSection (key, val, TemplateDictionary::xml_escape, sec);
                else
                    cd->p->SetEscapedValueAndShowSection (key, val, TemplateDictionary::xml_escape, sec);
                break;
            case 2:
                if (cd->root == true)
                    cd->d.SetEscapedValueAndShowSection (key, val, TemplateDictionary::javascript_escape, sec);
                else
                    cd->p->SetEscapedValueAndShowSection (key, val, TemplateDictionary::javascript_escape, sec);
                break;
            case 3:
                if (cd->root == true)
                    cd->d.SetEscapedValueAndShowSection (key, val, TemplateDictionary::json_escape, sec);
                else
                    cd->p->SetEscapedValueAndShowSection (key, val, TemplateDictionary::json_escape, sec);
                break;
            case 0:
            default:
                if (cd->root == true)
                    cd->d.SetEscapedValueAndShowSection (key, val, TemplateDictionary::html_escape, sec);
                else
                    cd->p->SetEscapedValueAndShowSection (key, val, TemplateDictionary::html_escape, sec);
        }
    }
    else
    {
        switch (e)
        {
            case 1:
                if (cd->root == true)
                    cd->d.SetEscapedValue (key, val, TemplateDictionary::xml_escape);
                else
                    cd->p->SetEscapedValue (key, val, TemplateDictionary::xml_escape);
                break;
            case 2:
                if (cd->root == true)
                    cd->d.SetEscapedValue (key, val, TemplateDictionary::javascript_escape);
                else
                    cd->p->SetEscapedValue (key, val, TemplateDictionary::javascript_escape);
                break;
            case 0:
            default:
                if (cd->root == true)
                    cd->d.SetEscapedValue (key, val, TemplateDictionary::html_escape);
                else
                    cd->p->SetEscapedValue (key, val, TemplateDictionary::html_escape);
        }
    }
    RETURN_TRUE;
}
/* }}} */

/* {{{ bool = cTemplateDict->SetGlobalValue (string key, string val) */
/**
 * @brief
 * @param key
 * @param val
 * @retval TRUE
 * @retval FALSE
 */
PHP_FUNCTION(d_SetGlobalValue)
{
    cTemplateDict *cd = NULL;
    const char *key = NULL, *val = NULL;
    int key_len, val_len;

    if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len, &val, &val_len) == FAILURE)
        RETURN_NULL ();;

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_SetGlobalValue: error getting object");
        RETURN_FALSE;
    }

    if (cd->root == true)
        cd->d.SetGlobalValue (key, val);
    else
        cd->p->SetGlobalValue (key, val);

    RETURN_TRUE;
}
/* }}} */

/* {{{ object cTemplateDict->AddSectionDictionary (string another_section) */
/**
 * @brief
 * @param another_section
 * @return
 */
PHP_FUNCTION(d_AddSectionDictionary)
{
    zval *rv;
    cTemplateDict *cd, *ce;
    const char *sec = NULL;
    int sec_len;

    if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &sec, &sec_len) == FAILURE)
        RETURN_NULL ();

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_AddSectionDictionary: error getting object");
        RETURN_FALSE;
    }

    ce = new cTemplateDict;
    if (cd->root == true)
        ce->p = cd->d.AddSectionDictionary(sec);
    else
        ce->p = cd->p->AddSectionDictionary(sec);
    ce->root = false;

    if (ce != NULL)
    {
        MAKE_STD_ZVAL (rv);
        _add_resource (rv, ce, le_cTemplateDict TSRMLS_CC);
        SEPARATE_ZVAL (&rv);
        *return_value = *rv;
        FREE_ZVAL (rv);
        return;
    }
    RETURN_FALSE;
}
/* }}} */

/* {{{ bool cTemplateDict->showsection (string section) */
/**
 * @brief
 * @param section
 * @retval TRUE
 * @retval FALSE
 */
PHP_FUNCTION(d_ShowSection)
{
    cTemplateDict *cd = NULL;
    const char *sec = NULL;
    int sec_len;

    if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &sec, &sec_len) == FAILURE)
        RETURN_NULL ();

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_ShowSection: error getting object");
        RETURN_FALSE;
    }

    if (cd->root == true)
        cd->d.ShowSection (sec);
    else
        cd->p->ShowSection (sec);
    RETURN_TRUE;
}
/* }}} */

/* {{{ object cTemplateDict->AddIncludeDictionary (string another_section) */
/**
 * @brief
 * @param another_section
 * @return
 */
PHP_FUNCTION(d_AddIncludeDictionary)
{
    zval *rv;
    cTemplateDict *cd, *ce;
    const char *sec = NULL;
    int sec_len;

    if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &sec, &sec_len) == FAILURE)
        RETURN_NULL ();

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_AddIncludeDictionary: error getting object");
        RETURN_FALSE;
    }

    ce = new cTemplateDict;
    if (cd->root == true)
        ce->p = cd->d.AddIncludeDictionary (sec);
    else
        ce->p = cd->p->AddIncludeDictionary (sec);
    ce->root = false;

    if (ce != NULL)
    {
        MAKE_STD_ZVAL (rv);
        _add_resource (rv, ce, le_cTemplateDict TSRMLS_CC);
        SEPARATE_ZVAL (&rv);
        *return_value = *rv;
        FREE_ZVAL (rv);
        return;
    }
    RETURN_FALSE;
}
/* }}} */

/* {{{ bool cTemplateDict->setfilename(string fn) */
/**
 * @brief
 * @param fn
 * @retval TRUE
 * @retval FALSE
 */
PHP_FUNCTION(d_SetFilename)
{
    cTemplateDict *cd = NULL;
    const char *fn = NULL;
    char *out = NULL;
    int fn_len;

    if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &fn, &fn_len) == FAILURE)
        RETURN_NULL ();

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_SetFilename: error getting object");
        RETURN_FALSE;
    }

    out = php_trim ((char *)fn, fn_len, NULL, 0, NULL, 3 TSRMLS_CC);

    if (cd->root == true)
        cd->d.SetFilename (out);
    else
        cd->p->SetFilename (out);

    efree (out);
    RETURN_TRUE;
}
/* }}} */

/* {{{ bool cTemplateDict->dump() */
/**
 * @brief
 * @retval FALSE
 * @retval TRUE
 */
PHP_FUNCTION(d_Dump)
{
    cTemplateDict * cd = NULL;

    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_Dump: error getting object");
        RETURN_FALSE;
    }

    if (cd->root == true)
        cd->d.Dump ();
    else
        cd->p->Dump ();
    RETURN_TRUE;
}
/* }}} */

/* {{{ string = cTemplateDict->dumpTostring() */
/**
 * @brief
 * @retval FALSE
 * @retval string 
 */
PHP_FUNCTION(d_DumpToString)
{
    cTemplateDict * cd = NULL;
    string ret;

    if (ZEND_NUM_ARGS() != 0)
    {
        WRONG_PARAM_COUNT;
    }

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_DumpToString: error getting object");
        RETURN_FALSE;
    }

    if (cd->root == true)
        cd->d.DumpToString (&ret);
    else
        cd->p->DumpToString (&ret);
    RETURN_STRINGL ((char *)ret.c_str(), ret.length(), 1);
}
/* }}} */

/* {{{ bool cTemplateDict->SetAnnotateOutput (string path) */
/**
 * @brief
 * @param path
 * @retval TRUE
 * @retval FALSE
 */
PHP_FUNCTION(d_SetAnnotateOutput)
{
    cTemplateDict * cd = NULL;
    const char *path = NULL;
    int path_len;

    if (zend_parse_parameters( ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE)
        RETURN_NULL ();

    cd = (cTemplateDict *)_return_resource (getThis(), le_cTemplateDict TSRMLS_CC);
    if (cd == NULL)
    {
        php_error (E_WARNING, "d_SetAnnotateOutput: error getting object");
        RETURN_FALSE;
    }

    if (cd->root == true)
        cd->d.SetAnnotateOutput (path);
    else
        cd->p->SetAnnotateOutput (path);
    RETURN_TRUE;
}
/* }}} */

/*
 * vim:ts=4:expandtab:sw=4
 */
