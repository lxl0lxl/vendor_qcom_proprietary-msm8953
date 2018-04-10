/********************************************************************
---------------------------------------------------------------------
Copyright (c) 2014 Qualcomm Technologies, Inc.
All Rights Reserved. Qualcomm Technologies Proprietary and Confidential.
----------------------------------------------------------------------
GPTEE (QTEEC) Sample/Test Client app.
*********************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/msm_ion.h>
#include <utils/Log.h>
#include "TEE_client_api.h"
#include "common_log.h"
#include <sys/mman.h>
#include <getopt.h>


/* adb log */
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "GPTEST: "
#ifdef LOG_NDDEBUG
#undef LOG_NDDEBUG
#endif
#define LOG_NDDEBUG 0 /* Define to enable LOGD */
#ifdef LOG_NDEBUG
#undef LOG_NDEBUG
#endif
#define LOG_NDEBUG  0 /* Define to enable LOGV */

#define __64KB__ 0x10000
#define __32KB__ 0x8000
#define __16KB__ 0x4000
#define __8KB__ 0x2000
#define __4KB__ 0x1000
#define __2KB__ 0x800
#define __1KB__ 0x400

/* Error code: status sent as response to command from sample client*/

#define SUCCESS 0
#define FAILED -1

#define GPTEST_APP_VERSION 0x1

/* UUID for gptest app */
TEEC_UUID destination =
{
        0xcad10542, 0x34e4, 0x452d,
        {0x61, 0x56, 0xe9, 0x79, 0xaa, 0x6e, 0x61, 0xbc}
};

/* UUID for gptest2 app */
TEEC_UUID destination_gptest2 =
{
        0x23914957, 0xC174, 0x4EA6,
        {0x54, 0xd1, 0xda, 0x82, 0x0f, 0x77, 0xb4, 0xb0}
};
TEEC_Result expected_return_result = SUCCESS;
/*********************************************************************
**  print_text() - print the text message
*********************************************************************/
static void print_text(char *intro_message, unsigned char *text_addr,
						unsigned int size)
{
	unsigned int   i;

	printf("%s @ address = 0x%x\n", intro_message, (unsigned int)text_addr);
	for (i = 0;  i < size;  i++) {
		printf("%2x ", text_addr[i]);
		if ((i & 0xf) == 0xf)
			printf("\n");
	}
	printf("\n");
}

/*----------------------------------------------------------------------------
 * Store the TEE application UUID
 * -------------------------------------------------------------------------*/
char *APPNAME = "gptest";

/*----------------------------------------------------------------------------
 * Definitions of the Trusted Application Command IDs
 * -------------------------------------------------------------------------*/
#define CMD_BASIC_MULT_DATA 1
#define CMD_BASIC_COPY_BACK 2
#define CMD_GP_SHARED_MEM_TESTS 300

uint32_t command = CMD_BASIC_MULT_DATA;
uint32_t value_a = 2;
/*----------------------------------------------------------------------------
 * TEST TEEC_SharedMemory test
 * -------------------------------------------------------------------------*/
 int QTEEC_TEST_SharedMemoryAccessTest(int iterations)
{
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Session session1;
	TEEC_Operation operation;
	TEEC_Operation operation1;
	TEEC_Result result = 0xFFFFFFFF;
	TEEC_SharedMemory commsSM;
	uint32_t returnOrigin = 0xFFFFFFFF;
	TEEC_UUID uuid;
	TEEC_UUID uuid1;
	TEEC_Context context1;
	TEEC_SharedMemory commsSM1;
	uint32_t retun_address = 0;
	command = CMD_GP_SHARED_MEM_TESTS;

	printf("START gp_SharedMemoryAccess_test\n");
	memset((char *)&uuid, 0, sizeof(TEEC_UUID));
	memcpy((char *)&uuid, (char *)&destination, sizeof(TEEC_UUID));
	memset((char *)&uuid1, 0, sizeof(TEEC_UUID));
	memcpy((char *)&uuid1, (char *)&destination_gptest2, sizeof(TEEC_UUID));

	/* First Context Initialize */
	/* [1] Connect to TEE */
	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS)
		goto cleanup1;

	/* [2] Open session with TEE application */
	/* Open a Session with the TEE application. */
	/* No connection data needed for TEEC_LOGIN_USER. */
	/* No payload, and do not want cancellation. */
	result = TEEC_OpenSession(&context, &session,&uuid,TEEC_LOGIN_USER,NULL, NULL,&returnOrigin);
	if (result != TEEC_SUCCESS)
		goto cleanup2;
	/* [3] Initialize the Shared Memory buffers */
	/* [a] Communications buffer. */
	commsSM.size = __1KB__;
	commsSM.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	/* Use TEE Client API to allocate the underlying memory buffer. */
	result = TEEC_AllocateSharedMemory(&context, &commsSM);
	if (result != TEEC_SUCCESS)
		goto cleanup3;
	/* Initialize buffer to all 1's as data*/
	memset(commsSM.buffer, 0x1, commsSM.size);
	/* [4] Issue commands to operate on allocated and registered buffers  */
	/* [a] Set the parameter types */
	operation.paramTypes = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,TEEC_MEMREF_PARTIAL_INOUT,TEEC_NONE, TEEC_NONE);
	operation.params[0].value.a = value_a;
	operation.params[1].memref.parent = &commsSM;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = __1KB__;

	print_text("Initial data buffer commsSM", commsSM.buffer, 15);
	/* Return result of function:TEEC_invokeCommand */
	/* is the address (value) received from the TA. */
	result = TEEC_InvokeCommand(&session, command,&operation,&returnOrigin);
	retun_address = result;
	printf("\n address return from GPTEST App where data has written 0x%x\n",retun_address);
	print_text("Modified  data buffer by GPTEST App\n", commsSM.buffer, 15);

	/* Second contex initialize */
	/* [1] Connect to TEE */
	result = TEEC_InitializeContext(NULL, &context1);
	if (result != TEEC_SUCCESS)
		goto cleanup4;
	/* [2] Open session with TEE application */
	/* Open a Session with the TEE application. */
	/* No connection data needed for TEEC_LOGIN_USER. */
	/* No payload, and do not want cancellation. */
	result = TEEC_OpenSession(&context1, &session1,&uuid1,TEEC_LOGIN_USER,NULL, NULL,&returnOrigin);
	if (result != TEEC_SUCCESS)
		goto cleanup5;
	commsSM1.size = __1KB__;
	commsSM1.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	result = TEEC_AllocateSharedMemory(&context1, &commsSM1);
	if (result != TEEC_SUCCESS)
	        goto cleanup6;
	/* Initialize buffer to all 1's as data*/
	memset(commsSM1.buffer, 0x1, commsSM1.size);
	operation1.paramTypes = 0;
	operation1.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,TEEC_MEMREF_PARTIAL_INOUT,TEEC_NONE, TEEC_NONE);
	operation1.params[0].value.a = retun_address;
	operation1.params[1].memref.parent = &commsSM1;
	operation1.params[1].memref.offset = 0;
	operation1.params[1].memref.size = __1KB__;

	result = TEEC_InvokeCommand(&session1, command, &operation1,&returnOrigin);
	if (result==TEEC_SUCCESS)
		printf("\nGP_sharedbuffer test Failed\n");
	else
	{
	  printf("GP_app cannot able to access shared memory data of other GP_app\n");
	  printf("\nGP_sharedbuffer test Passed\n");
	}
	if (result != TEEC_SUCCESS)
	    goto cleanup7;

	/* [5] Tidy up resources */
	cleanup7:
		TEEC_ReleaseSharedMemory(&commsSM1);
	cleanup6:
		TEEC_CloseSession(&session1);
	cleanup5:
	        TEEC_FinalizeContext(&context1);
	cleanup4:
		TEEC_ReleaseSharedMemory(&commsSM);
	cleanup3:
		TEEC_CloseSession(&session);
	cleanup2:
		TEEC_FinalizeContext(&context);
	cleanup1:
		printf("result 0x%x, origin = 0x%x\n",
						result, returnOrigin);

	return result;
}
/*----------------------------------------------------------------------------
 * TEST TEEC_InitializeContext/TEEC_FinalizeContext APIS
 * -------------------------------------------------------------------------*/
int QTEEC_TEST_init_deinit_context(int iterations)
{
	/* Allocate TEE Client structures on the stack. */
	TEEC_Context context;
	TEEC_Result result = 0xFFFFFFFF;
	int i = 0;

	printf("	START QTEEC_TEST_init_deinit_context:  \n");
	for (i = 0; i < iterations; i++) {
		/* [1] Connect to TEE  */
		result = TEEC_InitializeContext(NULL, &context);
		if (result != TEEC_SUCCESS)
			goto cleanup1;
		/* [2] Close the Session with the TEE application. */
		TEEC_FinalizeContext(&context);
	}
	/* [3] Tidy up resources */
cleanup1:
	if (result == SUCCESS)
		printf("	PASSED: result 0x%x,\n", result);
	else
		printf("	FAILED: iteration = %d result 0x%x,\n",
							i, result);
	return result;
}


/*----------------------------------------------------------------------------
 * TEST TEEC_OpenSession/TEEC_CloseSession APIS
 * -------------------------------------------------------------------------*/
int QTEEC_TEST_open_close_session(int iterations)
{
	/* Allocate TEE Client structures on the stack. */
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Result result = 0xFFFFFFFF;
	uint32_t returnOrigin = 0xFFFFFFFF;
	int i = 0;
	uint32_t ConnectionData = 0;
	TEEC_UUID uuid;

	printf("	START QTEEC_TEST_open_close_session: \n");
	memset((char *)&uuid, 0, sizeof(TEEC_UUID));
	memcpy((char *)&uuid, (char *)&destination, sizeof(TEEC_UUID));

	/* [1] Connect to TEE */
	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS)
		goto cleanup1;
	for (i = 0; i < iterations; i++) {
		/* [2] Open a Session with the TEE application. */
		/* No connection data needed for TEEC_LOGIN_USER. */
		/* No payload, and do not want cancellation. */
		result = TEEC_OpenSession(&context, &session, &uuid,
						TEEC_LOGIN_USER, (void *)&ConnectionData,
						NULL, &returnOrigin);
		if (result != TEEC_SUCCESS)
			goto cleanup2;

		/* [3] Close the Session with the TEE application. */
		TEEC_CloseSession(&session);
	}
	/* [4] Tidy up resources */
cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	if (result == SUCCESS)
		printf("PASSED:iteration = %d  result 0x%x, origin = 0x%x\n",
			i, result, returnOrigin);
	else
		printf("FAILED: iteration = %d result 0x%x, origin = 0x%x\n",
			i, result, returnOrigin);
	return result;
}


/*----------------------------------------------------------------------------
 * TEST Multiple TEEC_OpenSession/TEEC_CloseSession APIS
 * -------------------------------------------------------------------------*/
int QTEEC_TEST_multiple_session(int iterations)
{
	/* Allocate TEE Client structures on the stack. */
	TEEC_Context context;
	TEEC_Session session_array[10];
	TEEC_Result result = 0xFFFFFFFF;
	uint32_t returnOrigin = 0xFFFFFFFF;
	int i = 0, j = 0;
	TEEC_UUID uuid;

	printf("	START QTEEC_TEST_multiple_session: \n");
	memset((char *)&uuid,  0,  sizeof(TEEC_UUID));
	memcpy((char *)&uuid, (char *)&destination, sizeof(TEEC_UUID));
	/* [1] Connect to TEE  */
	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS)
		goto cleanup1;
	for (i = 0; i < 2; i++) {
		/* [2] Open a Session with the TEE application. */
		/* No connection data needed for TEEC_LOGIN_USER. */
		/* No payload, and do not want cancellation. */
		for (j = 0; j < 3; j++) {
			result = TEEC_OpenSession(&context, &session_array[j],
							&uuid,
							TEEC_LOGIN_USER,
							NULL, NULL,
							&returnOrigin);

			if (result != TEEC_SUCCESS)
				goto cleanup2;
		}
		for (j = 0; j < 3; j++) {
			/* [3] Close the Session with the TEE application. */
			TEEC_CloseSession(&session_array[j]);
		}
	}
	/* [4] Tidy up resources */
cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	if (result == SUCCESS)
		printf("	PASSED: result 0x%x, origin = 0x%x\n",
						result, returnOrigin);
	else
		printf("	FAILED: iter %d sess %d res 0x%x, orig=0x%x\n",
						i, j, result, returnOrigin);
	return result;
}

/*----------------------------------------------------------------------------
 * TEST TEEC_AllocateSharedMemory API
 * -------------------------------------------------------------------------*/
TEEC_Result QTEEC_TEST_TEEC_AllocateSharedMemory(int cancel_request)
{
	/* Allocate TEE Client structures on the stack. */
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_Result result = 0xFFFFFFFF;
	TEEC_SharedMemory commsSM;
	uint32_t returnOrigin = 0xFFFFFFFF;
	TEEC_UUID uuid;
	uint32_t inputSize = __64KB__ * 16;
	uint32_t outputSize = __64KB__ * 16;;

	printf("	START QTEEC_TEST_basic_tests_mult\n");
	memset((char *)&uuid, 0, sizeof(TEEC_UUID));
	memcpy((char *)&uuid, (char *)&destination, sizeof(TEEC_UUID));
	/* [1] Connect to TEE */
	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS)
		goto cleanup1;

	/* [2] Open session with TEE application */
	/* Open a Session with the TEE application. */
	/* No connection data needed for TEEC_LOGIN_USER. */
	/* No payload, and do not want cancellation. */
	result = TEEC_OpenSession(&context, &session, &uuid,
					TEEC_LOGIN_USER, NULL, NULL,
					&returnOrigin);
	if (result != TEEC_SUCCESS)
		goto cleanup2;

	/* [3] Initialize the Shared Memory buffers */
	/* [a] Communications buffer. */
	commsSM.size = 4096;
	commsSM.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	/* Use TEE Client API to allocate the underlying memory buffer. */
	result = TEEC_AllocateSharedMemory(&context, &commsSM);
	if (result != TEEC_SUCCESS)
		goto cleanup3;
	/* Initialize buffer to all 1's */
	memset(commsSM.buffer, 0x1, commsSM.size);

	/* [4] Issue commands to operate on allocated and registered buffers  */
	/* [a] Set the parameter types */
	operation.paramTypes = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						TEEC_MEMREF_PARTIAL_INOUT,
						TEEC_NONE, TEEC_NONE);
	/* [b] Set the multiplier value input param[0]
	and buffer param[1] to operate on */
	operation.params[0].value.a = value_a;
	operation.params[1].memref.parent = &commsSM;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = 4096;

	print_text("Initial data buffer", commsSM.buffer, 128);

	/* [c] Issue command to multiply commsSM by
	value set  in first parameter. */
	result = TEEC_InvokeCommand(&session, command,
					&operation,
					&returnOrigin);
	if (result != TEEC_SUCCESS)
		goto cleanup4;

	print_text("Modified data buffer", commsSM.buffer, 128);

	if (cancel_request)
		TEEC_RequestCancellation(&operation);

	/* [5] Tidy up resources */
cleanup4:
	TEEC_ReleaseSharedMemory(&commsSM);
cleanup3:
	TEEC_CloseSession(&session);
cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	if (result == expected_return_result)
		printf("	PASSED: result 0x%x, origin = 0x%x\n",
						result, returnOrigin);
	else
		printf("	FAILED: result 0x%x, origin = 0x%x\n",
						result, returnOrigin);
	return result;
}

/*----------------------------------------------------------------------------
 * TEST TEEC_RegisterSharedMemory API
 * -------------------------------------------------------------------------*/
TEEC_Result QTEEC_TEST_TEEC_RegisterSharedMemory(int cancel_request)
{
	/* Allocate TEE Client structures on the stack. */
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_Result result = 0xFFFFFFFF;
	TEEC_SharedMemory inputSM;
	TEEC_SharedMemory outputSM;
	uint32_t returnOrigin = 0xFFFFFFFF;
	TEEC_UUID uuid;
	uint8_t const * inputBuffer;
	uint32_t inputSize = __64KB__ * 16;
	uint8_t* outputBuffer;
	uint32_t outputSize = __64KB__ * 16;;

	outputBuffer = malloc(outputSize);
	if (outputBuffer == NULL) {
		printf("Error! Out of memroy\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}
	inputBuffer = malloc(inputSize);
	if (inputBuffer == NULL) {
		printf("Error! Out of memroy\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}
	memset(inputBuffer, 0xA, inputSize);
	memset(outputBuffer, 0xD, outputSize);

	printf("	START QTEEC_TEST_basic_tests_copy\n");
	memset((char *)&uuid, 0, sizeof(TEEC_UUID));
	memcpy((char *)&uuid, (char *)&destination, sizeof(TEEC_UUID));
	/* [1] Connect to TEE  */
	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS)
		goto cleanup1;

	/* [2] Open session with TEE application */
	/* Open a Session with the TEE application. */
	/* No connection data needed for TEEC_LOGIN_USER. */
	/* No payload, and do not want cancellation. */
	result = TEEC_OpenSession(&context, &session, &uuid,
					TEEC_LOGIN_USER, NULL,
					NULL, &returnOrigin);
	if (result != TEEC_SUCCESS)
		goto cleanup2;

	/* [3] Initialize the Shared Memory buffers */
	/* Issue commands to operate on allocated and registered buffers  */
	/* [a] Bulk input buffer. */
	inputSM.size = inputSize;
	inputSM.flags = TEEC_MEM_INPUT;
	inputSM.buffer = (uint8_t *)inputBuffer;

	/* Use TEE Client API to register the underlying memory buffer. */
	result = TEEC_RegisterSharedMemory(&context, &inputSM);
	if (result != TEEC_SUCCESS)
		goto cleanup4;

	/* [b] Bulk output buffer (also input for digest). */
	outputSM.size = outputSize;
	outputSM.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	outputSM.buffer = outputBuffer;

	/* Use TEE Client API to register the underlying memory buffer. */
	result = TEEC_RegisterSharedMemory(&context, &outputSM);
	if (result != TEEC_SUCCESS)
		goto cleanup5;

	/* [c] Initialize source  TEE_param opeartion. */
	operation.params[0].memref.parent = &inputSM;
	operation.params[0].memref.offset = 0;
	operation.params[0].memref.size = inputSize;

	/* [d] Initialize destination TEE_param opeartion. */
	operation.params[1].memref.parent = &outputSM;
	operation.params[1].memref.offset = 0;
	operation.params[1].memref.size = operation.params[0].memref.size;

	print_text("Initial src buffer", inputBuffer, 256);
	print_text("Initial dst buffer", outputBuffer, 256);

	/*[4]  Set parameter type. */
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT,
						TEEC_MEMREF_PARTIAL_OUTPUT,
						TEEC_NONE,
						TEEC_NONE);

	/*[5] Issue command to copy date from buffer in param[0]
	to buffer in param[1]. */
	result = TEEC_InvokeCommand(&session, CMD_BASIC_COPY_BACK,
					&operation, &returnOrigin);
	if (result != TEEC_SUCCESS)
		goto cleanup6;

	print_text("Final untouched src buffer", inputBuffer, 256);
	print_text("Modified dst buffer", outputBuffer, 256);

	if (cancel_request)
		TEEC_RequestCancellation(&operation);
	/* [6] Tidy up resources */
cleanup6:
	TEEC_ReleaseSharedMemory(&outputSM);
cleanup5:
	TEEC_ReleaseSharedMemory(&inputSM);
cleanup4:
cleanup3:
	TEEC_CloseSession(&session);
cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	/* Tidy up resources */
	free(outputBuffer);
	free(inputBuffer);
	if (result == expected_return_result)
		printf("	PASSED: result 0x%x, origin = 0x%x\n",
						result, returnOrigin);
	else
		printf("	FAILED: result 0x%x, origin = 0x%x\n",
						result, returnOrigin);
	return result;
}
/*----------------------------------------------------------------------------
 * TEST TEEC_RegisterSharedMemory API
 * -------------------------------------------------------------------------*/
TEEC_Result QTEEC_TEST_TEEC_TempMemory(int cancel_request)
{
	/* Allocate TEE Client structures on the stack. */
	TEEC_Context context;
	TEEC_Session session;
	TEEC_Operation operation;
	TEEC_Result result = 0xFFFFFFFF;
	TEEC_TempMemoryReference inputSM;
	TEEC_TempMemoryReference outputSM;
	uint32_t returnOrigin = 0xFFFFFFFF;
	TEEC_UUID uuid;
	uint8_t const * inputBuffer;
	uint32_t inputSize = __64KB__ * 16;
	uint8_t* outputBuffer;
	uint32_t outputSize = __64KB__ * 16;;

	outputBuffer = malloc(outputSize);
	if (outputBuffer == NULL) {
		printf("Error! Out of memroy\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}
	inputBuffer = malloc(inputSize);
	if (inputBuffer == NULL) {
		printf("Error! Out of memroy\n");
		return TEEC_ERROR_OUT_OF_MEMORY;
	}
	memset(inputBuffer, 0xA, inputSize);
	memset(outputBuffer, 0xD, outputSize);

	printf("	START QTEEC_TEST_basic_tests_copy\n");
	memset((char *)&uuid, 0, sizeof(TEEC_UUID));
	memcpy((char *)&uuid, (char *)&destination, sizeof(TEEC_UUID));
	/* [1] Connect to TEE  */
	result = TEEC_InitializeContext(NULL, &context);
	if (result != TEEC_SUCCESS)
		goto cleanup1;

	/* [2] Open session with TEE application */
	/* Open a Session with the TEE application. */
	/* No connection data needed for TEEC_LOGIN_USER. */
	/* No payload, and do not want cancellation. */
	result = TEEC_OpenSession(&context, &session, &uuid,
					TEEC_LOGIN_USER, NULL,
					NULL, &returnOrigin);
	if (result != TEEC_SUCCESS)
		goto cleanup2;

	/* [3] Initialize the Shared Memory buffers */
	/* Issue commands to operate on allocated and registered buffers  */
	/* [a] Bulk input buffer. */
	inputSM.size = inputSize;
	inputSM.buffer = (uint8_t *)inputBuffer;


	/* [b] Bulk output buffer (also input for digest). */
	outputSM.size = outputSize;
	outputSM.buffer = outputBuffer;

	/* [c] Initialize source  TEE_param opeartion. */
	operation.params[0].tmpref.buffer = inputSM.buffer;
	operation.params[0].tmpref.size = inputSM.size;

	/* [d] Initialize destination TEE_param opeartion. */
	operation.params[1].tmpref.buffer = outputSM.buffer;
	operation.params[1].tmpref.size = outputSM.size;

	print_text("Initial src buffer", inputSM.buffer, 256);
	print_text("Initial dst buffer", outputSM.buffer, 256);

	/*[4]  Set parameter type. */
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
						TEEC_MEMREF_TEMP_OUTPUT,
						TEEC_NONE,
						TEEC_NONE);

	/*[5] Issue command to copy date from buffer in param[0]
	to buffer in param[1]. */
	result = TEEC_InvokeCommand(&session, CMD_BASIC_COPY_BACK,
					&operation, &returnOrigin);
	if (result != TEEC_SUCCESS)
		goto cleanup3;

	print_text("Final untouched src buffer", inputSM.buffer, 256);
	print_text("Modified dst buffer", outputSM.buffer, 256);

	if (cancel_request)
		TEEC_RequestCancellation(&operation);

	/* [6] Tidy up resources */

cleanup3:
	TEEC_CloseSession(&session);
cleanup2:
	TEEC_FinalizeContext(&context);
cleanup1:
	/* Tidy up resources */
	free(outputBuffer);
	free(inputBuffer);
	if (result == expected_return_result)
		printf("	PASSED: result 0x%x, origin = 0x%x\n",
						result, returnOrigin);
	else
		printf("	FAILED: result 0x%x, origin = 0x%x\n",
						result, returnOrigin);
	return result;
}


static int verbose;
static int internal_test_enabled;

enum test_types {
	NOMINAL,
	ADVERSARIAL,
	REPEAT,
	STRESS,
	LAST_TEST,
	INTERNAL = LAST_TEST,
};

struct option testopts[] = {
	{"Nominal", no_argument, NULL, 'n'},
	{"Adversarial", no_argument, NULL, 'a'},
	{"Repeatability", no_argument, NULL, 'r'},
	{"Stress", no_argument, NULL, 's'},
	{"Verbose", no_argument, NULL, 'v'},
	{"Help", no_argument, NULL, 'h'},
	{"Internal", no_argument, NULL, 'i'},
	{"UUID", required_argument, NULL, 'u'},
	{"ExpRetRst", required_argument, NULL, 'e'},
	{NULL, 0, NULL, 0},
};

static int (*test_func[]) (void) = {
	[NOMINAL] = NULL,
	[ADVERSARIAL] = NULL,
	[REPEAT] = NULL,
	[STRESS] = NULL,
};

void internal_test_menu(void)
{
	printf("\n\n");
	printf("The currently implemented tests are:\n");
	printf("\n");
	printf("    Input 1:	TEST CASE NUMBER\n");
	printf("   ------------------------------\n");
	printf("    0 ------->  Dispaly Menu\n");
	printf("    1 ------->  Test Initialize/deinitialize context (interations)\n");
	printf("    2 ------->  Test Open/Close Session (iteractions)\n");
	printf("    3 ------->  Test open multiple sessions/close all\n");
	printf("    4 ------->  Test Invoke comamnd with AllocateSharedMemory API\n");
	printf("    5 ------->  Test Invoke comamnd with RegisterSharedMemory API\n");
	printf("    6 ------->  Test Invoke comamnd with TempMemory Type API\n");
	printf("    7 ------->  Test request cancellation with AllocateSharedMemory Type API\n");
	printf("    8 ------->  Test request cancellation with RegisterSharedMemory Type API\n");
	printf("    9 ------->  Test request cancellation with TempMemory Type API\n");
	printf("    10 ------>  Shared buffer: trying to share buffer for two app in GP Test framework\n");
	printf("    100------>  Generic Test: Input cmd & value to send to TA (trusted APP)\n\n");

	printf("   -------------------------------------------------------------------------------------------------\n");
	printf("    Input 1 Enter test case number ((test cases 1 - 10) to run\n");
	printf("    Input 2 (Applicable to test cases 1 - 10) iteration? --> enter number of iterations\n");
	printf("    Input 3 (Applicable to test case 100 only) command ID? --> enter command ID to send to TA\n");
	printf("    Input 4 (Applicable to test case 100 only) value in TEEC_operation? --> enter value to send to TA\n");
	printf("   --------------------------------------------------------------------------------------------------\n\n");


	printf("    ----------  ------   ------   ------  -------\n");
	printf("    Test Cases  Input1   Input2   Input3  Input4 \n");
	printf("               (Test #)  (iter)  (CMD_ID)  (val) \n");
	printf("    ----------  -------  ------   ------  -------\n");
	printf("     1 - 3         X        X        -       -   \n");
	printf("     4 - 9         X        1        -       -   \n");
	printf("     100          100       1        X       X   \n");
}

static void usage(int ret)
{
	printf("\n\n---------------------------------------------------------\n"
		"Usage: gptest -[OPTION] -[TEST_TYPE0]..-[TEST_TYPEn]\n\n"
		"Runs the user space tests specified by the TEST_TYPE\n"
		"parameter(s).\n"
		"\n\n"
		"OPTION can be:\n"
		"  -v, --verbose         run with debug messages on\n\n"
		"TEST_TYPE can be:\n"
		"  -i, --internal        Internal test cases\n"
		"  -u <UUID>             To load custom TA based on the UUID\n"
		"                        UUID format xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxxxx \n"
		"  -e <ExpRetRst>        Expected return result (in HEX)\n"
		"  -h, --help            print this help message and exit\n\n\n"
		" - 'adb push out/target/product/<target>/obj/EXECUTABLES/"
		"gptest_intermediates/gptest  /data/'\n\n"
		" - Connect to device: From command shell, do 'adb shell'\n"
		" - Once in the shell: do 'cd /data/'\n"
		" - Change permission for gptest: "
		"   do 'chmod 777 gptest'\n"
		" - Run gptest:\n"
		"      do './gptest -<OPTION> -<TEST_TYPE0> -<TEST_TYPE1>"
		" ......-<TEST_TYPEn>'\n"
		"         [see OPTION & TEST_TYPE described below]\n"
		"         EX: ./gptest -i <internal test number> \n"
		"---------------------------------------------------------\n");
	internal_test_menu();
	exit(ret);
}

int internal_test(int argc, char **argv)
{
	int test_number, result;
	int i = 0;
	unsigned int iterations = 0;

	if (!argv[0]) {
		internal_test_menu();
		return 0;
	}
	test_number = atoi(argv[0]);
	iterations = atoi(argv[1]);

	if (verbose)
		printf("Running test case %d\n", test_number);

	switch (test_number) {
	case 0:
		internal_test_menu();
		break;

	case 1:
		result = QTEEC_TEST_init_deinit_context(iterations);
		break;

	case 2:
		result = QTEEC_TEST_open_close_session(iterations);
		break;

	case 3:
		result = QTEEC_TEST_multiple_session(iterations);
		break;

	case 4:
		result =  QTEEC_TEST_TEEC_AllocateSharedMemory(0);
		break;

	case 5:
		result = QTEEC_TEST_TEEC_RegisterSharedMemory(0);
		break;

	case 6:
		result = QTEEC_TEST_TEEC_TempMemory(0);
		break;

	case 7:
		result =  QTEEC_TEST_TEEC_AllocateSharedMemory(1);
		break;

	case 8:
		result = QTEEC_TEST_TEEC_RegisterSharedMemory(1);
		break;

	case 9:
		result = QTEEC_TEST_TEEC_TempMemory(1);
		break;

	case 10:
		result = QTEEC_TEST_SharedMemoryAccessTest(0);
		break;

	case 100:
		command = atoi(argv[2]);
		value_a = atoi(argv[3]);
		result =  QTEEC_TEST_TEEC_AllocateSharedMemory(0);
		break;

	default:
		internal_test_menu();
		break;
	}
	exit(0);
}

static void gptest_get_uuid()
{
	char *pch;
	char *saveptr = NULL;
	char clockSeqAndNode[4] = {0};
	int i = 0;
	int ret = 0;

	pch = strtok_r (optarg, "-", &saveptr);
	if (!pch) {
		ret = -1;
		goto exit;
	}
	destination.timeLow = strtoul(pch, NULL, 16);
	pch = strtok_r (NULL, "-", &saveptr);
	if (!pch) {
		ret = -1;
		goto exit;
	}
	destination.timeMid = strtoul(pch, NULL, 16);
	pch = strtok_r (NULL, "-", &saveptr);
	if (!pch) {
		ret = -1;
		goto exit;
	}
	destination.timeHiAndVersion = strtoul(pch, NULL, 16);
	pch = strtok_r ((char *)NULL, "-", &saveptr);
	if (!pch) {
		ret = -1;
		goto exit;
	}
	for (i = 0; i < 8; i++) {
		if (!pch) {
			ret = -1;
			goto exit;
		}
		strlcpy(clockSeqAndNode, pch, 3);
		destination.clockSeqAndNode[i] =
			strtoul(clockSeqAndNode, NULL, 16);
		pch += 2;
	}
	printf("UUID Set to: 0x%8x-0x%4x-0x%4x-0x%2x%2x%2x%2x%2x%2x%2x%2x\n",
		destination.timeLow, destination.timeMid,
		destination.timeHiAndVersion,
		destination.clockSeqAndNode[0],destination.clockSeqAndNode[1],
		destination.clockSeqAndNode[2],destination.clockSeqAndNode[3],
		destination.clockSeqAndNode[4],destination.clockSeqAndNode[5],
		destination.clockSeqAndNode[6],destination.clockSeqAndNode[7]);
exit:
	if (ret)
		printf("Failed to parse UUID %s\n", optarg);
	return;
}

static unsigned int parse_command(int argc, char *const argv[])
{
	int command = 0;
	unsigned int ret = 0;

	verbose = 0;

	while ((command = getopt_long(argc, argv, "vihu:e:", testopts,
				      NULL)) != -1) {
		switch (command) {
		case 'v':
			verbose = 1;
			break;
		case 'i':
			ret = 1 << INTERNAL;
			break;
		case 'h':
			usage(0);
			break;
		case 'u':
			gptest_get_uuid(optarg);
			break;
		case 'e':
			expected_return_result = strtoul(optarg, NULL, 16);
			break;
		default:
			usage(-1);
		}
	}
	/* When only verbose option is given*/
	if (ret == 0)
		usage(-1);
	return ret;
}

int main(int argc, char **argv)
{
	int rc = 0;
	unsigned int num_tests_failed = 0;
	int error = -1;
	unsigned int test_mask = 0;

	test_mask = parse_command(argc, argv);

	/* Condition to check if it is a internal test or ABAIT test */
	if ((optind < argc) && ((test_mask & (1 << INTERNAL)) == (1U << INTERNAL)))
		internal_test(argc-optind, &argv[optind]);

	return (-num_tests_failed);
}
