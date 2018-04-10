/*===========================================================================
Copyright (c) 2015 Qualcomm Technologies, Inc.
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

#include "thermal.h"
#include "thermal_config.h"

static char *vs_list_8996[] = {
	"tsens_tz_sensor4",
	"tsens_tz_sensor6",
	"tsens_tz_sensor9",
	"tsens_tz_sensor11"
};

static struct setting_info vs_cfgs_8996[] =
{
	{
		.desc = "VIRTUAL-CPUS",
		.algo_type = VIRTUAL_SENSOR_TYPE,
		.dynamic = 0,
		.data.v = {
			.trip_sensor = "tsens_tz_sensor11",
			.sensor_list = vs_list_8996,
			.list_cnt = ARRAY_SIZE(vs_list_8996),
			.list_size = 1,
			.set_point = 75000,
			.set_point_clr = 65000,
			.sampling_period_ms = 10,
			.math_type = MATH_MAX,
		},
	},
};

static char *vs_list_cobalt_cluster0[] = {
	"tsens_tz_sensor1",
	"tsens_tz_sensor2",
	"tsens_tz_sensor3",
	"tsens_tz_sensor4"
};

static char *vs_list_cobalt_cluster1[] = {
	"tsens_tz_sensor7",
	"tsens_tz_sensor8",
	"tsens_tz_sensor9",
	"tsens_tz_sensor10"
};

static struct setting_info vs_cfgs_cobalt[] =
{
	{
		.desc = "VIRTUAL-CLUSTER0",
		.algo_type = VIRTUAL_SENSOR_TYPE,
		.dynamic = 0,
		.data.v = {
			.trip_sensor = "tsens_tz_sensor1",
			.sensor_list = vs_list_cobalt_cluster0,
			.list_cnt = ARRAY_SIZE(vs_list_cobalt_cluster0),
			.list_size = 1,
			.set_point = 60000,
			.set_point_clr = 50000,
			.sampling_period_ms = 10,
			.math_type = MATH_MAX,
		},
	},
	{
		.desc = "VIRTUAL-CLUSTER1",
		.algo_type = VIRTUAL_SENSOR_TYPE,
		.dynamic = 0,
		.data.v = {
			.trip_sensor = "tsens_tz_sensor7",
			.sensor_list = vs_list_cobalt_cluster1,
			.list_cnt = ARRAY_SIZE(vs_list_cobalt_cluster1),
			.list_size = 1,
			.set_point = 60000,
			.set_point_clr = 50000,
			.sampling_period_ms = 10,
			.math_type = MATH_MAX,
		},
	},
};

static char *vs_list_8937[] = {
	"tsens_tz_sensor5",
	"tsens_tz_sensor6",
	"tsens_tz_sensor7",
	"tsens_tz_sensor8",
	"tsens_tz_sensor9"
};

static struct setting_info vs_cfgs_8937[] =
{
	{
		.desc = "VIRTUAL-CPUS",
		.algo_type = VIRTUAL_SENSOR_TYPE,
		.dynamic = 0,
		.data.v = {
			.trip_sensor = "tsens_tz_sensor5",
			.sensor_list = vs_list_8937,
			.list_cnt = ARRAY_SIZE(vs_list_8937),
			.list_size = 1,
			.set_point = 75000,
			.set_point_clr = 65000,
			.sampling_period_ms = 50,
			.math_type = MATH_MAX,
		},
	},
};

static char *vs_list_8953[] = {
	"tsens_tz_sensor9",
	"tsens_tz_sensor10",
	"tsens_tz_sensor11",
	"tsens_tz_sensor12",
	"tsens_tz_sensor13",
	"tsens_tz_sensor4",
	"tsens_tz_sensor5",
	"tsens_tz_sensor6",
	"tsens_tz_sensor7",
	"tsens_tz_sensor8"
};

static struct setting_info vs_cfgs_8953[] =
{
	{
		.desc = "VIRTUAL-CPUS",
		.algo_type = VIRTUAL_SENSOR_TYPE,
		.dynamic = 0,
		.data.v = {
			.trip_sensor = "tsens_tz_sensor9",
			.sensor_list = vs_list_8953,
			.list_cnt = ARRAY_SIZE(vs_list_8953),
			.list_size = 1,
			.set_point = 75000,
			.set_point_clr = 65000,
			.sampling_period_ms = 50,
			.math_type = MATH_MAX,
		},
	},
};

static char *vs_list_8917[] = {
	"tsens_tz_sensor5",
	"tsens_tz_sensor6",
	"tsens_tz_sensor7",
	"tsens_tz_sensor8"
};

static struct setting_info vs_cfgs_8917[] =
{
	{
		.desc = "VIRTUAL-CPUS",
		.algo_type = VIRTUAL_SENSOR_TYPE,
		.dynamic = 0,
		.data.v = {
			.trip_sensor = "tsens_tz_sensor5",
			.sensor_list = vs_list_8917,
			.list_cnt = ARRAY_SIZE(vs_list_8917),
			.list_size = 1,
			.set_point = 75000,
			.set_point_clr = 65000,
			.sampling_period_ms = 50,
			.math_type = MATH_MAX,
		},
	},
};

void virtual_sensors_init_data(struct thermal_setting_t *setting)
{
	int i = 0, arr_size = 0;
	struct setting_info *cfg = NULL;

	switch (therm_get_msm_id()) {
		case THERM_MSM_8996:
		case THERM_MSM_8996AU:
			cfg = vs_cfgs_8996;
			arr_size = ARRAY_SIZE(vs_cfgs_8996);
			break;
		case THERM_MSM_COBALT:
			cfg = vs_cfgs_cobalt;
			arr_size = ARRAY_SIZE(vs_cfgs_cobalt);
			break;
		case THERM_MSM_8937:
		case THERM_MSM_8940:
			cfg = vs_cfgs_8937;
			arr_size = ARRAY_SIZE(vs_cfgs_8937);
			break;
		case THERM_MSM_8953:
			cfg = vs_cfgs_8953;
			arr_size = ARRAY_SIZE(vs_cfgs_8953);
			break;
		case THERM_MSM_8917:
		case THERM_MSM_8920:
			cfg = vs_cfgs_8917;
			arr_size = ARRAY_SIZE(vs_cfgs_8917);
			break;
		default:
			dbgmsg("%s: No virtual sensors added for this platform\n",
				__func__);
			return;
	}

	for (i = 0; i < arr_size; i++)
		add_setting(setting, &cfg[i]);
}

