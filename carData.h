/*!
 * @file    carData.h 
 * @author  meltyMakesCo
 * @date    December, 2023
 * 
 * @brief   Struct for holding data from the car.
 * 
 */

#ifndef CAR_DATA_H
#define CAR_DATA_H


/*! 
 *  @struct CarData
 *  @brief  Structure for containing car data that we want to keep track of.
 */
typedef struct CarData
{
    int rpm         = 0;    /*!< Engine Rotations per Minute. */
    int speedKph    = 0;    /*!< Average car speed in Kilometers per hour. */
    char gear       = 'X';  /*!< Automatic Transmission Gear Mode. */

} CarData;


#endif