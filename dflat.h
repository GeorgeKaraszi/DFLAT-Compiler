#ifndef DFLAT_H_
#define DFLAT_H_

#define _XOPEN_SOURCE 500 /* POSXI strdup test macro to statisfy compile warnings */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>
#include "bison.tab.h"


extern char *strdup(const char *s);
extern char *strndup(const char *s, size_t n);

extern FILE *yyin;

extern void yyerror(char *s);
extern int yyparse(void);

#endif