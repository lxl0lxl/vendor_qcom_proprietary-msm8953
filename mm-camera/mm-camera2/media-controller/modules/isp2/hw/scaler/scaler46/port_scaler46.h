/* port_scaler46.h
 *
 * Copyright (c) 2012-2014 Qualcomm Technologies, Inc. All Rights Reserved.
 * Qualcomm Technologies Proprietary and Confidential.
 */

#ifndef __PORT_SCALER46_H__
#define __PORT_SCALER46_H__

/* std headers */
#include "pthread.h"
#include "module_scaler46.h"

/** port_scaler46_data_t:
 *
 * @mutex: mutex lock
 * @is_reserved : is reserved flag
 * @session_id: current session id
 * @num_streams: number of streams using this port
 *  @int_peer_port: internal peer port
 **/
typedef struct {
  pthread_mutex_t mutex;
  boolean         is_reserved;
  uint32_t        session_id;
  uint32_t        num_streams;
  mct_port_t     *int_peer_port;
} port_scaler46_data_t;

boolean port_scaler46_create(mct_module_t *module);

void port_scaler46_delete_ports(mct_module_t *module);

boolean port_scaler46_handle_hal_set_parm(mct_module_t* module,
  scaler46_t *scaler, mct_event_control_parm_t *hal_param, uint32_t identity);

boolean port_scaler46_handle_cds_request(scaler46_t *scaler,
  mct_event_t *event);

#endif
