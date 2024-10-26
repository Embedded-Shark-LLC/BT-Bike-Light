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

/**
 * @brief Get acceleration in X-axis
 * 
 * @return double accleration (g)
 */
double accel_get_x(void);

/**
 * @brief Get acceleration in Y-axis
 * 
 * @return double accleration (g)
 */
double accel_get_y(void);

/**
 * @brief Get acceleration in Z-axis
 * 
 * @return double accleration (g)
 */
double accel_get_z(void);

/**
 * @brief Get temperature
 * 
 * @return int16_t temperature (celsius)
 */
int16_t accel_get_temp(void);

/**
 * @brief Initialize accelerometer
 * 
 */
void accel_init(void);

#endif  /* __ACCEL_H__ */