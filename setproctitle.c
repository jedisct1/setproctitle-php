/* $Id$ */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include "php.h"
#include "SAPI.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_setproctitle.h"

static int le_setproctitle;
static char *argv0 = NULL;
static size_t argv_lth;

const zend_function_entry setproctitle_functions[] = {
   PHP_FE(setproctitle,	NULL)
   { NULL, NULL, NULL }
};

zend_module_entry setproctitle_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
   STANDARD_MODULE_HEADER,
#endif
   "setproctitle",
   setproctitle_functions,
#ifndef PHP_SYSTEM_PROVIDES_SETPROCTITLE   
   PHP_MINIT(setproctitle),
#else
   NULL,
#endif
   NULL,
   NULL,
   NULL,
   PHP_MINFO(setproctitle),
#if ZEND_MODULE_API_NO >= 20010901
   "0.1",
#endif
   STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SETPROCTITLE
ZEND_GET_MODULE(setproctitle)
#endif

static void clearargs(void)
{
#if defined(__linux__) && !defined(HAVE_SETPROCTITLE)
    extern char **environ;
    char        **new_environ;
    char         *next = argv0;
    char         *env0 = environ[0];
    unsigned int  env_nb = 0U;

    while (next != env0) {
	puts(next);
	next += strlen(next) + 1U;
    }
    while (*next != 0) {
	puts(next);
	next += strlen(next) + 1U;
    } 
    argv_lth = next - argv0;
    while (environ[env_nb] != NULL) {
	env_nb++;
    }
    if ((new_environ = malloc((1U + env_nb) * sizeof (char *))) == NULL) {
	abort();
    }
    new_environ[env_nb] = NULL;
    while (env_nb > 0U) {
	env_nb--;
	new_environ[env_nb] = strdup(environ[env_nb]);
    }
    environ = new_environ;    
#endif
}

PHP_MINIT_FUNCTION(setproctitle)
{
    sapi_module_struct *symbol = &sapi_module;
    
    if (symbol != NULL) {
	argv0 = symbol->executable_location;
    }
    clearargs();

    return SUCCESS;
}

PHP_FUNCTION(setproctitle)
{
    char *title;
    long  title_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			      &title, &title_len) == FAILURE) {
	RETURN_NULL();
    }
#ifdef HAVE_SETPROCTITLE
    setproctitle("-%s", title);
#else
    if (argv0 != NULL) {
	memset(argv0, 0, argv_lth);
	strncpy(argv0, title, argv_lth - 2);
    }
#endif
}

PHP_MINFO_FUNCTION(setproctitle)
{
   php_info_print_table_start();
   php_info_print_table_header(2, "setproctitle support", "enabled");
   php_info_print_table_end();
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
