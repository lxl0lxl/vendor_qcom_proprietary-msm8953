/*============================================================================
  @file DeviceOrientation.h

  @brief
  DeviceOrientation class definition.

  Copyright (c) 2016 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
============================================================================*/
#ifndef ANDROID_DEV_ORI_SENSOR_H
#define ANDROID_DEV_ORI_SENSOR_H

#include "SAMSensor.h"
#include "sns_sam_dev_ori_v01.h"

/*============================================================================
 * Class DeviceOrientation
=============================================================================*/

class DeviceOrientation : public SAMSensor {

public:
    DeviceOrientation(int handle);
    ~DeviceOrientation();
    int enable(int en);
/*===========================================================================
  FUNCTION:  processResp
    Process the response to the sensor1 SENSOR1_MSG_TYPE_RESP
    Parameters
        @msg_hdr : sensor1 message header
        @msg_ptr : sensor1 message data
===========================================================================*/
    void processResp(sensor1_msg_header_s *msg_hdr, void *msg_ptr);
/*===========================================================================
  FUNCTION:  processInd
    Process the response to the sensor1 SENSOR1_MSG_TYPE_IND
    Parameters
        @msg_hdr : sensor1 message header
        @msg_ptr : sensor1 message data
===========================================================================*/
    void processInd(sensor1_msg_header_s *msg_hdr, void *msg_ptr);

private:
/*===========================================================================
  FUNCTION:  sendGetReportReq
    Get the last report data after enabling the sensor.
===========================================================================*/
    int sendGetReportReq(void);
};

#endif
