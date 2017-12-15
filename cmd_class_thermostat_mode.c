/*===================================================================================*/
/*************************************************************************************/
/**
 * @file       cmd_class_thermostat_mode.c
 * @brief      Collection of zwave command packet build and report handling APIs for
 *	       thermostat mode command class(version-4).
 * @details    Contains various APIs required for building all commands required for
 *             thermostat mode report command class and handlers for all reports(version-4).
 * @see	       http://zwavepublic.com, Zwave Command Class Specification
 * @author     Dhaval Punjabi, dhavalpunjabi@elear.solutions
 * @copyright  Copyright (c) 2017 Elear Solutions Tech Private Limited. All rights
 *             reserved.
 * @license    To any person (the "Recipient") obtaining a copy of this software and
 *             associated documentation files (the "Software"):\n
 *             All information contained in or disclosed by this software is
 *             confidential and proprietary information of Elear Solutions Tech
 *             Private Limited and all rights therein are expressly reserved.
 *             By accepting this material the recipient agrees that this material and
 *             the information contained therein is held in confidence and in trust
 *             and will NOT be used, copied, modified, merged, published, distributed,
 *             sublicensed, reproduced in whole or in part, nor its contents revealed
 *             in any manner to others without the express written permission of
 *             Elear Solutions Tech Private Limited.
 */
/*************************************************************************************/
/*===================================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include <json-c/json.h>
#include <zwareclib/include/zip_api.h>
#include "allocate.h"
#include "logger.h"
#include "util.h"
#include "cmd_class_common.h"
#include "json_services.h"
#include "mqtt_outbox_handlers.h"
#include <math.h>
#include <string.h>
#include "ocf_json_services.h"
#include "json-c/json_object.h"

/*************************************************************************************
 *                          LOCAL MACROS                                             *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL TYPEDEFS                                           *
 *************************************************************************************/

 /*************************************************************************************
 *                          LOCAL PROTOTYPES                                         *
 *************************************************************************************/

 /*************************************************************************************
 *                          GLOBAL VARIABLES                                         *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL VARIABLES                                          *
 *************************************************************************************/

 /*************************************************************************************
 *                          PRIVATE FUNCTIONS                                        *
 *************************************************************************************/
/******************************************************************************
Name									:
Input(s)					:
Output(s) 			:
Description		: