/*
 * $Header: /home/ncvs/php_extension/cTemplate/cObj.cpp,v 1.1 2006/08/07 15:45:09 vanilla Exp $
 */

#include "cObj.h"

cTemplate::cTemplate ()
    : t (), s (), fs (false)
{
}

cTemplate::~cTemplate ()
{
    if (s != NULL)
        s->ClearCache ();

    if (t != NULL)
        t->ClearCache ();
}

cTemplateDict::cTemplateDict ()
	: d ("default"), root (true)
{
}

cTemplateDict::~cTemplateDict()
{
}
