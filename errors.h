/*!
 * @file    mcp2515Driver.h 
 * @author  meltyHandsCo
 * @date    April, 2024
 * 
 * @brief   Common struct for function return types.
 * 
 */

#ifndef ERRORS_H
#define ERRORS_H

/*! 
 *  @brief  Enum that describes various return states.
 */
enum Errors {
    ERROR_OK        = 0,    /*!< No errors to report. */
    ERROR_FAIL      = 1,    /*!< Function failed to complete. */
    ERROR_WARNING   = 2,    /*!< Function reports a warning. */
    ERROR_MAX
};

#endif