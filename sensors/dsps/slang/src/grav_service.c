/* ===================================================================
** Copyright (c) 2015 Qualcomm Technologies, Inc.
** All Rights Reserved.
** Confidential and Proprietary - Qualcomm Technologies, Inc.
**
** FILE: grav_service.c
** DESC: sensor language - slang - sam gravity vector support
** Revision History
** 11-09-2015       df      Fix static analysis errors
** ================================================================ */
#include "sam_common.h"

#include "sensor1.h"
#include "sns_smgr_common_v01.h"
#include "sns_sam_gravity_vector_v01.h"

#include <stdio.h>      // sprintf

/**
 * @name GravSetReq
 * @brief set field in the req_msg to the value specified by
 *        t_keyvalue.
 * @param[io] req_msg
 * @param[i] t_keyvalue
 */
void GravSetReq( request *req_msg, const token *t_keyvalue)
{
    void *body = req_msg->payload;
    int32_t msg_id = req_msg->header.msg_id;

    char *fieldname = t_keyvalue->pSymbol;
    if ( fieldname == NULL) {
        RTErrorMsg( "GravSetReq: pSymbol == NULL\n");
        return;
    }

    if ( msg_id == SNS_SAM_GRAVITY_ENABLE_REQ_V01) {
        sns_sam_gravity_enable_req_msg_v01 *payload = body;
        if ( strcasecmp( fieldname, get_literal( REPORT_PERIOD)) == 0) {
            if ( t_keyvalue->datatype == INTEGER) {
                uint32_t value = (int)( t_keyvalue->data.i * 65536);                     // convert to Q16
                payload->report_period = value;
            }
            else if ( t_keyvalue->datatype == FLOAT) {
                uint32_t value = (int)( t_keyvalue->data.d * 65536);                     // convert to Q16
                payload->report_period = value;
            }
            else {
                ErrReasonForField( BAD_VALUE, fieldname);
            }
        }
        else if ( strcasecmp( fieldname, get_literal( SAMPLERATE)) == 0) {
            if ( t_keyvalue->datatype == INTEGER) {
                payload->sample_rate_valid = 1;
                uint32_t value = (int)( t_keyvalue->data.i * 65536);                     // convert to Q16
                payload->sample_rate = value;
            }
            else if ( t_keyvalue->datatype == FLOAT) {
                payload->sample_rate_valid = 1;
                uint32_t value = (int)( t_keyvalue->data.d * 65536);                     // convert to Q16
                payload->sample_rate = value;
            }
            else {
                ErrReasonForField( BAD_VALUE, fieldname);
            }
        }
        else if ( strcmp( fieldname, get_literal( NOTIFY_SUSPEND)) == 0) {
            payload->notify_suspend_valid = 1;
            payload->notify_suspend.proc_type = 0;
            payload->notify_suspend.send_indications_during_suspend = t_keyvalue->data.i;
        }
        else {
            InvalidFieldName( fieldname);
        }
    }
    else if ( msg_id == SNS_SAM_GRAVITY_DISABLE_REQ_V01) {
        sns_sam_gravity_disable_req_msg_v01 *payload = body;
        if ( strcmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
            payload->instance_id = (uint8_t)t_keyvalue->data.i;
        }
        else {
            InvalidFieldName( fieldname);
        }
    }
    else if ( msg_id == SNS_SAM_GRAVITY_GET_REPORT_REQ_V01) {
        sns_sam_gravity_get_report_req_msg_v01 *payload = body;
        if ( strcmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
            payload->instance_id = (uint8_t)t_keyvalue->data.i;
        }
        else {
            InvalidFieldName( fieldname);
        }
    }
    else  if ( msg_id == SNS_SAM_GRAVITY_BATCH_REQ_V01) {
        sns_sam_gravity_batch_req_msg_v01 *payload = body;
        if ( strcmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
            payload->instance_id = (uint8_t)t_keyvalue->data.i;
        }
        else if ( strcasecmp( fieldname, get_literal( BATCH_PERIOD)) == 0) {
            if ( t_keyvalue->datatype == INTEGER) {
                uint32_t value = (int)( t_keyvalue->data.i * 65536);                     // convert to Q16
                payload->batch_period = value;
            }
            else if ( t_keyvalue->datatype == FLOAT) {
                uint32_t value = (int)( t_keyvalue->data.d * 65536);                     // convert to Q16
                payload->batch_period = value;
            }
            else {
                ErrReasonForField( BAD_VALUE, fieldname);
            }
        }
        else {
            InvalidFieldName( fieldname);
        }
    }
    else  if ( msg_id == SNS_SAM_GRAV_UPDATE_BATCH_PERIOD_REQ_V01) {
        sns_sam_grav_update_batch_period_req_msg_v01 *payload = body;
        if ( strcmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
            payload->instance_id = (uint8_t)t_keyvalue->data.i;
        }
        else if ( strcasecmp( fieldname, get_literal( BATCH_PERIOD)) == 0) {
            if ( t_keyvalue->datatype == INTEGER) {
                uint32_t value = (int)( t_keyvalue->data.i * 65536);                     // convert to Q16
                payload->active_batch_period = value;
            }
            else if ( t_keyvalue->datatype == FLOAT) {
                uint32_t value = (int)( t_keyvalue->data.d * 65536);                     // convert to Q16
                payload->active_batch_period = value;
            }
            else {
                ErrReasonForField( BAD_VALUE, fieldname);
            }
        }
        else {
            InvalidFieldName( fieldname);
        }
    }
    else {
        // TODO ErrReasonForField( BAD_MESSAGE, fieldname);
    }
}

// ServiceNewReq really GENERIC...
/**
 * @name GravNewReq
 * @brief set fields in the req_msg to field/values supplied by the fieldlist
 * @param[io] req_msg
 * @param[i] fieldlist
 */
void GravNewReq( request *req_msg, const array *fieldlist)
{
    if ( fieldlist) {
        int i;
        for ( i = 0; i <= fieldlist->high_water; ) {
            token *t_keyvalue  = ( token *)get_array_element( fieldlist, i++);
            if ( t_keyvalue == NULL) {
                break;
            }
            if ( t_keyvalue->subtype != KEYVALUE) {
                RTErrorMsg( "invalid key => value argument");
                break;
            }
            GravSetReq( req_msg, t_keyvalue);
        }
    }
}

void GravGetEnableResp( const respind *p_respind, const token *t_field, token *t_value)
{
    CommonGetOptT3InstanceId( p_respind, t_field, t_value);
}

void GravGetDisableResp( const respind *p_respind, const token *t_field, token *t_value)
{
    CommonGetOptT3InstanceId( p_respind, t_field, t_value);
}

/**
 * @name  GravGetResult
 * @brief   get value of fieldname from the
 *          sns_sam_gravity_result_s_v01 structure
 * @param[i] p_result
 * @param[i] fieldname
 * @param[io] t_value   - gets field value
 * @returns 0 == successful, <> 0 == invalid field name
 */
int GravGetResult( const sns_sam_gravity_result_s_v01 *p_result, const char *fieldname, token *t_value)
{
    int rc = 0;     // successful
    t_value->datatype = FLOAT;

    if ( strcasecmp( fieldname, get_literal( GRAVX)) == 0) {
        t_value->data.d = p_result->gravity[0];
    }
    else if ( strcasecmp( fieldname, get_literal( GRAVY)) == 0) {
        t_value->data.d = p_result->gravity[1];
    }
    else if ( strcasecmp( fieldname, get_literal( GRAVZ)) == 0) {
        t_value->data.d = p_result->gravity[2];
    }
    else if ( strcasecmp( fieldname, get_literal( LINACCX)) == 0) {
        t_value->data.d = p_result->lin_accel[0];
    }
    else if ( strcasecmp( fieldname, get_literal( LINACCY)) == 0) {
        t_value->data.d = p_result->lin_accel[1];
    }
    else if ( strcasecmp( fieldname, get_literal( LINACCZ)) == 0) {
        t_value->data.d = p_result->lin_accel[2];
    }
    else if ( strcasecmp( fieldname, get_literal( ACCURACY)) == 0) {
        t_value->data.i = p_result->accuracy;
        t_value->datatype = INTEGER;
    }
    else {
        t_value->datatype = UNDEFINED;
        rc                = 1;       // invalid field name
    }

    return rc;
}

/**
 * @name  GravGetReportResp
 * @brief   get value of fieldname from the p_respind message
 * @param[i] p_respind
 * @param[i] t_field - KEYVALUE or fieldname SYMBOL
 * @param[io] t_value   - gets field value
 */
void GravGetReportResp( const respind *p_respind, const token *t_field, token *t_value)
{
    sns_sam_gravity_get_report_resp_msg_v01 *pMsg =
        ( sns_sam_gravity_get_report_resp_msg_v01 *) p_respind->msg_ptr;


    char *fieldname = t_field->pSymbol;

    if ( strcasecmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
        if ( pMsg->instance_id_valid) {
            t_value->data.i = pMsg->instance_id;
            t_value->datatype = INTEGER;
        }
    }
    else if ( strcasecmp( fieldname, get_literal( SSCTS)) == 0) {
        if ( pMsg->timestamp_valid) {
            t_value->data.i = pMsg->timestamp;
            t_value->datatype = INTEGER;
        }
    }
    else if ( strcasecmp( fieldname, get_literal( MOTION_STATE)) == 0) {
        if ( pMsg->device_motion_state_valid) {
            t_value->data.i = pMsg->device_motion_state;
            t_value->datatype = INTEGER;
        }
    }
    else {
        sns_sam_gravity_result_s_v01 *p_result = &pMsg->result;
        if ( GravGetResult( p_result, fieldname, t_value)) {
            BadFieldName( fieldname);
        }
    }
}

/**
 * @name  GravGetBatchResp
 * @brief   get value of fieldname from the p_respind message
 * @param[i] p_respind
 * @param[i] t_field - KEYVALUE or fieldname SYMBOL
 * @param[io] t_value   - gets field value
 */
void GravGetBatchResp( const respind *p_respind, const token *t_field, token *t_value)
{
    sns_sam_gravity_batch_resp_msg_v01 *pMsg =
        ( sns_sam_gravity_batch_resp_msg_v01 *) p_respind->msg_ptr;

    char *fieldname = t_field->pSymbol;

    if ( strcasecmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
        if ( pMsg->instance_id_valid) {
            t_value->data.i = pMsg->instance_id;
            t_value->datatype = INTEGER;
        }
    }
    if ( strcasecmp( fieldname, get_literal( MAX_BATCH_SIZE)) == 0) {
        if ( pMsg->max_batch_size_valid) {
            t_value->data.i = pMsg->max_batch_size;
            t_value->datatype = INTEGER;
        }
    }
    else if ( strcasecmp( fieldname, get_literal( SSCTS)) == 0) {
        if ( pMsg->timestamp_valid) {
            t_value->data.i = pMsg->timestamp;
            t_value->datatype = INTEGER;
        }
    }
    else {
        BadFieldName( fieldname);
    }
}

/**
 * @name  GravGetUpdateBatchPeriodResp
 * @brief   get value of fieldname from the p_respind message
 * @param[i] p_respind
 * @param[i] t_field - KEYVALUE or fieldname SYMBOL
 * @param[io] t_value   - gets field value
 */
void GravGetUpdateBatchPeriodResp( const respind *p_respind, const token *t_field, token *t_value)
{
    sns_sam_grav_update_batch_period_resp_msg_v01 *pMsg =
        ( sns_sam_grav_update_batch_period_resp_msg_v01 *) p_respind->msg_ptr;

    char *fieldname = t_field->pSymbol;

    if ( strcasecmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
        if ( pMsg->instance_id_valid) {
            t_value->data.i = pMsg->instance_id;
            t_value->datatype = INTEGER;
        }
    }
    else if ( strcasecmp( fieldname, get_literal( SSCTS)) == 0) {
        if ( pMsg->timestamp_valid) {
            t_value->data.i = pMsg->timestamp;
            t_value->datatype = INTEGER;
        }
    }
    else {
        BadFieldName( fieldname);
    }
}

/**
 * @name  GravGetReportInd
 * @brief   get value of fieldname from the p_respind message
 * @param[i] p_respind
 * @param[i] t_field - KEYVALUE or fieldname SYMBOL
 * @param[io] t_value   - gets field value
 */
void GravGetReportInd( const respind *p_respind, const token *t_field, token *t_value)
{
    sns_sam_gravity_report_ind_msg_v01 *pMsg =
        ( sns_sam_gravity_report_ind_msg_v01 *) p_respind->msg_ptr;

    char *fieldname = t_field->pSymbol;

    if ( strcasecmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
        t_value->data.i = pMsg->instance_id;
        t_value->datatype = INTEGER;
    }
    else if ( strcasecmp( fieldname, get_literal( SSCTS)) == 0) {
        t_value->data.i = pMsg->timestamp;
        t_value->datatype = INTEGER;
    }
    else if ( strcasecmp( fieldname, get_literal( MOTION_STATE)) == 0) {
        if ( pMsg->device_motion_state_valid) {
            t_value->data.i = pMsg->device_motion_state;
            t_value->datatype = INTEGER;
        }
    }
    else {
        sns_sam_gravity_result_s_v01 *p_result = &pMsg->result;
        if ( GravGetResult( p_result, fieldname, t_value)) {
            BadFieldName( fieldname);
        }
    }
}

/**
 * @name  GravGetBatchInd
 * @brief   get value of fieldname from the p_respind message
 * @param[i] p_respind
 * @param[i] t_field - KEYVALUE or fieldname SYMBOL
 * @param[io] t_value   - gets field value
 */
void GravGetBatchInd( const respind *p_respind, const token *t_field, token *t_value)
{
    static uint32_t report_index = 0;

    sns_sam_gravity_batch_ind_msg_v01 *pMsg =
        ( sns_sam_gravity_batch_ind_msg_v01 *) p_respind->msg_ptr;

    char *fieldname = t_field->pSymbol;

    if ( t_field->type == KEYVALUE) {
        if ( strcasecmp( fieldname, get_literal( REPORT)) == 0) {
            report_index = t_field->data.i;
            if ( report_index > pMsg->reports_len) {
                RTErrorMsg( get_literal( BAD_INDEX));
                report_index = 0;
            }
        }
        else {
            BadFieldName( fieldname);
        }
        return;
    }

    if ( strcasecmp( fieldname, get_literal( INSTANCE_ID)) == 0) {
        t_value->data.i = pMsg->instance_id;
        t_value->datatype = INTEGER;
    }
    else if ( strcasecmp( fieldname, get_literal( IND_TYPE)) == 0) {
        if ( pMsg->ind_type_valid) {
            t_value->data.i = pMsg->ind_type;
            t_value->datatype = INTEGER;
        }
    }
    else if ( strcasecmp( fieldname, get_literal( REPORT)) == 0) {
        if ( pMsg->timestamp_offsets_len == pMsg->reports_len) {
            t_value->data.i = pMsg->reports_len;            // return number of reports
            t_value->datatype = INTEGER;
        }
        else {
            RTErrorMsg( get_literal( MALFORMED_PACKET));
        }
    }
    else if ( pMsg->timestamp_offsets_len && pMsg->reports_len ) {
        if ( strcasecmp( fieldname, get_literal( SSCTS)) == 0) {
            uint32_t ssc_ts = pMsg->first_report_timestamp;
            uint32_t i;
            for ( i = 0; i <= report_index; i++) {
                ssc_ts += pMsg->timestamp_offsets[ i];  //  compute ssc_ts for this report
            }
            t_value->data.i = ssc_ts;
            t_value->datatype = INTEGER;
        }
        else {
            sns_sam_gravity_report_s_v01 *p_report = &pMsg->reports[ report_index];
            if ( strcasecmp( fieldname, get_literal( MOTION_STATE)) == 0) {
                t_value->data.i = p_report->device_motion_state;
                t_value->datatype = INTEGER;
            }
            else {
                sns_sam_gravity_result_s_v01 *p_result = &p_report->result;
                if ( GravGetResult( p_result, fieldname, t_value)) {
                    BadFieldName( fieldname);
                }
            }
        }
    }
}

/*
 * @name GravGetter
 * @brief       get field values from the p_respind message
 *
 * @param[i] p_respind
 * @param[i] t_field - KEYVALUE or fieldname SYMBOL
 * @param[io] element
 */
void GravGetter( const respind *p_respind, const token *t_field, token *t_value)
{

    SetTypes( t_value, VALUE, SCALAR, UNDEFINED);

    uint32_t service_number = p_respind->header.service_number;
    if ( !( service_number == SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01)) {
        FatalError( get_literal( BAD_SRVC_NUM));
    }

    uint32_t msg_id = p_respind->header.msg_id;
    uint16_t msg_size = p_respind->header.msg_size;
    sensor1_msg_type_e msg_type = p_respind->msg_type;

    if ( msg_type == SENSOR1_MSG_TYPE_RESP) {
        if ( msg_id == SNS_SAM_GRAVITY_ENABLE_RESP_V01) {
            GravGetEnableResp( p_respind, t_field, t_value);
        }
        else if ( msg_id == SNS_SAM_GRAVITY_DISABLE_RESP_V01) {
            GravGetDisableResp( p_respind, t_field, t_value);
        }
        else if ( msg_id == SNS_SAM_GRAVITY_GET_REPORT_RESP_V01) {
            GravGetReportResp( p_respind, t_field, t_value);
        }
        else if ( msg_id == SNS_SAM_GRAVITY_BATCH_RESP_V01) {
            GravGetBatchResp( p_respind, t_field, t_value);
        }
        else if ( msg_id == SNS_SAM_GRAV_UPDATE_BATCH_PERIOD_RESP_V01) {
            GravGetUpdateBatchPeriodResp( p_respind, t_field, t_value);
        }
        else if ( msg_id == SNS_SAM_GRAV_GET_ATTRIBUTES_RESP_V01) {     // GENERIC
            CommonGetAttributesResp( p_respind, t_field, t_value);
        }
        else {
            RTErrorMsg( get_literal( BAD_MSG_ID));
        }
    }
    else if ( msg_type == SENSOR1_MSG_TYPE_IND) {
        if ( msg_id == SNS_SAM_GRAVITY_REPORT_IND_V01) {
            GravGetReportInd( p_respind, t_field, t_value);
        }
        else if ( msg_id == SNS_SAM_GRAVITY_BATCH_IND_V01) {
            GravGetBatchInd( p_respind, t_field, t_value);
        }
        else {
            RTErrorMsg( get_literal( BAD_MSG_ID));
        }
    }
}

/**
 * @name GravInit
 */

void GravInit( void)
{
    int svc_id = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].name = "amd";
    service_toc[ svc_id].new_req_func = (new_req)&GravNewReq;
    service_toc[ svc_id].set_req_func = (set_req)&GravSetReq;
    service_toc[ svc_id].getter_func  = (get_func)&GravGetter;

    service_toc[ svc_id].req_msg[ SAM_CANCEL].header.service_number = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].req_msg[ SAM_CANCEL].header.msg_id = SNS_SAM_GRAVITY_CANCEL_REQ_V01;
    service_toc[ svc_id].req_msg[ SAM_CANCEL].header.msg_size = 0;

    service_toc[ svc_id].req_msg[ SAM_VERSION].header.service_number = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].req_msg[ SAM_VERSION].header.msg_id = SNS_SAM_GRAVITY_VERSION_REQ_V01;
    service_toc[ svc_id].req_msg[ SAM_VERSION].header.msg_size = 0;

    service_toc[ svc_id].req_msg[ SAM_ENABLE].header.service_number = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].req_msg[ SAM_ENABLE].header.msg_id = SNS_SAM_GRAVITY_ENABLE_REQ_V01;
    service_toc[ svc_id].req_msg[ SAM_ENABLE].header.msg_size = sizeof(sns_sam_gravity_enable_req_msg_v01);

    service_toc[ svc_id].req_msg[ SAM_DISABLE].header.service_number = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].req_msg[ SAM_DISABLE].header.msg_id = SNS_SAM_GRAVITY_DISABLE_REQ_V01;
    service_toc[ svc_id].req_msg[ SAM_DISABLE].header.msg_size = sizeof(sns_sam_gravity_disable_req_msg_v01);

    service_toc[ svc_id].req_msg[ SAM_GET_REPORT].header.service_number = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].req_msg[ SAM_GET_REPORT].header.msg_id = SNS_SAM_GRAVITY_GET_REPORT_REQ_V01;
    service_toc[ svc_id].req_msg[ SAM_GET_REPORT].header.msg_size = sizeof(sns_sam_gravity_get_report_req_msg_v01);

    service_toc[ svc_id].req_msg[ SAM_BATCH].header.service_number = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].req_msg[ SAM_BATCH].header.msg_id = SNS_SAM_GRAVITY_BATCH_REQ_V01;
    service_toc[ svc_id].req_msg[ SAM_BATCH].header.msg_size = sizeof(sns_sam_gravity_batch_req_msg_v01);

    service_toc[ svc_id].req_msg[ SAM_FLUSH].header.service_number = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].req_msg[ SAM_FLUSH].header.msg_id = SNS_SAM_GRAV_UPDATE_BATCH_PERIOD_REQ_V01;
    service_toc[ svc_id].req_msg[ SAM_FLUSH].header.msg_size = sizeof(sns_sam_grav_update_batch_period_req_msg_v01);

    service_toc[ svc_id].req_msg[ SAM_ALGO_ATTRIB].header.service_number = SNS_SAM_GRAVITY_VECTOR_SVC_ID_V01;
    service_toc[ svc_id].req_msg[ SAM_ALGO_ATTRIB].header.msg_id = SNS_SAM_GRAV_GET_ATTRIBUTES_REQ_V01;
    service_toc[ svc_id].req_msg[ SAM_ALGO_ATTRIB].header.msg_size = 0;
}


