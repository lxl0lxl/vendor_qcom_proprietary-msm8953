/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

GENERAL DESCRIPTION
  DataItemConcreteTypeFieldNames

  Copyright (c) 2015-2016 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
=============================================================================*/

#ifndef __IZAT_MANAGER_DATAITEMCONCRETETYPEFIELDNAMES_H__
#define __IZAT_MANAGER_DATAITEMCONCRETETYPEFIELDNAMES_H__

#define RESPONSE_FIELD_NAME "INFO"
#define RESPONSE_FIELD_VALUE_OS "OS-STATUS-UPDATE"
#define RESPONSE_FIELD_VALUE_RIL "RIL_UPDATE"

#define AIRPLANEMODE_CARD ""
#define AIRPLANEMODE_FIELD_MODE "IS_AIRPLANE_MODE_ON"

#define ENH_CARD ""
#define ENH_FIELD_ENABLED "IS_QUALCOMM_ENHANCED_PROVIDER_ENABLED"

#define GPSSTATE_CARD ""
#define GPSSTATE_FIELD_ENABLED "IS_GPS_PROVIDER_ENABLED"

#define NLPSTATUS_CARD ""
#define NLPSTATUS_FIELD_ENABLED "IS_NETWORK_PROVIDER_ENABLED"

#define WIFIHARDWARESTATE_CARD ""
#define WIFIHARDWARESTATE_FIELD_ENABLED "IS_WIFI_HARDWARE_ON"

#define SCREENSTATE_CARD ""
#define SCREENSTATE_FIELD_ENABLED "IS_SCREEN_ON"

#define POWERCONNECTSTATE_CARD ""
#define POWERCONNECTSTATE_FIELD_ENABLED "IS_POWER_CONNECTED"

#define TIMEZONECHANGE_CARD ""
#define TIMEZONECHANGE_FIELD_ENABLED "IS_TIMEZONE_CHANGED"

#define TIMECHANGE_CARD ""
#define TIMECHANGE_FIELD_ENABLED "IS_TIME_CHANGED"
#define TIMECHANGE_FIELD_CURRENT_TIME_MILLIS "CURR_TIME_MILLIS"
#define TIMECHANGE_FIELD_RAW_OFFSET_TZ "RAW_OFFSET_TZ"
#define TIMECHANGE_FIELD_DST_OFFSET_TZ "DST_OFFSET_TZ"

#define SHUTDOWN_CARD ""
#define SHUTDOWN_FIELD_ENABLED "IS_SHUTDOWN"

#define ASSISTEDGPS_CARD ""
#define ASSISTEDGPS_FIELD_ENABLED "IS_ASSISTED_GPS_ENABLED"

#define NETWORKINFO_CARD "ACTIVE_NETWORK_INFO"
#define NETWORKINFO_FIELD_TYPE "TYPE"
#define NETWORKINFO_FIELD_TYPENAME "TYPE_NAME"
#define NETWORKINFO_FIELD_SUBTYPENAME "SUB_TYPE_NAME"
#define NETWORKINFO_FIELD_AVAILABLE "IS_AVAILABLE"
#define NETWORKINFO_FIELD_CONNECTED "IS_CONNECTED"
#define NETWORKINFO_FIELD_ROAMING "IS_ROAMING"

#define SERVICESTATUS_CARD ""
#define SERVICESTATUS_FIELD_STATE "CELL_NETWORK_STATUS"

#define MODEL_CARD ""
#define MODEL_FIELD_NAME "MODEL"

#define MANUFACTURER_CARD ""
#define MANUFACTURER_FIELD_NAME "MANUFACTURER"

#define OSSTATUS_CARD "ACTIVE_NETWORK_INFO"

#define RILSERVICEINFO_CARD "RIL-SERVICE-INFO"
//#define RILSERVICEINFO_FIELD_VALID_MASK
#define RILSERVICEINFO_FIELD_ARIF_TYPE_MASK "SUPPORTED-AIRINTERFACE-TYPE-MASK"
#define RILSERVICEINFO_FIELD_CARRIER_ARIF_TYPE "CARRIER-AIRINTERFACE-TYPE"
#define RILSERVICEINFO_FIELD_CARRIER_MCC "MOBILE-COUNTRY-CODE"
#define RILSERVICEINFO_FIELD_CARRIER_MNC "MOBILE-NETWORK-CODE"
//#define RILSERVICEINFO_FIELD_CARRIER_NAME_LEN
#define RILSERVICEINFO_FIELD_CARRIER_NAME "HOME-CARRIER-NAME"

#define RILCELLINFO_CARD "RIL-CELL-UPDATE"
#define RILCELLINFO_FIELD_NETWORK_STATUS "NETWORK-STATUS"
#define RILCELLINFO_FIELD_RIL_TECH_TYPE "RIL-TECH-TYPE"
#define RILLCELLINFO_FIELD_MCC "MOBILE-COUNTRY-CODE"
#define RILLCELLINFO_FIELD_MNC "MOBILE-NETWORK-CODE"
#define RILLCELLINFO_FIELD_LAC "LOCATION-AREA-CODE"
#define RILLCELLINFO_FIELD_CID "CELL-ID"
#define RILLCELLINFO_FIELD_SID "SYSTEM-ID"
#define RILLCELLINFO_FIELD_NID "NETWORK-ID"
#define RILLCELLINFO_FIELD_BSID "BASE-STATION-ID"
#define RILLCELLINFO_FIELD_BSLAT "BASE-STATION-LATITUDE"
#define RILLCELLINFO_FIELD_BSLON "BASE-STATION-LONGITUDE"
#define RILLCELLINFO_FIELD_UTC_TIME_OFFSET "TIME-ZONE-OFFSET"
#define RILLCELLINFO_FIELD_DAYLIGHT_TIMEZONE "IN-DAY-LIGHT-SAVING"
#define RILLCELLINFO_FIELD_TAC "TRACKING-AREA-CODE"
#define RILLCELLINFO_FIELD_PCI "PHYSICAL-CELL-ID"

#define WIFI_SUPPLICANT_STATUS_CARD ""
#define WIFI_SUPPLICANT_FIELD_STATE "WIFI-SUPPLICANT-STATE"

#define TAC_CARD ""
#define TAC_FIELD_NAME "TAC"

#define MCCMNC_CARD ""
#define MCCMNC_FIELD_NAME "MCCMNC"

#endif // #ifndef __IZAT_MANAGER_DATAITEMCONCRETETYPEFIELDNAMES_H__
