#ifndef SNS_SAM_ALGO_H
#define SNS_SAM_ALGO_H

/*============================================================================
  @file sns_sam_algo_api.h

  @brief
  Data structures and constants used by the SAM Framework and the SAM
  algorithms, and should only be used internally to the framework and
  its communication with the algorithms.

  @note
  In general, all queues used as parameters in the algorithm API must be
  processed in-order.  Algorithms are not permitted to pick-and-choose the
  order in which they process incoming input or output queues.  Similarly,
  algorithms may not save pointers to any entry on any queue.

  @note
  File line length should generally be limited to <= 80 columns.

  Copyright (c) 2014-2016 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/

/*===========================================================================
  Include Files
  ===========================================================================*/
#include <stdint.h>
#include "qmi_idl_lib.h"
#include "sns_queue.h"
#include "sns_usmr.h"
#include "fixed_point.h"
#include "sns_common_v01.h"
#include "log.h"
#include "sns_profiling.h"

/*============================================================================
  Preprocessor Definitions and Constants
  ===========================================================================*/

/** @ingroup sam_constants
  Maximum registry groups that a single algorithm might read.
*/
#define SNS_SAM_MAX_REG_GRP 3

/** @name Log packet timestamp type macro
*/
/** @{ */
/** Dedicated Sensors Processing Subsystem (DSPS) timestamp. */
#define SNS_TIMESTAMP_DSPS  0
/** Application Processor Subsystem (APSS) timestamp. */
#define SNS_TIMESTAMP_APPS  1
/** Modem Processor Subsystem (MPSS) timestamp. */
#define SNS_TIMESTAMP_MODEM 2
/** @} */ /* end_name_group Log packet timestamp type macro */
#ifndef UNREFERENCED_PARAMETER
# define UNREFERENCED_PARAMETER(x) (void)x;
#endif /* UNREFERENCED_PARAMETER */

/** Maximum dependent sensors (SMGR + SAM). */
#define SNS_SAM_MAX_DEPENDENT_SENSOR 10
/** @} */ /* end_addtogroup sa_constants */

/*============================================================================
  Extern Declarations
  ===========================================================================*/

/** @ingroup sns_q_check
 * Returns a pointer to the data block at the head of the queue.
 *
 * @param[in] q_ptr Sensors queue pointer; initialized via sns_q_init().
 *
 * @return
 * First item on the queue (head) or NULL if the queue is empty.
*/
void *sns_q_check( sns_q_s const *q_ptr );

/** @ingroup sns_q_next
 * Returns a pointer to the next item on the queue.
 *
 * @param[in] q_ptr Sensors queue pointer; initialized via sns_q_init().
 * @param[in] link_ptr Current item on the queue.
 *
 * @return
 * Next item on the queue or NULL if at the end of the queue.
*/
void *sns_q_next ( sns_q_s const *q_ptr, sns_q_link_s const *link_ptr );

/*============================================================================
  Type Declarations
  ===========================================================================*/

/* Forward declaration.  See sns_sam.h */
struct sns_sam_sensor_algo;
struct sns_sam_algo_inst;
struct sns_sam_algo_callback;

/** @addtogroup sa_datatypes
@{ */
/**
 * Timestamp type used by SAM.
*/
typedef uint64_t sns_sam_timestamp;

/**
 * SAM Algorithm API error codes.
*/
enum sns_sam_err{
  SAM_ENONE = 0,
  /**< No error occurred; success. */
  SAM_EFAILED,
  /**< Unfixable or internal error occurred. */
  SAM_ENOT_SUPPORTED,
  /**< This API is not supported or is not implemented. */
  SAM_ETYPE,
  /**< Function argument contains invalid data type,
   * e.g., unknown message ID, unknown registry group, or unexpected
   * Sensor UID. */
  SAM_ESTATE,
  /**< Algorithm is currently in a bad or invalid state; algorithm is unable to
       recover. */
  SAM_EMAX,
  /**< The algorithm is at the maximum of some resource, e.g., memory, CPU,
       etc. */

/** @cond */
  /* The items below shall not be used by algorithms; only for use in FW */
  /* Not enough memory to complete the request */
  SAM_ENOMEM = 200,
  /* One or more argument values had invalid range */
  SAM_ERANGE,
  /* Sensors Message Router Framework error */
  SAM_ESMR,
  /* This operation is not available at this time */
  SAM_ENOT_AVAILABLE,
/** @endcond */
};
typedef enum sns_sam_err sns_sam_err;

/**
 * Universal sensor IDs from all sources (SAM, SMGR, etc.). SAM algorithms
 * provide the IDs via the attributes query request. SMGR provides them via the
 * single sensor information response.
*/
typedef uint64_t sns_sam_sensor_uid;

/** Generic structure for SAM memory management. Algorithms must never modify
   these fields. */
struct sns_sam_buffer
{
  intptr_t buf;     /**< Buffer. */
  uint32_t bufSize; /**< Buffer size. */
};

/**
 * Opaque data generated by the SAM Algorithm Message API and delivered to the
 * SAM Algorithm API. This is generally a repackage of the enable request
 * message. It is acceptable to use the same data structure for both the
 * message and configuration data. */
typedef struct sns_sam_buffer sns_sam_algo_config;

/**
 * Opaque handle used by the algorithm to store persistent data. Persistent
 * data is associated with the algorithm, not any specific algorithm
 * instance, and the handle remains after an algorithm instance is stopped
 * and removed.  Note that this buffer might be or is shared across multiple
 * algorithm instances; so, the algorithm must not make any concurrency
 * assumptions regarding this data. */
typedef struct sns_sam_buffer sns_sam_algo_persist;

/**
 * Opaque handle used by the algorithm to store state data per the algorithm
 * instance. */
typedef struct sns_sam_buffer sns_sam_algo_state;

/**
 * Number of input samples of a specific sensor type.*/
struct sns_sam_input_stat
{
    sns_sam_sensor_uid * suid; /**< Sensor UID.*/
    uint32_t cnt; /**< Count of sensor samples.*/
};
typedef struct sns_sam_input_stat sns_sam_input_stat;

/**
 * Record of the algorithm's input samples.
 * PEND: This structure should be removed & replaced with a different mechanism.
*/
struct sns_sam_input_q
{
  sns_q_s inputData;
  /**< Algorithm input queue. */
  sns_sam_input_stat  input_stat[SNS_SAM_MAX_DEPENDENT_SENSOR];
  /**< A record of how many samples of each sensor type that are present in 
       the input queue. This can optionally be used by an algorithm. SAM 
       neither sets nor uses this field at any time. */
};
typedef struct sns_sam_input_q sns_sam_input_q;


/**
 * Represents a piece of algorithm input data. */
struct sns_sam_algo_input
{
  sns_q_link_s qLink;
  /**< Data fields necessary to add this object to a SAM list.
       Algorithms must only modify or use this field when explicitly stated. */
  intptr_t data;
  /**< Data item. */
  sns_sam_timestamp timestamp;
  /**< Timestamp. */
};
typedef struct sns_sam_algo_input sns_sam_algo_input;

/**
 * Represents a piece of algorithm output data. */
struct sns_sam_algo_output
{
  sns_q_link_s qLink;
  /**< Data fields necessary to add this object to a SAM list.
       Algorithms must only modify or use this field when explicitly stated. */
  intptr_t data;
  /**< Data item. */
};
typedef struct sns_sam_algo_output sns_sam_algo_output;

/**
 * Generic structure for a QMI message. Algorithms only need to set the
 * msgID field for dependent enable requests. Other than that, the framework
 * handles setting the msgID, msg.bufSize, and sensorUID for all incoming
 * and outgoing messages.
*/
struct sns_sam_msg
{
  sns_q_link_s qLink;
  /**< Data fields necessary to add this object to a SAM list.
     Algorithms must only modify/use this field when explicitly stated. */

  struct sns_sam_buffer msg;
  /**< QMI decoded message. */
  uint32_t msgID;
  /**< QMI message ID. */
  sns_sam_sensor_uid const *sensorUID;
  /**< Sensor from which this message was generated, or the intended destination.
     The SAM Framework exclusively sets this field. @newpagetable */
};

/**
 * QMI decoded message that contains an enable request for some sensor.
 * The req_handle field is opaque from the algorithm. */
typedef struct sns_sam_msg sns_sam_enable_req;

/**
 * QMI decoded message that contains a response message generated by service. */
typedef struct sns_sam_msg sns_sam_resp_msg;

/**
 * QMI decoded message that contains an indication message for some service. */
typedef struct sns_sam_msg sns_sam_ind_msg;

/**
 * QMI decoded message that contains an update request for some service. */
typedef struct sns_sam_msg sns_sam_update_req;

/**
 * Log packet sent via diagnostics. It must point to the stack data. */
typedef struct sns_sam_buffer sns_sam_log_pkt;

/**
 * Specifies a SAM algorithm registry requirement. See sns_reg_api_v02.h for
 * valid registry group IDs. */
typedef int32_t sns_sam_reg_group;

/**
 * Represents a dependent sensor to an algorithm.
*/
struct sns_sam_algo_dep_sensor
{
  sns_sam_sensor_uid sensorUID;
  /**< Universal sensor ID. */
  qmi_idl_service_object_type serviceObj;
  /**< QMI service object to communicate with the sensor type. */
};

/**
 * SAM batch indication.
*/
enum sns_sam_batch_ind_type
{
  SAM_BATCH_ONLY_IND_V01 = 0,
  /**< Standalone batch indication. */
  SAM_BATCH_FIRST_IND_V01 = 1,
  /**< First indication in a stream of back-to-back indications. */
  SAM_BATCH_INTERMEDIATE_IND_V01 = 2,
  /**< Intermediate indication in a stream of back-to-back indications. */
  SAM_BATCH_LAST_IND_V01 = 3
  /**< Last indication in a stream of back-to-back indications. */
};
typedef enum sns_sam_batch_ind_type sns_sam_batch_ind_type;

/**
 * Reporting modes of the SAM algorithms. An algorithm can support multiple
 * modes, but an algorithm instance must be associated with exactly one.
*/
enum sns_sam_report_mode
{
  SNS_SAM_REPORT_MODE_UNKNOWN,
  /**< Unknown reporting mode. */
  SNS_SAM_REPORT_MODE_PERIODIC,
  /**< Generate and send reports at a fixed interval. */
  SNS_SAM_REPORT_MODE_SYNC,
  /**< Generate and send a report upon receipt of specific input. */
  SNS_SAM_REPORT_MODE_ASYNC,
  /**< Generate and send a report at the algorithm's discretion. */
  SNS_SAM_REPORT_MODE_ONESHOT,
  /**< Send a single report, then cancel the algorithm. */


  SNS_SAM_REPORT_MODE_CNT
  /**< SNS_SAM_REPORT_MODE_CNT must be the last entry in this enumeration. */
};
typedef enum sns_sam_report_mode sns_sam_report_mode;

/**
 * SAM algorithm attributes.
*/
/*  Additional items may be added to the end. */
enum sns_sam_algo_attr_type{
  SAM_ALGO_ATTR_REVISION = 0,
  /**< Revision number of the algorithm implemented by the service. */
  SAM_ALGO_ATTR_POWER,
  /**< Power use estimated by this algorithm; does not include dependent
       power. */
  SAM_ALGO_ATTR_REPORT_MODE,
  /**< Bitmask for the supported reporting modes of the algorithm. */
  SAM_ALGO_ATTR_MIN_REPORT,
  /**< Minimum reporting rate supported by this algorithm (Hz, Q16). */
  SAM_ALGO_ATTR_MAX_REPORT,
  /**< Maximum report rate supported (Hz, Q16). */
  SAM_ALGO_ATTR_MIN_SAMPLE,
  /**< Minimum sampling rate supported by this algorithm (Hz, Q16). */
  SAM_ALGO_ATTR_MAX_SAMPLE,
  /**< Maximum sampling rate supported (Hz, Q16). */
  SAM_ALGO_ATTR_MAX_BATCH,
  /**< Maximum number of reports that can be batched/buffered. */
  SAM_ALGO_ATTR_MAX_REPORTS_PER_IND,
  /**< Maximum number of reports per indication. */
  SAM_ALGO_ATTR_CNT
  /**< SAM_ALGO_ATTR_CNT must be the last entry in this enumeration. */
};

/**
 * Represents an <attribute, value> pair.
*/
struct sns_sam_algo_attr
{
  uint64_t attrValue;
  /**< Value associated with the attribute; must not use the entire 64 bits. */
  enum sns_sam_algo_attr_type attribute;
  /**< Sensor attribute. */
  bool supported;
  /**< Whether this attribute is supported by the algorithm; whether attrValue
       is valid. */
};
typedef struct sns_sam_algo_attr sns_sam_algo_attr;

/**
 * Callback function for an algorithm to indicate that all of its dependencies
 * have been met (i.e., both registry and sensors). This function is only
 * called synchronously and within one of the ...dep_*_met() functions. After
 * an algorithm calls this function, there is no mechanism for an algorithm to
 * <i>rescind</i> its vote.  No other algorithm API function (besides
 * ...dep_*_met()) is called until this function is called.
*/
typedef void (*sns_sam_algo_all_dep_met)( struct sns_sam_sensor_algo const *algo );

/**
 * Attributes parsed from the SAM enable request message, as parsed by the
 * algorithm.
*/
struct sns_sam_client_attr
{
  q16_t reportPeriod;
  /**< Minimum reporting rate that is requested. This is zero for non-periodic
       clients.  */
  uint8_t dcPercentOn;
  /**< Fraction of time the algorithm must run (out of 255).
       Zero refers to an opportunistic client, who only receives samples if
       another client exists for this algorithm instance. */
  enum sns_sam_report_mode reportMode;
  /**< Reporting mode requested by the client. */
  sns_suspend_notification_s_v01 notifySuspend;
  /**< Whether to send indications for this algorithm while the processor is
       in the Suspend state. @newpagetable */
};
typedef struct sns_sam_client_attr sns_sam_client_attr;

/**
 * Proxy object for data stored within the sensors registry.
*/
struct sns_sam_reg_data
{
  sns_sam_reg_group groupID;
  /**< Group ID. */
  uint32_t groupDataLen;
  /**< Number of valid bytes in groupData. */
  void *groupData;
  /**< Data read from the registry. */
};
typedef struct sns_sam_reg_data sns_sam_reg_data;

/**
 * Debug string parameters structure.
 * This contains format specifiers and details on the file and line number.
*/
struct sns_sam_algo_debug_params
{
  char const *filename;
  /**< Filename. */
  uint16_t line_num;
  /**< Line number. */
  uint8_t num_params_valid;
  /**< Number of valid parameters. */
  int32_t param1;
  /**< Parameter 1. */
  int32_t param2;
  /**< Parameter 2. */
  int32_t param3;
  /**< Parameter 3. */
};
typedef struct sns_sam_algo_debug_params sns_sam_algo_debug_params;
/** @} */ /* end_addtogroup sa_datatypes */

/** @ingroup sns_sam_algo_timer_cb
 * Callback function for timer events.
 *
 * @datatypes
 * #sns_sam_algo_state
 *
 * @param[in] algoInst This algorithm instance.
 * @param[in] cbFunc Provides additional functionality to the algorithms.
 * @param[in,out] algoState Algorithm state that might be updated as a result.
 *
 * @return
 * None.
 *
 * @dependencies
 * sns_sam_algo_callback::sns_sam_algo_timer_reg() must be called previously by
 * the algorithm instance with a nonzero timeout value.
 */
typedef void (*sns_sam_algo_timer_cb)(
  struct sns_sam_algo_inst const *algoInst,
  struct sns_sam_algo_callback const *cbFunc,
  sns_sam_algo_state *algoState );

/** @ingroup sns_sam_algo_callback
 * Callback functions into the SAM Framework from the SAM algorithms. These
 * can be implemented now as simply utility functions exposed to the algorithms.
 * However, that approach might not work in a dynamic linking environment.
*/
struct sns_sam_algo_callback
{
  uint32_t structSize; /**< Size of this structure. */

  /**
   * Opaque handle back to the SAM Framework's algorithm instance object. This
   * is used within callback functions so the framework knows from which
   * algorithm the request originates. */
  struct sns_sam_algo_inst const *algoInst;

/** @addtogroup sns_sam_algo_callback_functions
@{ */
  /**
   * Sends a write request to the sensors registry. This function is processed
   * asynchronously.
   *
   * @datatypes
   * #sns_sam_reg_data
   *
   * @param[in] algoInst This algorithm instance.
   * @param[in,out] regData Registry data to write.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_ETYPE -- Invalid registry ID or wrong data length. \n
   * SAM_EFAILED -- Error occurred when the request was sent; no response is expected.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_reg_req)(
      struct sns_sam_algo_inst const *algoInst, sns_sam_reg_data *regData );

  /**
   * Logs a message string generated by the algorithm. The SAM Framework might
   * do nothing with the message, or log it to one of several buffers.
   *
   * @datatypes
   * #sns_sam_algo_debug_params
   *
   * @param[in] algoInst This algorithm instance.
   * @param[in] message Debug string.
   * @param[in] fmt_params_ptr Parameters to the debug string.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_EFAILED -- Error occurred while the log was processed. \n
   * SAM_ETYPE -- String was improperly formatted.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_log)(
      struct sns_sam_algo_inst const *algoInst,
      char const *message,
      sns_sam_algo_debug_params const *fmt_params_ptr );

  /**
   * Indicates to the SAM Framework that the list or qualities of the
   * algorithm's dependent sensors has changed. The framework then calls
   * sns_sam_algo_msg_api::sns_sam_algo_gen_req() for the sensor; the
   * algorithm returns SAM_ESTATE to cancel the sensor stream.
   *
   * @note1hang Algorithms are only allowed one stream per dependent sensor.
   *
   * @param[in] algoInst This algorithm instance.
   * @param[in] sensorUID Dependent sensor that has been changed, added,
   *                      or removed.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_ETYPE -- Invalid or unknown sensor ID. \n
   * SAM_EFAILED -- An error occurred; the sensor change was not recorded.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_sensor_change)(
      struct sns_sam_algo_inst const *algoInst,
      sns_sam_sensor_uid const *sensorUID );

  /**
   * Allocates memory for a single input data, adds that item to the algorithm
   * instance input queue, and copies the supplies contents onto the buffer.
   *
   * @note1hang inputData must have the length as supplied in
   *            sns_sam_algo_api::sns_sam_algo_mem_req().
   *
   * @param[in] algoInst This algorithm instance.
   * @param[in] inputData Data to be added to the input queue.
   * @param[in] timestamp SAM arranges the input data in the input queue in
   *                      chronological order using the timestamp provided 
   *                      here.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_EFAILED -- No more memory is presently available on the system. \n
   * SAM_EMAX -- Algorithm is not allowed any more input allocations.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_add_input)(
      struct sns_sam_algo_inst const *algoInst,
      intptr_t inputData,
      sns_sam_timestamp timestamp );

  /**
   * Allocates memory for a single output data, adds that item to the algorithm
   * instance output queue, and copies the supplies contents onto the buffer.
   *
   * @note1hang outputData must have the length as supplied in
   *            sns_sam_algo_api::sns_sam_algo_mem_req().
   *
   * @param[in] algoInst This algorithm instance.
   * @param[out] outputData Data to be added to the output queue.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_EFAILED -- No more memory is presently available on the system. \n
   * SAM_EMAX -- Algorithm is not allowed any more output allocations.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_add_output)(
      struct sns_sam_algo_inst const *algoInst,
      intptr_t outputData );

  /**
   * Submits a log packet to diagnostics. The log buffer is not saved by SAM.
   *
   * @param[in] algoInst This algorithm instance.
   * @param[in] logPktID Log packet ID.
   * @param[in] logPktSize Size of the log packet.
   * @param[in] logPkt Packet packet to submit.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_EFAILED -- Unable to submit the log packet successfully.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_log_submit)(
      struct sns_sam_algo_inst const *algoInst,
      uint32_t logPktID,
      uint32_t logPktSize,
      void* logPkt );

  /**
   * Indicates to the SAM Framework that the algorithm would prefer that the
   * associated sensor stream be batched. This request persists until
   * the algorithm calls this function or sns_sam_algo_sensor_change().
   *
   * @param[in] algoInst This algorithm instance.
   * @param[in] sensorUID Dependent sensor that was already enabled.
   * @param[in] batchPeriod Maximum time (in seconds Q16) between
   *                        batched indications.
   * @param[in] flushFull Whether to flush the buffer when full.
   *                      - TRUE -- Wake the client to flush the full buffer
   *                      - FALSE -- Replace with oldest if the buffer is full
   *                        and the client is suspended. @tablebulletend
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_ETYPE -- No active stream is found with this sensor.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_req_batch)(
      struct sns_sam_algo_inst const *algoInst,
      sns_sam_sensor_uid const *sensorUID,
      uint32_t batchPeriod,
      bool flushFull );

  /**
   * Registers a timer callback. Only one timer is active per algorithm
   * instance; subsequent requests update or disable the existing timer.
   *
   * @note1hang If a timer is disabled by the algorithm, it is guaranteed
   *            not to receive the timeout event.
   *
   * @param[in] algoInst This algorithm instance.
   * @param[in] timeout Nanoseconds until the algorithm is alerted;
   *                    zero to disable the current active timer.
   * @param[in] timerCB Callback function, sns_sam_algo_timer_cb(), to register
   *                    for this timer event.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_EFAILED -- Unable to successfully register the timer.
   *
   * @dependencies
   * None.
   * 
   * @newpage 
   */
  sns_sam_err (*sns_sam_algo_timer_reg)(
      struct sns_sam_algo_inst const *algoInst,
      uint64_t timeout,
      sns_sam_algo_timer_cb timerCB );

  /**
   * Logs Qualcomm Debug Subsystem (QDSS) events for algorithm update per 
   * input.
   *
   * @param[in] algoInst This algorithm instance.
   * @param[in] qdssEventId QDSS software event.
   * @param[in] inputType Input type for the algorithm depending on the inputs
   *                      from multiple sensors. For an algorithm depending on
   *                      only one kind of sensor input, 0 is used.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   *
   * @dependencies
   * None.
   */
  sns_sam_err (*sns_sam_algo_qdss_algo_update)(
      struct sns_sam_algo_inst const *algoInst,
      sns_tracer_event qdssEventId,
      uint32_t inputType );
/** @} */ /* end_addtogroup sns_sam_algo_callback_functions */
};
typedef struct sns_sam_algo_callback sns_sam_algo_callback;

/** @addtogroup sa_datatypes
@{ */
/**
 * Algorithm-provided data that specifies the sizes of its data objects. All
 * fields must be initialized to valid values. Zero is acceptable if the
 * algorithm does not intend to use that data handle.
*/
struct sns_sam_algo_buf_size
{
  uint32_t structSize;     /**< Size of this structure. */

  uint32_t algoStateSize;  /**< Algorithm state size. */
  uint32_t outputDataSize; /**< Output data size. */
  uint32_t inputDataSize;  /**< Input data size. @newpagetable */
};
typedef struct sns_sam_algo_buf_size sns_sam_algo_buf_size;

/**
 * Algorithm-provided data that specifies the buffer sizes of all constant-size
 * data objects, i.e., the data objects whose sizes are not due to the
 * algorithm configuration.
*/
struct sns_sam_algo_const_buf_size
{
  uint32_t structSize;      /**< Size of this structure. */

  uint32_t configDataSize;  /**< Configuration data size. */
  uint32_t persistDataSize; /**< Persistent data size. */
};
typedef struct sns_sam_algo_const_buf_size sns_sam_algo_const_buf_size;
/** @} */ /* end_addtogroup sa_datatypes */

/** @ingroup sns_sam_algo_api
 * All algorithms must implement this API to be used by the SAM
 * Framework. This API performs the actual algorithm computations. It is to
 * have no knowledge of its associated QMI IDL nor the IDL messages of any of
 * its dependencies.
*/
 struct sns_sam_algo_api
{
  uint32_t structSize; /**< Size of this structure. */

/** @addtogroup sam_algo_api_functions
@{ */
  /**
   * Generates the memory requirements algorithm data buffers that vary based
   * on the algorithm configuration.
   *
   * @note1hang Algorithms must not exceed the size of the structure.
   *
   * @datatypes
   * #sns_sam_algo_config \n
   * #sns_sam_algo_buf_size
   *
   * @param[in] configData Data generated by
   *                       sns_sam_algo_msg_api::sns_sam_algo_process_enable_req().
   * @param[out] bufferSizes Buffer sizes associated with this algorithm
   *                         configuration.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * @par
   * SAM_EFAILED -- Algorithm was unable to determine its buffer sizes;
   *                this error causes the framework not to use the algorithm.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_mem_req)(
      sns_sam_algo_config const *configData,
      sns_sam_algo_buf_size *bufferSizes );

  /**
   * Resets and initializes an algorithm based on configuration data.
   *
   * @datatypes
   * #sns_sam_algo_config \n
   * #sns_sam_algo_callback \n
   * #sns_sam_algo_persist \n
   * #sns_sam_algo_state
   *
   * @param[in] configData Data created by
   *                       sns_sam_algo_msg_api::sns_sam_algo_process_enable_req().
   * @param[in] cbFunc Provides additional functionality to the algorithms.
   * @param[in] persistData Algorithm-wide persistent data.
   * @param[out] algoState Initialized state data.
   *
   *
   * @detdesc
   * This function is called upon an algorithm instance creation, or when the
   * algorithm configuration changes. Either case causes the algorithm instance
   * state to be reset.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * SAM_EFAILED -- Algorithm failed to initialize; the framework might
   *                choose to try again. \n
   * SAM_ESTATE  -- Based on the configuration data, the algorithm cannot be
   *                initialized.
   *
   * @dependencies
   * sns_sam_algo_all_dep_met() must be called by the algorithm before the SAM
   * Framework will use this function.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_reset)(
      sns_sam_algo_config const *configData,
      sns_sam_algo_callback const *cbFunc,
      sns_sam_algo_persist *persistData,
      sns_sam_algo_state *algoState );

  /**
   * Updates an algorithm with some input data. Zero or more pieces of output
   * data might be generated.
   *
   * @datatypes
   * #sns_sam_algo_state \n
   * #sns_sam_algo_callback \n
   * #sns_sam_algo_persist
   *
   * @param[in] algoState Algorithm state that might be updated as a result.
   * @param[in] cbFunc Provides additional functionality to the algorithms.
   * @param[in,out] inputData List of input data objects to be processed.
   * @param[in,out] persistData Algorithm-wide persistent data.
   * @param[out] processedCnt Number of items processed or to be removed from
   *                          inputData.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * SAM_EFAILED -- Algorithm failed to process any data at this time. \n
   * SAM_ESTATE  -- Algorithm cannot process any further input. \n
   * SAM_EMAX    -- Algorithm is unable to create enough output objects.
   *
   * @dependencies
   * sns_sam_algo_reset() must be called prior to this function.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_update)(
      sns_sam_algo_state *algoState,
      sns_sam_algo_callback const *cbFunc,
      sns_sam_input_q const *inputData,
      sns_sam_algo_persist *persistData,
      uint32_t *processedCnt );


  /**
   * Generates output data based on the current state of the algorithm.  One use
   * case of this function is to service a get report request.
   *
   * @note1hang This function does not require implementation for algorithms
   *            that do not support periodic reporting nor have a
   *            get report request message. In which case, it can be set
   *            to NULL. If there is no output to be generated, this function
   *            must return SAM_ESTATE.
   *
   * @datatypes
   * #sns_sam_algo_state \n
   * #sns_sam_algo_output \n
   * #sns_sam_algo_callback
   *
   * @param[in] algoState Currently active algorithm instance.
   * @param[in] cbFunc Provides additional functionality to the algorithms.
   * @param[out] outputData Generated output.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_EFAILED -- Algorithm failed to generate output data. \n
   * SAM_ESTATE -- Algorithm cannot presently generate output data.
   *
   * @dependencies
   * sns_sam_algo_reset() must be called prior to this function.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_generate)(
      sns_sam_algo_state *algoState,
      sns_sam_algo_callback const *cbFunc,
      sns_sam_algo_output *outputData );

  /**
   * Determines if the specified configuration is compatible with the existing
   * algorithm instance.
   *
   * @datatypes
   * #sns_sam_algo_state \n
   * #sns_sam_algo_config
   *
   * @param[in] algoState Currently active algorithm instance.
   * @param[in] configData Proposed configuration from a new client.
   *
   * @return
   * TRUE -- Configuration is compatible. \n
   * FALSE -- New instance must be created.
   *
   * @dependencies
   * sns_sam_algo_reset() must be called prior to this function.
   *
   * @newpage
   */
  bool (*sns_sam_algo_check_config)(
      sns_sam_algo_state const *algoState,
      sns_sam_algo_config const *configData );
/** @} */ /* end_addtogroup sam_algo_api_functions */
};
typedef struct sns_sam_algo_api sns_sam_algo_api;

/** @ingroup sns_sam_algo_msg_api
 * Translation layer between QMI messages and the algorithm API defined
 * in sns_sam_algo_api. The implementation must not be computationally
 * intensive.
 *
 * @dependencies
 * The algo argument is memset once to 0 prior to the use of any of these
 * functions.
 */
struct sns_sam_algo_msg_api
{
  uint32_t structSize; /**< Size of this structure. */

/** @addtogroup sns_sam_algo_msg_api_functions
@{ */
  /**
   * Generates the memory requirements algorithm data buffers that are
   * independent of the algorithm configuration.
   *
   * @note1hang Algorithms must not exceed the size of the structure.
   *
   * @datatypes
   * #sns_sam_algo_const_buf_size
   *
   * @param[out] bufferSizes Data length, in bytes.
   *
   * @return
   * None.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  void (*sns_sam_algo_mem_const_req)(
      sns_sam_algo_const_buf_size *bufferSizes );

  /**
   * Obtains all attributes of an algorithm.
   *
   * @datatypes
   * #sns_sam_algo_persist \n
   * #sns_sam_algo_attr
   *
   * @param[in] persistData Persistent data for the algorithm.
   * @param[in,out] attributes Sensor attributes. The algorithm is expected
   *                           to set the value and supported fields for
   *                           all entries. SAM_ALGO_ATTR_CNT marks the end of
   *                           the array.
   *
   * @return
   * None.
   *
   * @dependencies
   * sns_sam_algo_all_dep_met() must be called by the algorithm before the SAM
   * Framework will use this function.
   *
   * @newpage
   */
  void (*sns_sam_algo_get_attr)(
      sns_sam_algo_persist const *persistData,
      sns_sam_algo_attr attributes[] );

  /**
   * Determines whether the algorithm's sensor dependencies have been met.
   * The algorithm is expected to save the sensorUID pointer for use in the
   * request messages.
   *
   * @datatypes
   * #sns_sam_algo_all_dep_met \n
   * #sns_sam_algo_persist \n
   * #sns_sam_algo_attr
   *
   * @param[in] algo Opaque handle to the algorithm to be used in depMetFunc.
   * @param[in] depMetFunc See #sns_sam_algo_all_dep_met.
   * @param[in] persistData Persistent data for the algorithm.
   * @param[in] sensorUID Dependent sensor that is now available.
   * @param[in] attr Pointer to an array of attributes for sensorUID.
   *
   * @detdesc
   * This function continues to be called after it returns TRUE the first time
   * with other dependent sensors. The SAM Framework reserves the right to
   * call this function multiple times for the same Sensor UID.
   *
   * @note1hang The algorithm's response to sns_sam_algo_dep_sensors()
   *            determines which sensors are advertised to it here.
   *
   * @return
   * None.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  void (*sns_sam_algo_dep_sensors_met)(
      struct sns_sam_sensor_algo const *algo,
      sns_sam_algo_all_dep_met depMetFunc,
      sns_sam_algo_persist *persistData,
      sns_sam_sensor_uid const *sensorUID,
      sns_sam_algo_attr const (*sns_sam_attr_array)[ SAM_ALGO_ATTR_CNT ] );

  /**
   * Determines whether the algorithm's registry dependencies have been met.
   *
   * @note1hang This function does not require implementation for algorithms
   *            with no registry groups to read. In which case, it can be set
   *            to NULL.
   *
   * @datatypes
   * #sns_sam_algo_all_dep_met \n
   * #sns_sam_algo_persist \n
   * #sns_sam_reg_data
   *
   * @param[in] algo Opaque handle to the algorithm to be used in depMetFunc.
   * @param[in] depMetFunc See #sns_sam_algo_all_dep_met.
   * @param[in] persistData Persistent data for the algorithm.
   * @param[in] regData Registry data group.
   *
   * @detdesc
   * This function continues to be called after it returns TRUE the first
   * time with other dependent registry data. If the SAM Framework
   * does not determine that a specified registry group is or will be
   * available, the framework passes in a registry data size of zero.
   * @par
   * The SAM Framework reserves the right to call this function multiple
   * times for the same registry group.
   *
   * @note1hang The algorithm's response to sns_sam_algo_reg_grps()
   *            determines which registry data is distributed to it here.
   * @return
   * None.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  void (*sns_sam_algo_dep_registry_met)(
      struct sns_sam_sensor_algo const *algo,
      sns_sam_algo_all_dep_met depMetFunc,
      sns_sam_algo_persist *persistData,
      sns_sam_reg_data const *regData );

  /**
   * Gets the list of an algorithm's dependent sensors. These are both
   * dependent SAM algorithms and dependent SMGR sensors and events.
   *
   * @datatypes
   * #sns_sam_algo_dep_sensor
   *
   * @param[in,out] sensorCount    Maximum number of dependent sensors allowed;
   *                               the algorithm must set the number of sensors
   *                               returned.
   * @param[out] dependentSensors  Sensors required by this algorithm.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_EMAX -- Algorithm requires more sensors than the framework allows.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_dep_sensors)(
      uint32_t *sensorCount,
      struct sns_sam_algo_dep_sensor dependentSensors[] );

  /**
   * Gets the list of an algorithm's registry groups.
   *
   * @param[in,out] registryGrpCount Maximum number of registry groups allowed;
   *                                 number of registry groups returned.
   * @param[out] registryGroups      Registry groups requested by this
   *                                 algorithm; must be a static array
   *                                 allocated by the algorithm.
   *
   * @detdesc
   * On the algorithm's behalf, the framework requests the associated registry
   * data. There is no guarantee that the data is  available at the algorithm
   * instantiation, at which time the algorithm can refuse the request or
   * use some internal default.
   *
   * @return
   * SAM_ENONE -- Operation was successful. \n
   * SAM_EMAX  -- Algorithm requires more registry data than the framework
   *              allows.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_reg_grps)(
      uint32_t *registryGrpCount,
      sns_sam_reg_group registryGroups[] );

  /**
   * Processes/parses an incoming enable IDL request message for this algorithm.
   *
   * @datatypes
   * #sns_sam_enable_req \n
   * #sns_sam_algo_persist \n
   * #sns_sam_client_attr \n
   * #sns_sam_algo_config
   *
   * @param[in] enableReq QMI decoded request message.
   * @param[in] persistData Persistent data for the algorithm, i.e., registry.
   * @param[in,out] clientAttr Basic request information to be parsed by
   *                           the algorithm. Default values are set by the
   *                           framework.
   * @param[in,out] configData Configuration data for the algorithm instance.
   *
   * @detdesc
   * The algorithm must fill in the applicable clientAttr fields and save
   * any algorithm-specific configuration parameters into configData. The
   * algorithm can choose to use defaults (registry or hardcoded), which are
   * presumably kept in the persistData.
   *
   * @note1hang This is the appropriate function to create and commit the
   *            configuration log packet.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * @par
   * SAM_EFAILED -- Unable to process the enable request. An error response
   *                is sent to the client. \n
   * @par
   * SAM_ESTATE  -- Current persist or static data does not permit this
   *                operation. For example, this error is sent if parameter
   *                validation fails because the specified value is outside
   *                the supported range.
   *
   * @dependencies
   * sns_sam_algo_all_dep_met() must be called by the algorithm before the SAM
   * Framework will use this function.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_process_enable_req)(
      sns_sam_enable_req const *enableReq,
      sns_sam_algo_persist *persistData,
      sns_sam_client_attr *clientAttr,
      sns_sam_algo_config *configData );

  /**
   * Processes an incoming update request message for this algorithm instance.
   *
   * @note1hang This function does not require implementation for algorithms
   *            with no supported update request message. In which case, it
   *            can be set to NULL.
   *
   * @datatypes
   * #sns_sam_algo_callback \n
   * #sns_sam_update_req \n
   * #sns_sam_algo_persist \n
   * #sns_sam_algo_state \n
   * #sns_sam_resp_msg
   *
   * @param[in] cbFunc Provides additional functionality to the algorithms.
   * @param[in] updateReq QMI decoded request message.
   * @param[in] persistData Persistent data for the algorithm, i.e., registry.
   * @param[in,out] algoState Algorithm state to be updated.
   * @param[out] respMsg Algorithm's response to the update request.
   *
   * @detdesc
   * This request is algorithm-specific, and it can reset statistics, update
   * the configuration, etc. Typically, algorithms that support this function
   * always return FALSE in sns_sam_algo_api::sns_sam_algo_check_config().
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * SAM_EFAILED -- Unable to process the request. An error response
   *                is sent to the client. \n
   * SAM_ESTATE  -- Current algorithm state does not permit this operation.
   *
   * @dependencies
   * sns_sam_algo_api::sns_sam_algo_reset() must be called prior to this function.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_process_update_req)(
      sns_sam_algo_callback const *cbFunc,
      sns_sam_update_req const *updateReq,
      sns_sam_algo_persist const *persistData,
      sns_sam_algo_state *algoState,
      sns_sam_resp_msg *respMsg );

  /**
   * Processes/parses an incoming indication from one of this algorithm's sensor
   * dependencies.
   *
   * @datatypes
   * #sns_sam_algo_callback \n
   * #sns_sam_ind_msg
   *
   * @param[in] cbFunc Provides additional functionality to the algorithms.
   * @param[in] indMsg QMI decoded indication message.
   * @param[in,out] inputData List of existing or unprocessed input data.
   *
   * @note1hang inputData entries are allocated via
   *            sns_sam_algo_callback::sns_sam_algo_add_input().
   *
   *
   * @detdesc
   * One or more input data objects can be added to the queue. If <i>unfilled</i>
   * data objects already exist on the queue, the algorithm can simply update
   * those with this new data, rather than create new objects.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * @par
   * SAM_EFAILED -- Unable to process the indication. An error indication
   *                might be sent to the client.
   * @par
   * SAM_EMAX    -- Algorithm requires more input data than the framework
   *                allows. Receipt of this error might result in lost samples.
   *                The framework might also choose to call again in the
   *                future.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_process_ind)(
      sns_sam_algo_callback const *cbFunc,
      sns_sam_ind_msg const *indMsg,
      sns_sam_input_q *inputData );

  /**
   * Generates an indication from a piece of algorithm output data.
   *
   * @datatypes
   * #sns_sam_algo_callback \n
   * #sns_sam_algo_output \n
   * #sns_sam_ind_msg
   *
   * @param[in] cbFunc Provides additional functionality to the algorithms.
   * @param[in] outputData First item on the algorithm output data queue.
   * @param[in] extInstanceID ID assigned by SAM to refer to this client
   *                          request.
   * @param[out] indMsg Indication message generated from the output data.
   *                    The message is preallocated and zeroed; bufferSize field
   *                    is set.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * SAM_EFAILED -- Unable to generate an indication message. The framework
   *                might try again. \n
   * SAM_ESTATE  -- Output data cannot be formed into an indication.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_gen_ind)(
      sns_sam_algo_callback const *cbFunc,
      sns_sam_algo_output const *outputData,
      uint8_t extInstanceID,
      sns_sam_ind_msg *indMsg );

  /**
   * Generates a batched indication from the algorithm output data. The
   * algorithm is obligated to fill in all applicable fields except the
   * instance ID.
   *
   * @note1hang This function does not require implementation for algorithms
   *            that do not support a batch operation. In which case, it
   *            can be set to NULL.
   *
   * @datatypes
   * #sns_sam_algo_callback \n
   * #sns_sam_batch_ind_type \n
   * #sns_sam_ind_msg
   *
   * @param[in] cbFunc        Provides additional functionality to the
   *                          algorithms.
   * @param[in] extInstanceID ID assigned by SAM to refer to this client
   *                          request.
   * @param[in] indType       Value for the ind_type field of the batched
   *                          indication message.
   * @param[in] outputData    Output data queue.
   * @param[out] dataCount    How many data items were processed.
   * @param[out] indMsg       Indication message generated from the output data.
   *                          The message is preallocated and zeroed; bufferSize
   *                          field is set.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * SAM_EFAILED -- Unable to generate an indication message. The framework
   *                might try again. \n
   * SAM_ESTATE  -- Output data cannot be formed into an indication.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_gen_batch_ind)(
      sns_sam_algo_callback const *cbFunc,
      uint8_t extInstanceID,
      sns_sam_batch_ind_type indType,
      sns_q_s const *outputData,
      uint32_t *dataCount,
      sns_sam_ind_msg *indMsg );

  /**
   * Generates the report response message. This function is similar to
   * sns_sam_algo_process_ind(), but it is given the output from
   * sns_sam_algo_api::sns_sam_algo_generate() instead.
   *
   * @note1hang The SAM Framework sets the instance_id message field.
   *
   * @note1hang This function does not require implementation for algorithms
   *            that do not a get report request message. In which case, it
   *            can be set to NULL.
   *
   * @datatypes
   * #sns_sam_algo_output \n
   * #sns_sam_resp_msg
   *
   * @param[in] outputData First item on the algorithm output data queue.
   * @param[out] respMsg Response message generated from the output data.
   *                     The message is preallocated and zeroed; bufferSize field
   *                     is set.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * SAM_EFAILED -- Unable to generate a response message. The framework
   *                might try again. \n
   * SAM_ESTATE  -- Output data cannot be formed into a response.
   *
   * @dependencies
   * None.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_gen_report)(
      sns_sam_algo_output const *outputData,
      sns_sam_resp_msg *respMsg );

  /**
   * Generates the enable request message for the specified dependent sensor.
   *
   * @datatypes
   * #sns_sam_algo_state \n
   * #sns_sam_enable_req
   *
   * @param[in] algoState Opaque handle to the algorithm state data.
   * @param[in] sensorUID Sensor to enable.
   * @param[in,out] enableReqMsg Preallocated request message.
   *
   * @return
   * SAM_ENONE   -- Operation was successful. \n
   * SAM_EFAILED -- Unable to generate a message. The framework
   *                might try again. \n
   * SAM_ESTATE  -- Algorithm does not require this Sensor UID.
   *
   * @dependencies
   * sns_sam_algo_api::sns_sam_algo_reset() must be called prior to this function.
   *
   * @newpage
   */
  sns_sam_err (*sns_sam_algo_gen_req)(
      sns_sam_algo_state const *algoState,
      sns_sam_sensor_uid const *sensorUID,
      sns_sam_enable_req *enableReqMsg );

  /**
   * Checks if a proposed sensor dependency request is acceptable to this
   * algorithm instance. The SAM Framework reserves the right to combine
   * streams from a single service, but maintains the requirements
   * specified in these enable request messages.
   *
   * @datatypes
   * #sns_sam_algo_state \n
   * #sns_sam_enable_req
   *
   * @param[in] algoState Existing algorithm state.
   * @param[in] enableReq New/proposed enable request.
   *
   * @return
   * TRUE -- Proposed enable request can replace the existing one. \n
   * FALSE -- Algorithm must maintain its stream.
   *
   * @dependencies
   * sns_sam_algo_api::sns_sam_algo_reset() must be called prior to this function.
   *
   * @newpage
   */
  bool (*sns_sam_algo_check_sensor)(
      sns_sam_algo_state const *algoState,
      sns_sam_enable_req const *enableReq );

  /**
   * The SAM Framework notifies of a change in the reporting associated with
   * one of the clients of the algorithm instance. The algorithms can decide
   * to update their dependent sensor requests accordingly. 
   *
   * @note1hang This function does not require implementation for algorithms
   *            that do not change operation based on client reporting. In
   *            which case, it can be set to NULL.
   *
   * @datatypes
   * #sns_sam_algo_callback \n
   * #sns_sam_algo_persist \n
   * #sns_sam_algo_state
   *
   * @param[in] cbFunc Provides additional functionality to the algorithms.
   * @param[in] algoState Existing algorithm state.
   * @param[in] minReportPeriod Aggregate smallest output report period, 
   *                            in seconds Q16.
   * @param[in] flushFull Whether to flush the buffer when full.
   *                      - TRUE -- Wake the client to flush the full buffer
   *                      - FALSE -- Replace with oldest if the buffer is full
   *                        and the client is suspended. @tablebulletend
   *
   * @return
   * None.
   *
   * @dependencies
   * sns_sam_algo_api::sns_sam_algo_reset() must be called prior to this function.
   */
  void (*sns_sam_algo_handle_report_change)(
      sns_sam_algo_callback const *cbFunc,
      sns_sam_algo_persist *persistData,
      sns_sam_algo_state const *algoState,
      uint32_t minReportPeriod,
      bool flushFull );
/** @} */ /* end_addtogroup sns_sam_algo_msg_api_functions */
};
typedef struct sns_sam_algo_msg_api sns_sam_algo_msg_api;

/** @ingroup sns_sam_algo_register
 * This function is registered for each algorithm. It returns basic information
 * about the algorithm and how the SAM Framework can use it. It is only called
 * once during the SAM Framework initialization.
 *
 * @datatypes
 * #sns_sam_algo_api \n
 * #sns_sam_algo_msg_api
 *
 * @param[out] algoAPI Algorithm API; function is NULL if not supported.
 * @param[out] msgAPI Algorithm Message API; function is NULL if not supported.
 * @param[out] serviceObj QMI Service object associated with this algorithm.
 * @param[out] sensorUID Universal Sensor ID associated with this algorithm.
 *
 * @return
 * SAM_ENONE -- Operation was successful. \n
 * SAM_ENOT_SUPPORTED -- Algorithm does not support the API.
 *
 * @dependencies
 * None.
 */
typedef sns_sam_err (*sns_sam_algo_register)(
      sns_sam_algo_api **algoAPI,
      sns_sam_algo_msg_api **msgAPI,
      qmi_idl_service_object_type *serviceObj,
      sns_sam_sensor_uid *sensorUID );

#endif /* SNS_SAM_ALGO_H */
