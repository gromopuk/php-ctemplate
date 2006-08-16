/*
 * $Header: /home/ncvs/php_extension/cTemplate/cObj.h,v 1.1 2006/08/07 15:45:09 vanilla Exp $
 */

#ifndef COBJ_H
#define COBJ_H

extern "C" {
#include "php.h"
}

#include "google/template.h"
#include "google/template_from_string.h"
#include "google/template_dictionary.h"

using namespace std;
using namespace google;

class cTemplate {

public:
    Template *t;
    TemplateFromString *s;
    bool fs;

    cTemplate();
    ~cTemplate();
};

class cTemplateDict {
public:
    TemplateDictionary d;
    TemplateDictionary *p;
    bool root;

    cTemplateDict ();
    ~cTemplateDict ();
};

#endif
