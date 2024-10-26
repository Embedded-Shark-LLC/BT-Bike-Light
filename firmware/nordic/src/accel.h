/**
 * @file accel.h
 * @author Ben Owen (ben@embeddedshark.com)
 * @brief Header file for accel.c
 * 
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2024 Embedded Shark, LLC
 * 
 */

#ifndef __ACCEL_H__
#define __ACCEL_H__

#include "stdint.h"

/**
 * FUNCTION DECLARATIONS
 */

double accel_get_x();
double accel_get_y();
double accel_get_z();
int16_t accel_get_temp();

void accel_init();

#endif  /* __ACCEL_H__ */