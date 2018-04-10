#ifndef __FILEMUX_INTERNAL_DEFS_H__
#define __FILEMUX_INTERNAL_DEFS_H__
/* =============================================================================
                              FileMuxInternalDefs.h
DESCRIPTION
  Filemux feature definitions.
  Copyright (c) 2011-2014, 2016 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

================================================================================
*/

/* =============================================================================
                             Edit History

$Header: //depot/asic/msmshared/users/sateesh/multimedia2/Video/wfd-source/mm-mux/main/FilemuxInternalDefs/inc/filemuxinternaldefs.h#2 $

================================================================================
*/

/* =============================================================================

                     INCLUDE FILES FOR MODULE

================================================================================
 */


/**=============================================================================
  List of Filemux formats go here.  Features specific to a pariticular format
  shall go like.
  #define major
     #ifdef major
	      #define minor
     #endif

  Common features used across all formats shall go in the next section.
================================================================================
*/

/**-----------------------------------------------------------------------------
 This would enable recording of 3gp 3g2 mp4 k3g skm clips
--------------------------------------------------------------------------------
*/
#define FEATURE_FILEMUX_3GP

      /**-----------------------------------------------------------------------
       Enables the async file writing for achieveing performance for 3gp clips
	  --------------------------------------------------------------------------
	  */
	  #ifdef FEATURE_FILEMUX_3GP
		#define FILEMUX_WRITE_ASYNC
	  #endif


/**-----------------------------------------------------------------------------
  .qcp format recording
--------------------------------------------------------------------------------
*/
#undef FEATURE_FILEMUX_QCP


/**-----------------------------------------------------------------------------
 .aac - ADIF, ADTS formats
--------------------------------------------------------------------------------
*/
#undef FEATURE_FILEMUX_AAC


/**-----------------------------------------------------------------------------
 .amr -
--------------------------------------------------------------------------------
*/
#undef FEATURE_FILEMUX_AMR


/**-----------------------------------------------------------------------------
 .wav
--------------------------------------------------------------------------------
*/
#undef FEATURE_FILEMUX_WAV


/**-----------------------------------------------------------------------------
 .evb
--------------------------------------------------------------------------------
*/
#undef FEATURE_FILEMUX_EVRCB


/**-----------------------------------------------------------------------------
 .evw
--------------------------------------------------------------------------------
*/
#undef FEATURE_FILEMUX_EVRCWB

/**-----------------------------------------------------------------------------
 .m2ts
--------------------------------------------------------------------------------
*/
#define FEATURE_FILEMUX_MP2
/**-----------------------------------------------------------------------------
 .secure m2ts
--------------------------------------------------------------------------------
*/
//#define FEATURE_FILEMUX_SECURE_MP2


/**=============================================================================
 All common non format specific features Go here
================================================================================
*/





#endif // __FILEMUX_INTERNAL_DEFS_H__
