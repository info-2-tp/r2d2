/*
 * PR_HC_SR04.h
 *
 *  Created on: 26 oct. 2019
 *      Author: exo
 */

#ifndef HEADERS_PR_HC_SR04_H_
#define HEADERS_PR_HC_SR04_H_

#define CM 1
#define MM 10

typedef uint8_t distance_base;
typedef uint16_t distance_t;

void init_hc_sr04_sensor(void);
distance_t getDistance(distance_base base);
#endif /* HEADERS_PR_HC_SR04_H_ */
