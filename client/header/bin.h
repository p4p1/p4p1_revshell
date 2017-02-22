#ifndef _BIN_H_
#define _BIN_H_

/* bin header, location of all of the seperate commands that are used
** by the user and a wrapper for the custom commands
*/

#include "../main.h"
#include "win_api.h"
#include "command.h"
#include "network.h"

int process_command(struct main_struct *);
int exe(struct main_struct *);

#endif
