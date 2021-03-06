/*
 * Copyright (C) 2013-2016, Shenzhen Huiding Technology Co., Ltd.
 * All Rights Reserved.
 */
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <dirent.h>

#include "gf_type_define.h"
#include "gf_error.h"
#include "gf_hal.h"

#include "gf_fingerprint.h"
#include "ftm_factory_test.h"

#define LOG_TAG "[GF_HAL][gf_hal_factory_test]"

static gf_fingerprint_device_t *mDevice = NULL;
static sem_t g_notify_sem;
static uint8_t g_is_pass = 0;
static int32_t g_notify_cmd_id = 0;

extern void parser(uint8_t* result, uint32_t len);
extern gf_error_t update_config(token_map_t* result,uint32_t count);

extern token_map_t g_token_map[MAX_SIZE];
extern uint32_t g_token_count;

extern int g_ChipType;
extern int g_ChipSeries;

static uint32_t g_success_count = 0;
static uint32_t g_fail_count = 0;

#ifdef FACTORY_TEST_REE
#define  FILE_PATH  "/data/gf_data/factory/"
#define  PATH_LENGTH  1024
static uint8_t g_path[PATH_LENGTH];
#endif

static gf_cmd_test_id_t g_test_item_oswego_m[] = {
        CMD_TEST_SPI,
        CMD_TEST_RESET_PIN,
        CMD_TEST_INTERRUPT_PIN,
        CMD_TEST_PIXEL_OPEN,
        CMD_TEST_GET_VERSION
};

static gf_cmd_test_id_t g_test_item_milan_a_series[] = {
        CMD_TEST_SPI,
        CMD_TEST_RESET_PIN,
        CMD_TEST_GET_VERSION,
        CMD_TEST_INTERRUPT_PIN,
        CMD_TEST_PIXEL_OPEN
};

static gf_cmd_test_id_t g_test_item_milan_f_series[] = {
        CMD_TEST_SPI,
        CMD_TEST_RESET_PIN,
        CMD_TEST_INTERRUPT_PIN,
        CMD_TEST_SENSOR_VALIDITY,
        CMD_TEST_PIXEL_OPEN,
        CMD_TEST_GET_VERSION
};

static gf_cmd_test_id_t *g_test_item = NULL;

extern void parser(uint8_t* result,uint32_t len);

void hal_notify_callback(const gf_fingerprint_msg_t *msg) {
    int is_notify = 0;

    LOG_D(LOG_TAG, "[%s] type=%d, cmd_id=%d", __func__, msg->type,
            msg->data.test.cmd_id);
    switch (msg->type) {
    case GF_FINGERPRINT_ERROR:
        break;
    case GF_FINGERPRINT_ACQUIRED:
        break;
    case GF_FINGERPRINT_AUTHENTICATED:
        break;
    case GF_FINGERPRINT_TEMPLATE_ENROLLING:
        break;
    case GF_FINGERPRINT_TEMPLATE_REMOVED:
        break;
    case GF_FINGERPRINT_TEST_CMD:
        g_notify_cmd_id = msg->data.test.cmd_id;
        parser(msg->data.test.result, msg->data.test.result_len);

        is_notify = 1;
        break;
    case GF_FINGERPRINT_HBD:
        break;
    default:
        break;
    }

    if (is_notify != 0) {
        sem_post(&g_notify_sem);
    }
}

void gf_call_checker(void) {
    LOG_D(LOG_TAG, "[%s] cmd_id=%d", __func__, g_notify_cmd_id);

    switch (g_notify_cmd_id) {
    case CMD_TEST_ENUMERATE:
        break;
    case CMD_TEST_DRIVER: ////
        break;
    case CMD_TEST_PIXEL_OPEN:
        g_is_pass = checkPixelTestResult(g_token_map, g_token_count);
        break;
    case CMD_TEST_BAD_POINT:
        if (GF_OSWEGO_M == g_ChipSeries) {
            g_is_pass = Oswego_checkBadPointTestResult(g_token_map,
                    g_token_count);
        } else if (GF_MILAN_A_SERIES == g_ChipSeries) {
            g_is_pass = MilanASeries_checkBadPointTestResult(g_token_map,
                    g_token_count);
        } else if (GF_MILAN_F_SERIES == g_ChipSeries) {
            g_is_pass = MilanFSeries_checkBadPointTestResult(g_token_map,
                    g_token_count);
        }
        break;
    case CMD_TEST_PERFORMANCE:
        g_is_pass = checkPerformanceTestResult(g_token_map, g_token_count);
        break;
    case CMD_TEST_SPI_PERFORMANCE:
        break;
    case CMD_TEST_SPI:
        if (GF_OSWEGO_M == g_ChipSeries) {
            g_is_pass = Oswego_checkSpiTestResult(g_token_map, g_token_count);
        } else if (GF_MILAN_A_SERIES == g_ChipSeries) {
            g_is_pass = MilanASeries_checkSpiTestResult(g_token_map, g_token_count);
        } else if (GF_MILAN_F_SERIES == g_ChipSeries) {
            g_is_pass = MilanFSeries_checkSpiTestResult(g_token_map,
                    g_token_count);
        }
        break;
    case CMD_TEST_GET_VERSION:
        if (GF_OSWEGO_M == g_ChipSeries) {
            g_is_pass = Oswego_checkFwVersionTestResult(g_token_map,
                    g_token_count);
        } else if (GF_MILAN_A_SERIES == g_ChipSeries) {
            g_is_pass = MilanASeries_checkFwVersionTestResult(g_token_map,
                    g_token_count);
        } else if (GF_MILAN_F_SERIES == g_ChipSeries) {
            g_is_pass = checkFwVersionTestResult(g_token_map, g_token_count);
        }
        break;
    case CMD_TEST_FRR_FAR_GET_CHIP_TYPE:
        break;
    case CMD_TEST_FRR_FAR_INIT: ////
        break;
    case CMD_TEST_FRR_FAR_RECORD_BASE_FRAME:
        break;
    case CMD_TEST_FRR_FAR_RECORD_ENROLL:
        break;
    case CMD_TEST_FRR_FAR_RECORD_AUTHENTICATE:
        break;
    case CMD_TEST_FRR_FAR_RECORD_AUTHENTICATE_FINISH:
        break;
    case CMD_TEST_FRR_FAR_PLAY_BASE_FRAME: ////
        break;
    case CMD_TEST_FRR_FAR_PLAY_ENROLL: ////
        break;
    case CMD_TEST_FRR_FAR_PLAY_AUTHENTICATE: ////
        break;
    case CMD_TEST_FRR_FAR_ENROLL_FINISH:
        break;
    case CMD_TEST_CANCEL_FRR_FAR:
        break;
    case CMD_TEST_RESET_PIN:
        g_is_pass = checkResetPinTestReuslt(g_token_map, g_token_count);
        break;
    case CMD_TEST_CANCEL:
        break;
    case CMD_TEST_PRIOR_CANCEL:
        break;
    case CMD_TEST_GET_CONFIG: ///
        break;
    case CMD_TEST_SET_CONFIG: ////
        break;
    case CMD_TEST_DOWNLOAD_FW: ////
        break;
    case CMD_TEST_DOWNLOAD_CFG: ////
        break;
    case CMD_TEST_DOWNLOAD_FWCFG:
        break;
    case CMD_TEST_RESET_FWCFG:
        break;
    case CMD_TEST_SENSOR_VALIDITY:
        if (GF_MILAN_F_SERIES == g_ChipSeries) {
            LOG_D(LOG_TAG, "[%s] test sensor validity for milan_f_series", __func__);
            g_is_pass = MilanFSeries_checkSensorValidity(g_token_map,
                    g_token_count);
        } else {// not support now, but check it as passed.
            g_is_pass = 1;
        }
        break;
    case CMD_TEST_RESET_CHIP:
        break;
    case CMD_TEST_UNTRUSTED_ENROLL:
        break;
    case CMD_TEST_INTERRUPT_PIN:
        g_is_pass = checkInterruptPinTestReuslt(g_token_map, g_token_count);
        break;
    default:
        break;
    }
}

#ifdef FACTORY_TEST_REE
gf_error_t gf_delete_all(void) {
    gf_error_t err = GF_SUCCESS;
    int ret;
    struct dirent *de;
    DIR *dr;

    FUNC_ENTER();

    do {
	if (sizeof(FILE_PATH) < (PATH_LENGTH + 1)) {
            memset(g_path, 0, PATH_LENGTH);
            strncpy(g_path, FILE_PATH, sizeof(FILE_PATH));
	} else {
            LOG_E(LOG_TAG, "[%s]Invalid Filepath: %s", __func__, FILE_PATH);
            err = GF_ERROR_FILE_OPEN_FAILED;
            break;
	}

        dr = opendir(g_path);
        if (dr == NULL){
            err = GF_ERROR_FILE_OPEN_FAILED;
            break;
        }

        while ((de = readdir(dr)) != NULL){
            LOG_D(LOG_TAG, "[%s] remove file=%s", __func__, de->d_name);
            remove(de->d_name);
        }

        closedir(dr);
        remove(g_path);

        err = GF_SUCCESS;
    }while (0);

    FUNC_EXIT(err);

    return err;
}
#endif

int gf_factory_test_entry(void) {
    gf_error_t err = GF_SUCCESS;
    uint32_t count = 0;
    uint32_t total_count = 0;
    uint8_t is_pass = 0;
    uint32_t retry_times = 3000*10;

	LOG_E(LOG_TAG, "[%s]", __func__);

    do {
        /* 1.create semaphore */
        if (0 != sem_init(&g_notify_sem, 0, 0)) {
            LOG_E(LOG_TAG, "[%s] init semaphore failed", __func__);
            err = GF_ERROR_HAL_GENERAL_ERROR;
            break;
        }

        /*2.init*/
        mDevice = (gf_fingerprint_device_t *) malloc(
                sizeof(gf_fingerprint_device_t));
        memset(mDevice, 0, sizeof(gf_fingerprint_device_t));
        mDevice->test_notify = hal_notify_callback;
        g_fail_count = 0;
        g_success_count = 0;

        /*3.hal open*/
        err = gf_hal_open(mDevice);
        if (GF_SUCCESS != err) {
            LOG_E(LOG_TAG, "[%s] hal open failed, please check.", __func__);
            break;
        }

        /*4.wait for fingerbase finished*/
        while(gf_hal_is_inited() != 1 && (retry_times-- > 0)){
            usleep(100);
        }

        if (0 == retry_times) {
            LOG_E(LOG_TAG, "[%s] hal is not inited, exit factory testing...", __func__);
            break;
        }

        /*5.get config from hal */
        err = gf_hal_test_cmd(mDevice, CMD_TEST_GET_CONFIG, NULL, 0);
        if (GF_SUCCESS != err) {
            LOG_E(LOG_TAG, "[%s] CMD_TEST_GET_CONFIG Failed, exit factory testing...", __func__);
            break;
        }
    
        /* wait callback from hal*/
        sem_wait(&g_notify_sem);
        if (g_notify_cmd_id == CMD_TEST_GET_CONFIG) {
            err = update_config(g_token_map, g_token_count);
            if (GF_SUCCESS != err) {
                break;
            }
        }

        if (GF_OSWEGO_M == g_ChipSeries) {
            initOswegoChecker(g_ChipType);
            g_test_item = g_test_item_oswego_m;
            total_count = sizeof(g_test_item_oswego_m) / sizeof(g_test_item_oswego_m[0]);
        } else if (GF_MILAN_A_SERIES == g_ChipSeries) {
            initMilanASeriesChecker(g_ChipType);
            g_test_item = g_test_item_milan_a_series;
            total_count = sizeof(g_test_item_milan_a_series) / sizeof(g_test_item_milan_a_series[0]);
        } else if (GF_MILAN_F_SERIES == g_ChipSeries) {
            initMilanFSeriesChecker(g_ChipType);
            g_test_item = g_test_item_milan_f_series;
            total_count = sizeof(g_test_item_milan_f_series) / sizeof(g_test_item_milan_f_series[0]);
        } else {
            break;
        }

        /* 6.do test and wait callback from hal*/
        count = 0;
        while (count < total_count) {
            g_is_pass = 0;

            /* these cmd need parameter,so temporary does not support */
            if (CMD_TEST_DRIVER == g_test_item[count]
                    || CMD_TEST_FRR_FAR_INIT == g_test_item[count]
                    || CMD_TEST_FRR_FAR_PLAY_BASE_FRAME == g_test_item[count]
                    || CMD_TEST_FRR_FAR_PLAY_ENROLL == g_test_item[count]
                    || CMD_TEST_FRR_FAR_PLAY_AUTHENTICATE == g_test_item[count]
                    || CMD_TEST_DOWNLOAD_FW == g_test_item[count]
                    || CMD_TEST_DOWNLOAD_CFG == g_test_item[count]
                    || CMD_TEST_DOWNLOAD_FWCFG == g_test_item[count]
                    || CMD_TEST_RESET_FWCFG == g_test_item[count]
                    || CMD_TEST_SET_CONFIG == g_test_item[count]) {
                count++;
                continue;
            }

            err = gf_hal_test_cmd(mDevice, g_test_item[count], NULL, 0);
            if (GF_SUCCESS == err) {
                /* wait callback from hal*/
                sem_wait(&g_notify_sem);
                if (g_notify_cmd_id != g_test_item[count]) {
                    LOG_E(LOG_TAG, "[%s] bug,notify error", __func__);
                }

                gf_call_checker();

                if (g_is_pass) {
                    g_success_count++;
                } else {
                    LOG_E(LOG_TAG, "[%s] fail1 count = %d", __func__, count);
                    g_fail_count++;
                }
            } else {
                LOG_E(LOG_TAG, "[%s] fail2 count = %d", __func__, count);
                g_fail_count++;
                LOG_I(LOG_TAG, "[%s] the [%d] item test failed.", __func__, (count+1));
            }

            count++;
        }

        if (count == g_success_count) {
            is_pass = 1;
        }

        LOG_I(LOG_TAG, "[%s] success=%u, fail=%u", __func__, g_success_count,
                g_fail_count);
    } while (0);

    sem_destroy(&g_notify_sem);
    if (mDevice != NULL) {
#ifdef FACTORY_TEST_REE
        err = gf_delete_all();
        if (GF_SUCCESS != err) {
            /* may fail while factory test in TEE */
            LOG_E(LOG_TAG, "[%s] delete fail, err=%s, errno=%d", __func__, gf_strerror(err), err);
        }
#endif

        /* 7.hal close*/
        err = gf_hal_close(mDevice);
        if (GF_SUCCESS != err) {
            LOG_E(LOG_TAG, "[%s] hal close failed, please check...", __func__);
        }

        free(mDevice);
        mDevice = NULL;
    }

    return (1 == is_pass) ? (0) : (-1);
}

