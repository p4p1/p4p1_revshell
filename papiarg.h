#ifndef papiarg_h
#define papiarg_h

#include "papi.h"
#include "papimain.h"

void check_arg(struct server_info * inf, char * argv[], int argc);
void acandsendnotif(struct server_info * inf);

#endif
