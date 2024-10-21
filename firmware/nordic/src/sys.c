/**
 * @file sys.c
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief File containing global system-level functions
 *          Also contains global system-level state variables
 * 
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sys.h"

#include <nrfx.h>

/*
 * LOCAL VARIABLES
 */

sys_state_t sys_state = SYS_STATE_POWEROFF;
