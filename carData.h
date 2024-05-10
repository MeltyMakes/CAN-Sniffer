/*!
 * @file    carData.h 
 * @author  meltyMakes
 * @date    December, 2023
 * 
 * @brief   Struct for holding data from the car.
 * 
 */

#ifndef CAR_DATA_H
#define CAR_DATA_H


/* //todo: doxygen */
typedef struct CarData
{
    int rpm         = 0;
    int speedKph    = 0;
    char gear       = 'X';

} CarData;


#endif