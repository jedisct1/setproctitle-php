/* $Id$ */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef __linux__
# include <sys/prctl.h>
#endif
#ifdef __hpux__
# include <sys/param.h>
# include <sys/pstat.h>
#endif
#include <stdlib.h>
#include <string.h>

#include "php.h"
#include "SAPI.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_setproctitle.h"

const zend_function_entry setproctitle_functions[] = {
    PHP_FE(setproctitle, NULL)
#ifdef PHP_FE_END   
    PHP_FE_END
#else
    { NULL, NULL, NULL }
#endif
};

zend_module_entry setproctitle_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "setproctitle",
    setproctitle_functions,
    NULL,
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

#if defined(__linux__) && !defined(HAVE_SETPROCTITLE)
static char   *argv0 = NULL;
static size_t  argv_lth;

static void clearargs(void)
{
    extern char **environ;
    char        **new_environ;
    char         *next = argv0;
    char         *env0;
    unsigned int  env_nb = 0U;
    
    if (next == NULL || environ == NULL || (env0 = environ[0]) == NULL) {
        return;
    }
    while (next != env0) {
        next += strlen(next) + 1U;
    }
    while (*next != 0) {
        next += strlen(next) + 1U;
    }
    argv_lth = next - argv0;
    while (environ[env_nb] != NULL) {
        env_nb++;
    }
    if ((new_environ = calloc(1U + env_nb, sizeof (char *))) == NULL) {
        return;
    }
    new_environ[env_nb] = NULL;
    while (env_nb-- > 0U) {
        if ((new_environ[env_nb] = strdup(environ[env_nb])) == NULL) {
            return;
        }
    }
    environ = new_environ;
}

static void relocate_environ_for_linux(void)
{
    static _Bool done;

    if (done != 0) {
        return;
    }
    argv0 = sapi_module.executable_location;
    clearargs();
    done = 1;
}
#endif

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

#elif defined(__hpux__)

    union pstun pst;
    
    pst.pst_command = title;
    pstat(PSTAT_SETCMD, pst, strlen(title), 0, 0);

#elif defined(__linux__)

    relocate_environ_for_linux();    
    if (argv0 != NULL && argv_lth > 2U) {
        memset(argv0, 0, argv_lth);
        strncpy(argv0, title, argv_lth - 2U);
    }
# ifdef PR_SET_NAME
    prctl(PR_SET_NAME, (unsigned long) title, NULL, NULL, NULL);
# endif

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
