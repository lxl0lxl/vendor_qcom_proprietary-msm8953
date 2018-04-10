/* imx258_lib.h
 *
 * Copyright (c) 2015-2017 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#ifndef __IMX258_LIB_H__
#define __IMX258_LIB_H__

#include "sensor_lib.h"
#include "eeprom_lib.h"
#include "pdaf_api.h"
#include "pdaf_camif_api.h"
#define SENSOR_MODEL "imx258"
/* IMX258 Regs */
#define IMX258_DIG_GAIN_GR_ADDR       0x020E
#define IMX258_DIG_GAIN_R_ADDR        0x0210
#define IMX258_DIG_GAIN_B_ADDR        0x0212
#define IMX258_DIG_GAIN_GB_ADDR       0x0214
#define IMX258_EXP_RATIO_ADDR         0x0222
#define IMX258_ABS_GAIN_R_WORD_ADDR   0x0B90
#define IMX258_ABS_GAIN_B_WORD_ADDR   0x0B92

/* IMX258 CONSTANTS */
#define IMX258_MAX_INTEGRATION_MARGIN   20
/* STATS DATA TYPE */
#define IMX258_CSI_PD_ISTATS            0x2F

#define IMX258_DATA_PEDESTAL            0x40 /* 10bit value */

#define IMX258_MIN_AGAIN_REG_VAL        0 /* 1.0x */
#define IMX258_MAX_AGAIN_REG_VAL        480 /* 16.0x */

#define IMX258_MIN_DGAIN_REG_VAL        256 /* 1.0x */
#define IMX258_MAX_DGAIN_REG_VAL        256 /* 1.0x */

#define IMX258_MAX_DGAIN_DECIMATOR      256

/* IMX258 FORMULAS */
#define IMX258_MIN_AGAIN    (512 / (512 - IMX258_MIN_AGAIN_REG_VAL))
#define IMX258_MAX_AGAIN    (512 / (512 - IMX258_MAX_AGAIN_REG_VAL))

#define IMX258_MIN_DGAIN    (IMX258_MIN_DGAIN_REG_VAL / 256)
#define IMX258_MAX_DGAIN    (IMX258_MAX_DGAIN_REG_VAL / 256)

#define IMX258_MIN_GAIN     IMX258_MIN_AGAIN * IMX258_MIN_DGAIN
#define IMX258_MAX_GAIN     IMX258_MAX_AGAIN * IMX258_MAX_DGAIN

/* uncomment FLIP_MIRROR macro to
   enable flip and mirror in sensor readout
   change bayer pattern
   load pdaf flip and mirror header
*/
#define FLIP_MIRROR
#define START_REG_ARRAY \
{ \
  {0x0100, 0x01, 0x00}, \
}

#define STOP_REG_ARRAY \
{ \
  {0x0100, 0x00, 0x00}, \
}

#define GROUPON_REG_ARRAY \
{ \
  {0x0104, 0x01, 0x00}, \
}

#define GROUPOFF_REG_ARRAY \
{ \
  {0x0104, 0x00, 0x00}, \
}

#define DUALCAM_MASTER_REG_ARRAY \
{ \
  /* Master */ \
  {0x3004, 0x01, 0x00}, \
  {0x5A5D, 0x01, 0x00}, \
  {0x4635, 0x00, 0x00}, \
  {0x5A90, 0x04, 0x00}, \
  {0x5A92, 0x04, 0x00}, \
  {0x5A93, 0x40, 0x00}, \
  {0x3006, 0x00, 0x00}, \
  {0x3007, 0x00, 0x00}, \
}

#define INIT0_REG_ARRAY \
{ \
  /* External Clock Settings */ \
  {0x0136, 0x18, 0x00}, \
  {0x0137, 0x00, 0x00}, \
  /* Global Settings */ \
  {0x3051, 0x00, 0x00}, \
  {0x6B11, 0xCF, 0x00}, \
  {0x7FF0, 0x08, 0x00}, \
  {0x7FF1, 0x0F, 0x00}, \
  {0x7FF2, 0x08, 0x00}, \
  {0x7FF3, 0x1B, 0x00}, \
  {0x7FF4, 0x23, 0x00}, \
  {0x7FF5, 0x60, 0x00}, \
  {0x7FF6, 0x00, 0x00}, \
  {0x7FF7, 0x01, 0x00}, \
  {0x7FF8, 0x00, 0x00}, \
  {0x7FF9, 0x78, 0x00}, \
  {0x7FFA, 0x01, 0x00}, \
  {0x7FFB, 0x00, 0x00}, \
  {0x7FFC, 0x00, 0x00}, \
  {0x7FFD, 0x00, 0x00}, \
  {0x7FFE, 0x00, 0x00}, \
  {0x7FFF, 0x03, 0x00}, \
  {0x7F76, 0x03, 0x00}, \
  {0x7F77, 0xFE, 0x00}, \
  {0x7FA8, 0x03, 0x00}, \
  {0x7FA9, 0xFE, 0x00}, \
  {0x7B24, 0x81, 0x00}, \
  {0x7B25, 0x01, 0x00}, \
  {0x6564, 0x07, 0x00}, \
  {0x6B0D, 0x41, 0x00}, \
  {0x653D, 0x04, 0x00}, \
  {0x6B05, 0x8C, 0x00}, \
  {0x6B06, 0xF9, 0x00}, \
  {0x6B08, 0x65, 0x00}, \
  {0x6B09, 0xFC, 0x00}, \
  {0x6B0A, 0xCF, 0x00}, \
  {0x6B0B, 0xD2, 0x00}, \
  {0x6700, 0x0E, 0x00}, \
  {0x6707, 0x0E, 0x00}, \
  {0x9104, 0x00, 0x00}, \
  {0x7421, 0x1C, 0x00}, \
  {0x7423, 0xD7, 0x00}, \
  {0x5F04, 0x00, 0x00}, \
  {0x5F05, 0xED, 0x00}, \
  /* Image Quality Adjusment Settings */ \
  /* Defect Pixel Correction settings */ \
  {0x94C7, 0xFF, 0x00}, \
  {0x94C8, 0xFF, 0x00}, \
  {0x94C9, 0xFF, 0x00}, \
  {0x95C7, 0xFF, 0x00}, \
  {0x95C8, 0xFF, 0x00}, \
  {0x95C9, 0xFF, 0x00}, \
  {0x94C4, 0x3F, 0x00}, \
  {0x94C5, 0x3F, 0x00}, \
  {0x94C6, 0x3F, 0x00}, \
  {0x95C4, 0x3F, 0x00}, \
  {0x95C5, 0x3F, 0x00}, \
  {0x95C6, 0x3F, 0x00}, \
  {0x94C1, 0x02, 0x00}, \
  {0x94C2, 0x02, 0x00}, \
  {0x94C3, 0x02, 0x00}, \
  {0x95C1, 0x02, 0x00}, \
  {0x95C2, 0x02, 0x00}, \
  {0x95C3, 0x02, 0x00}, \
  {0x94BE, 0x0C, 0x00}, \
  {0x94BF, 0x0C, 0x00}, \
  {0x94C0, 0x0C, 0x00}, \
  {0x95BE, 0x0C, 0x00}, \
  {0x95BF, 0x0C, 0x00}, \
  {0x95C0, 0x0C, 0x00}, \
  {0x94D0, 0x74, 0x00}, \
  {0x94D1, 0x74, 0x00}, \
  {0x94D2, 0x74, 0x00}, \
  {0x95D0, 0x74, 0x00}, \
  {0x95D1, 0x74, 0x00}, \
  {0x95D2, 0x74, 0x00}, \
  {0x94CD, 0x2E, 0x00}, \
  {0x94CE, 0x2E, 0x00}, \
  {0x94CF, 0x2E, 0x00}, \
  {0x95CD, 0x2E, 0x00}, \
  {0x95CE, 0x2E, 0x00}, \
  {0x95CF, 0x2E, 0x00}, \
  {0x94CA, 0x4C, 0x00}, \
  {0x94CB, 0x4C, 0x00}, \
  {0x94CC, 0x4C, 0x00}, \
  {0x95CA, 0x4C, 0x00}, \
  {0x95CB, 0x4C, 0x00}, \
  {0x95CC, 0x4C, 0x00}, \
  {0x900E, 0x32, 0x00}, \
  {0x94E2, 0xFF, 0x00}, \
  {0x94E3, 0xFF, 0x00}, \
  {0x94E4, 0xFF, 0x00}, \
  {0x95E2, 0xFF, 0x00}, \
  {0x95E3, 0xFF, 0x00}, \
  {0x95E4, 0xFF, 0x00}, \
  {0x94DF, 0x6E, 0x00}, \
  {0x94E0, 0x6E, 0x00}, \
  {0x94E1, 0x6E, 0x00}, \
  {0x95DF, 0x6E, 0x00}, \
  {0x95E0, 0x6E, 0x00}, \
  {0x95E1, 0x6E, 0x00}, \
  {0x7FCC, 0x01, 0x00}, \
  {0x7B78, 0x00, 0x00}, \
  {0x9401, 0x35, 0x00}, \
  {0x9403, 0x23, 0x00}, \
  {0x9405, 0x23, 0x00}, \
  {0x9406, 0x00, 0x00}, \
  {0x9407, 0x31, 0x00}, \
  {0x9408, 0x00, 0x00}, \
  {0x9409, 0x1B, 0x00}, \
  {0x940A, 0x00, 0x00}, \
  {0x940B, 0x15, 0x00}, \
  {0x940D, 0x3F, 0x00}, \
  {0x940F, 0x3F, 0x00}, \
  {0x9411, 0x3F, 0x00}, \
  {0x9413, 0x64, 0x00}, \
  {0x9415, 0x64, 0x00}, \
  {0x9417, 0x64, 0x00}, \
  {0x941D, 0x34, 0x00}, \
  {0x941F, 0x01, 0x00}, \
  {0x9421, 0x01, 0x00}, \
  {0x9423, 0x01, 0x00}, \
  {0x9425, 0x23, 0x00}, \
  {0x9427, 0x23, 0x00}, \
  {0x9429, 0x23, 0x00}, \
  {0x942B, 0x2F, 0x00}, \
  {0x942D, 0x1A, 0x00}, \
  {0x942F, 0x14, 0x00}, \
  {0x9431, 0x3F, 0x00}, \
  {0x9433, 0x3F, 0x00}, \
  {0x9435, 0x3F, 0x00}, \
  {0x9437, 0x6B, 0x00}, \
  {0x9439, 0x7C, 0x00}, \
  {0x943B, 0x81, 0x00}, \
  {0x9443, 0x0F, 0x00}, \
  {0x9445, 0x0F, 0x00}, \
  {0x9447, 0x0F, 0x00}, \
  {0x9449, 0x0F, 0x00}, \
  {0x944B, 0x0F, 0x00}, \
  {0x944D, 0x0F, 0x00}, \
  {0x944F, 0x1E, 0x00}, \
  {0x9451, 0x0F, 0x00}, \
  {0x9453, 0x0B, 0x00}, \
  {0x9455, 0x28, 0x00}, \
  {0x9457, 0x13, 0x00}, \
  {0x9459, 0x0C, 0x00}, \
  {0x945D, 0x00, 0x00}, \
  {0x945E, 0x00, 0x00}, \
  {0x945F, 0x00, 0x00}, \
  {0x946D, 0x00, 0x00}, \
  {0x946F, 0x10, 0x00}, \
  {0x9471, 0x10, 0x00}, \
  {0x9473, 0x40, 0x00}, \
  {0x9475, 0x2E, 0x00}, \
  {0x9477, 0x10, 0x00}, \
  {0x9478, 0x0A, 0x00}, \
  {0x947B, 0xE0, 0x00}, \
  {0x947C, 0xE0, 0x00}, \
  {0x947D, 0xE0, 0x00}, \
  {0x947E, 0xE0, 0x00}, \
  {0x947F, 0xE0, 0x00}, \
  {0x9480, 0xE0, 0x00}, \
  {0x9483, 0x14, 0x00}, \
  {0x9485, 0x14, 0x00}, \
  {0x9487, 0x14, 0x00}, \
  {0x9501, 0x35, 0x00}, \
  {0x9503, 0x14, 0x00}, \
  {0x9505, 0x14, 0x00}, \
  {0x9507, 0x31, 0x00}, \
  {0x9509, 0x1B, 0x00}, \
  {0x950B, 0x15, 0x00}, \
  {0x950D, 0x1E, 0x00}, \
  {0x950F, 0x1E, 0x00}, \
  {0x9511, 0x1E, 0x00}, \
  {0x9513, 0x64, 0x00}, \
  {0x9515, 0x64, 0x00}, \
  {0x9517, 0x64, 0x00}, \
  {0x951D, 0x34, 0x00}, \
  {0x951F, 0x01, 0x00}, \
  {0x9521, 0x01, 0x00}, \
  {0x9523, 0x01, 0x00}, \
  {0x9525, 0x14, 0x00}, \
  {0x9527, 0x14, 0x00}, \
  {0x9529, 0x14, 0x00}, \
  {0x952B, 0x2F, 0x00}, \
  {0x952D, 0x1A, 0x00}, \
  {0x952F, 0x14, 0x00}, \
  {0x9531, 0x1E, 0x00}, \
  {0x9533, 0x1E, 0x00}, \
  {0x9535, 0x1E, 0x00}, \
  {0x9537, 0x6B, 0x00}, \
  {0x9539, 0x7C, 0x00}, \
  {0x953B, 0x81, 0x00}, \
  {0x9543, 0x0F, 0x00}, \
  {0x9545, 0x0F, 0x00}, \
  {0x9547, 0x0F, 0x00}, \
  {0x9549, 0x0F, 0x00}, \
  {0x954B, 0x0F, 0x00}, \
  {0x954D, 0x0F, 0x00}, \
  {0x954F, 0x15, 0x00}, \
  {0x9551, 0x0B, 0x00}, \
  {0x9553, 0x08, 0x00}, \
  {0x9555, 0x1C, 0x00}, \
  {0x9557, 0x0D, 0x00}, \
  {0x9559, 0x08, 0x00}, \
  {0x955D, 0x00, 0x00}, \
  {0x955E, 0x00, 0x00}, \
  {0x955F, 0x00, 0x00}, \
  {0x956D, 0x00, 0x00}, \
  {0x956F, 0x10, 0x00}, \
  {0x9571, 0x10, 0x00}, \
  {0x9573, 0x40, 0x00}, \
  {0x9575, 0x2E, 0x00}, \
  {0x9577, 0x10, 0x00}, \
  {0x9578, 0x0A, 0x00}, \
  {0x957B, 0xE0, 0x00}, \
  {0x957C, 0xE0, 0x00}, \
  {0x957D, 0xE0, 0x00}, \
  {0x957E, 0xE0, 0x00}, \
  {0x957F, 0xE0, 0x00}, \
  {0x9580, 0xE0, 0x00}, \
  {0x9583, 0x14, 0x00}, \
  {0x9585, 0x14, 0x00}, \
  {0x9587, 0x14, 0x00}, \
  {0x7F78, 0x00, 0x00}, \
  {0x7F89, 0x00, 0x00}, \
  {0x7F93, 0x00, 0x00}, \
  {0x924B, 0x1B, 0x00}, \
  {0x924C, 0x0A, 0x00}, \
  {0x9304, 0x04, 0x00}, \
  {0x9315, 0x04, 0x00}, \
  {0x9250, 0x50, 0x00}, \
  {0x9251, 0x3C, 0x00}, \
  {0x9252, 0x14, 0x00}, \
}

#ifndef FLIP_MIRROR
  #define FLIP_MIRROR_SETTING {{0x0101, 0x00, 0x00}}
#else
  #define FLIP_MIRROR_SETTING {{0x0101, 0x03, 0x00}}
#endif

#define RES0_REG_ARRAY \
{ \
  {0x0112, 0x0A, 0x00}, \
  {0x0113, 0x0A, 0x00}, \
  {0x0114, 0x03, 0x00}, \
  {0x0301, 0x05, 0x00}, \
  {0x0303, 0x02, 0x00}, \
  {0x0305, 0x04, 0x00}, \
  {0x0306, 0x00, 0x00}, \
  {0x0307, 0xAA, 0x00}, \
  {0x0309, 0x0A, 0x00}, \
  {0x030b, 0x01, 0x00}, \
  {0x030d, 0x02, 0x00}, \
  {0x030e, 0x00, 0x00}, \
  {0x030f, 0xd8, 0x00}, \
  {0x0310, 0x00, 0x00}, \
  {0x0820, 0x0f, 0x00}, \
  {0x0821, 0xf0, 0x00}, \
  {0x0822, 0x00, 0x00}, \
  {0x0823, 0x00, 0x00}, \
  {0x4648, 0x7f, 0x00}, \
  {0x7420, 0x00, 0x00}, \
  {0x7421, 0x1c, 0x00}, \
  {0x7422, 0x00, 0x00}, \
  {0x7423, 0xd7, 0x00}, \
  {0x9104, 0x00, 0x00}, \
  {0x0342, 0x14, 0x00}, \
  {0x0343, 0xe8, 0x00}, \
  {0x0340, 0x0c, 0x00}, \
  {0x0341, 0x50, 0x00}, \
  {0x0344, 0x00, 0x00}, \
  {0x0345, 0x00, 0x00}, \
  {0x0346, 0x00, 0x00}, \
  {0x0347, 0x00, 0x00}, \
  {0x0348, 0x10, 0x00}, \
  {0x0349, 0x6f, 0x00}, \
  {0x034a, 0x0c, 0x00}, \
  {0x034b, 0x2f, 0x00}, \
  {0x0381, 0x01, 0x00}, \
  {0x0383, 0x01, 0x00}, \
  {0x0385, 0x01, 0x00}, \
  {0x0387, 0x01, 0x00}, \
  {0x0900, 0x00, 0x00}, \
  {0x0901, 0x11, 0x00}, \
  {0x0401, 0x00, 0x00}, \
  {0x0404, 0x00, 0x00}, \
  {0x0405, 0x10, 0x00}, \
  {0x0408, 0x00, 0x00}, \
  {0x0409, 0x00, 0x00}, \
  {0x040a, 0x00, 0x00}, \
  {0x040b, 0x00, 0x00}, \
  {0x040c, 0x10, 0x00}, \
  {0x040d, 0x70, 0x00}, \
  {0x040e, 0x0c, 0x00}, \
  {0x040f, 0x30, 0x00}, \
  {0x3038, 0x00, 0x00}, \
  {0x303a, 0x00, 0x00}, \
  {0x303b, 0x10, 0x00}, \
  {0x300d, 0x00, 0x00}, \
  {0x034c, 0x10, 0x00}, \
  {0x034d, 0x70, 0x00}, \
  {0x034e, 0x0c, 0x00}, \
  {0x034f, 0x30, 0x00}, \
  {0x0202, 0x0c, 0x00}, \
  {0x0203, 0x46, 0x00}, \
  {0x0204, 0x00, 0x00}, \
  {0x0205, 0x00, 0x00}, \
  {0x020e, 0x01, 0x00}, \
  {0x020f, 0x00, 0x00}, \
  {0x0210, 0x01, 0x00}, \
  {0x0211, 0x00, 0x00}, \
  {0x0212, 0x01, 0x00}, \
  {0x0213, 0x00, 0x00}, \
  {0x0214, 0x01, 0x00}, \
  {0x0215, 0x00, 0x00}, \
  {0x7bcd, 0x00, 0x00}, \
  {0x94dc, 0x20, 0x00}, \
  {0x94dd, 0x20, 0x00}, \
  {0x94de, 0x20, 0x00}, \
  {0x95dc, 0x20, 0x00}, \
  {0x95dd, 0x20, 0x00}, \
  {0x95de, 0x20, 0x00}, \
  {0x7fb0, 0x00, 0x00}, \
  {0x9010, 0x3e, 0x00}, \
  {0x9419, 0x50, 0x00}, \
  {0x941b, 0x50, 0x00}, \
  {0x9519, 0x50, 0x00}, \
  {0x951b, 0x50, 0x00}, \
  {0x3030, 0x00, 0x00}, \
  {0x3032, 0x00, 0x00}, \
  {0x0220, 0x00, 0x00}, \
}
/* Sensor Handler */
static sensor_lib_t sensor_lib_ptr =
{
  .sensor_slave_info =
  {
    .sensor_name = SENSOR_MODEL,
    .slave_addr = 0x34,
    .i2c_freq_mode = SENSOR_I2C_MODE_FAST,
    .addr_type = CAMERA_I2C_WORD_ADDR,
    .sensor_id_info =
    {
      .sensor_id_reg_addr = 0x0016,
      .sensor_id = 0x0258,
    },
    .power_setting_array =
    {
      .power_setting_a =
      {
        {
          .seq_type = CAMERA_POW_SEQ_GPIO,
          .seq_val = CAMERA_GPIO_RESET,
          .config_val = GPIO_OUT_LOW,
          .delay = 1,
        },
        {
          .seq_type = CAMERA_POW_SEQ_GPIO,
          .seq_val = CAMERA_GPIO_VANA,
          .config_val = GPIO_OUT_HIGH,
          .delay = 0,
        },
        {
          .seq_type = CAMERA_POW_SEQ_GPIO,
          .seq_val = CAMERA_GPIO_VDIG,
          .config_val = GPIO_OUT_HIGH,
          .delay = 0,
        },
        {
          .seq_type = CAMERA_POW_SEQ_VREG,
          .seq_val = CAMERA_VIO,
          .config_val = 0,
          .delay = 0,
        },
        {
          .seq_type = CAMERA_POW_SEQ_CLK,
          .seq_val = CAMERA_MCLK,
          .config_val = 24000000,
          .delay = 1,
        },
        {
          .seq_type = CAMERA_POW_SEQ_GPIO,
          .seq_val = CAMERA_GPIO_RESET,
          .config_val = GPIO_OUT_HIGH,
          .delay = 12,
        },
      },
      .size = 6,
      .power_down_setting_a =
      {
        {
          .seq_type = CAMERA_POW_SEQ_GPIO,
          .seq_val = CAMERA_GPIO_RESET,
          .config_val = GPIO_OUT_LOW,
          .delay = 1,
        },
        {
          .seq_type = CAMERA_POW_SEQ_CLK,
          .seq_val = CAMERA_MCLK,
          .config_val = 0,
          .delay = 1,
        },
        {
          .seq_type = CAMERA_POW_SEQ_VREG,
          .seq_val = CAMERA_VIO,
          .config_val = 0,
          .delay = 0,
        },
        {
          .seq_type = CAMERA_POW_SEQ_GPIO,
          .seq_val = CAMERA_GPIO_VDIG,
          .config_val = GPIO_OUT_LOW,
          .delay = 0,
        },
        {
          .seq_type = CAMERA_POW_SEQ_GPIO,
          .seq_val = CAMERA_GPIO_VANA,
          .config_val = GPIO_OUT_LOW,
          .delay = 0,
        },
      },
      .size_down = 5,
    },
  },
  .sensor_output =
  {
    .output_format = SENSOR_BAYER,
    .connection_mode = SENSOR_MIPI_CSI,
    .raw_output = SENSOR_10_BIT_DIRECT,
#ifndef FLIP_MIRROR
    .filter_arrangement = SENSOR_RGGB,
#else
    .filter_arrangement = SENSOR_BGGR,
#endif
  },
  .output_reg_addr =
  {
    .x_output = 0x034C,
    .y_output = 0x034E,
    .line_length_pclk = 0x0342,
    .frame_length_lines = 0x0340,
  },
  .exp_gain_info =
  {
    .coarse_int_time_addr = 0x0202,
    .global_gain_addr = 0x0204,
    .vert_offset = IMX258_MAX_INTEGRATION_MARGIN,
  },
  .aec_info =
  {
    .min_gain = IMX258_MIN_GAIN,
    .max_gain = IMX258_MAX_GAIN,
    .max_analog_gain = IMX258_MAX_AGAIN,
    .max_linecount = 65525 - IMX258_MAX_INTEGRATION_MARGIN,
  },
  .sensor_num_frame_skip = 2,
  .sensor_num_HDR_frame_skip = 2,
  .sensor_max_pipeline_frame_delay = 2,
  .sensor_property =
  {
    .pix_size = 1.12, /* um */
    .sensing_method = SENSOR_SMETHOD_ONE_CHIP_COLOR_AREA_SENSOR,
    .crop_factor = 5.78,
  },
  .pixel_array_size_info =
  {
    .active_array_size =
    {
      .width = 4208,
      .height = 3120,
    },
    .left_dummy = 8,
    .right_dummy = 8,
    .top_dummy = 8,
    .bottom_dummy = 8,
  },
  .color_level_info =
  {
    .white_level = 1023,
    .r_pedestal = IMX258_DATA_PEDESTAL,
    .gr_pedestal = IMX258_DATA_PEDESTAL,
    .gb_pedestal = IMX258_DATA_PEDESTAL,
    .b_pedestal = IMX258_DATA_PEDESTAL,
  },
  .sensor_stream_info_array =
  {
    .sensor_stream_info =
    {
      {
        .vc_cfg_size = 2,
        .vc_cfg =
        {
          {
            .cid = 0,
            .dt = CSI_RAW10,
            .decode_format = CSI_DECODE_10BIT,
          },
          {
            .cid = 1,
            .dt = IMX258_CSI_PD_ISTATS,
            .decode_format = CSI_DECODE_8BIT,
          },
        },
        .pix_data_fmt =
        {
          SENSOR_BAYER,
          SENSOR_META,
        },
      },
      {
        .vc_cfg_size = 1,
        .vc_cfg =
        {
          {
            .cid = 2,
            .dt = CSI_EMBED_DATA,
            .decode_format = CSI_DECODE_8BIT,
          },
        },
        .pix_data_fmt =
        {
          SENSOR_META,
        },
      },
    },
    .size = 2,
  },
  .start_settings =
  {
    .reg_setting_a = START_REG_ARRAY,
    .addr_type = CAMERA_I2C_WORD_ADDR,
    .data_type = CAMERA_I2C_BYTE_DATA,
    .delay = 0,
  },
  .stop_settings =
  {
    .reg_setting_a = STOP_REG_ARRAY,
    .addr_type = CAMERA_I2C_WORD_ADDR,
    .data_type = CAMERA_I2C_BYTE_DATA,
    .delay = 0,
  },
  .groupon_settings =
  {
    .reg_setting_a = GROUPON_REG_ARRAY,
    .addr_type = CAMERA_I2C_WORD_ADDR,
    .data_type = CAMERA_I2C_BYTE_DATA,
    .delay = 0,
  },
  .groupoff_settings =
  {
    .reg_setting_a = GROUPOFF_REG_ARRAY,
    .addr_type = CAMERA_I2C_WORD_ADDR,
    .data_type = CAMERA_I2C_BYTE_DATA,
    .delay = 0,
  },
  .dualcam_settings =
  {
    .reg_setting_a = DUALCAM_MASTER_REG_ARRAY,
    .addr_type = CAMERA_I2C_WORD_ADDR,
    .data_type = CAMERA_I2C_BYTE_DATA,
    .delay = 0,
    .size = 8,
  },
  .embedded_data_enable_settings =
  {
    .reg_setting_a = {},
    .addr_type = 0,
    .data_type = 0,
    .delay = 0,
  },
  .embedded_data_disable_settings =
  {
    .reg_setting_a = {},
    .addr_type = 0,
    .data_type = 0,
    .delay = 0,
  },
  .test_pattern_info =
  {
    .test_pattern_settings =
    {
      {
        .mode = SENSOR_TEST_PATTERN_OFF,
        .settings =
        {
          .reg_setting_a =
          {
            {0x0600, 0x0000, 0x00},
          },
          .addr_type = CAMERA_I2C_WORD_ADDR,
          .data_type = CAMERA_I2C_WORD_DATA,
          .delay = 0,
        }
      },
      {
        .mode = SENSOR_TEST_PATTERN_SOLID_COLOR,
        .settings =
        {
          .reg_setting_a =
          {
            {0x0600, 0x0001, 0x00},
          },
          .addr_type = CAMERA_I2C_WORD_ADDR,
          .data_type = CAMERA_I2C_WORD_DATA,
          .delay = 0,
        },
      },
      {
        .mode = SENSOR_TEST_PATTERN_COLOR_BARS,
        .settings =
        {
          .reg_setting_a =
          {
            {0x0600, 0x0002, 0x00},
          },
          .addr_type = CAMERA_I2C_WORD_ADDR,
          .data_type = CAMERA_I2C_WORD_DATA,
          .delay = 0,
        },
      },
      {
        .mode = SENSOR_TEST_PATTERN_COLOR_BARS_FADE_TO_GRAY,
        .settings =
        {
          .reg_setting_a =
          {
            {0x0600, 0x0003, 0x00},
          },
          .addr_type = CAMERA_I2C_WORD_ADDR,
          .data_type = CAMERA_I2C_WORD_DATA,
          .delay = 0,
        },
      },
      {
        .mode = SENSOR_TEST_PATTERN_PN9,
        .settings =
        {
          .reg_setting_a =
          {
            {0x0600, 0x0004, 0x00},
          },
          .addr_type = CAMERA_I2C_WORD_ADDR,
          .data_type = CAMERA_I2C_WORD_DATA,
          .delay = 0,
        },
      },
    },
    .size = 5,
    .solid_mode_addr =
    {
      .r_addr = 0x0602,
      .gr_addr = 0x0604,
      .gb_addr = 0x0606,
      .b_addr = 0x0608,
    },
  },
  .init_settings_array =
  {
    .reg_settings =
    {
      {
        .reg_setting_a = INIT0_REG_ARRAY,
        .addr_type = CAMERA_I2C_WORD_ADDR,
        .data_type = CAMERA_I2C_BYTE_DATA,
        .delay = 0,
      },
      {
        .reg_setting_a = FLIP_MIRROR_SETTING,
        .addr_type = CAMERA_I2C_WORD_ADDR,
        .data_type = CAMERA_I2C_BYTE_DATA,
        .delay = 0,
      },
    },
    .size = 2,
  },
  .res_settings_array =
  {
    .reg_settings =
    {
      /* Res 0 */
      {
        .reg_setting_a = RES0_REG_ARRAY,
        .addr_type = CAMERA_I2C_WORD_ADDR,
        .data_type = CAMERA_I2C_BYTE_DATA,
        .delay = 0,
      },
     },
    .size = 1,
  },
  .out_info_array =
  {
    .out_info =
    {
      {
        .x_output = 4208,
        .y_output = 3120,
        .line_length_pclk = 5352,
        .frame_length_lines = 3152,
        .vt_pixel_clk = 404800000,
        .op_pixel_clk = 408000000,
        .binning_factor = 1,
        .min_fps = 10.0,
        .max_fps = 24.0,
        .mode = SENSOR_DEFAULT_MODE,
        .offset_x = 0,
        .offset_y = 0,
        .scale_factor = 1,
      },
    },
    .size = 1,
  },
  .csi_params =
  {
    .lane_cnt = 4,
    .settle_cnt = 0xB,
    .is_csi_3phase = 0,
  },
  .csid_lut_params_array =
  {
    .lut_params =
    {
      /* Res 0 */ 
      {
        .num_cid = 3,
        .vc_cfg_a =
        {
          {
            .cid = 0,
            .dt = CSI_RAW10,
            .decode_format = CSI_DECODE_10BIT
          },
          {
            .cid = 1,
            .dt = IMX258_CSI_PD_ISTATS,
            .decode_format = CSI_DECODE_8BIT
          },
          {
            .cid = 2,
            .dt = CSI_EMBED_DATA,
            .decode_format = CSI_DECODE_8BIT
          },
        },
      },
    },
    .size = 1,
  },
  .crop_params_array =
  {
    .crop_params =
    {
      /* Res 0 */
      {
        .top_crop = 0,
        .bottom_crop = 0,
        .left_crop = 0,
        .right_crop = 0,
      },
    },
    .size = 1,
  },
  .exposure_func_table =
  {
    .sensor_calculate_exposure = sensor_calculate_exposure,
    .sensor_fill_exposure_array = sensor_fill_exposure_array,
  },
  .meta_data_out_info_array =
  {
    .meta_data_out_info =
    {
      {
        /* set the meta half size which it should be to overcome the isp bug */
        .width = 80,
        .height = 1920,
        .stats_type = PD_STATS,
        .dt = IMX258_CSI_PD_ISTATS,
      },
    },
    .size = 1,
  },
  .sensor_capability = 0,
  .awb_func_table =
  {
    .sensor_fill_awb_array = 0,
    .awb_table_size = 0,
  },
  .parse_RDI_stats =
  {
    .parse_VHDR_stats = NULL,
  },
  .rolloff_config =
  {
    .enable = FALSE,
    .full_size_info =
    {
      .full_size_width = 0,
      .full_size_height = 0,
      .full_size_left_crop = 0,
      .full_size_top_crop = 0,
    },
  },
  .adc_readout_time = 0,
  .sensor_num_fast_aec_frame_skip = 0,
  .noise_coeff = {
    .gradient_S = 3.738032e-06,
    .offset_S = 3.651935e-04,
    .gradient_O = 4.499952e-07,
    .offset_O = -2.968624e-04,
  },
  .pdaf_config = {
#ifndef FLIP_MIRROR
    #include "imx258_pdaf.h"
#else
    #include "imx258_pdaf_flip_mirror.h"
#endif
  },
};

#endif /* __IMX258_LIB_H__ */
