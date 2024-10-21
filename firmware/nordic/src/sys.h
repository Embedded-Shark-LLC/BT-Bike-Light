/**
 * @file sys.h
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Header file for sys.c
 * 
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __SYS_H__
#define __SYS_H__

/* ENUMERATION DEFINITIONS */

typedef enum
{
    SYS_STATE_POWEROFF,
    SYS_STATE_WAKEUP,
    SYS_SYATE_RUN,
}
sys_state_t;

/* LOCAL VARIABLES */
extern sys_state_t sys_state;

#endif  /* __SYS_H__ */