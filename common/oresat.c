#include "oresat.h"
#include "CANopen.h"
#include "can_threads.h"

typedef struct {
    thread_descriptor_t desc;
    thread_t *tp;
} worker_t;

static worker_t workers[ORESAT_MAX_THREADS];
static uint32_t num_workers;

int reg_worker(const char *name, void *wa, size_t wa_size, tprio_t prio, tfunc_t funcp, void *arg)
{
    worker_t *wp;
    if (num_workers == ORESAT_MAX_THREADS) {
        return -1;
    }

    wp = &workers[num_workers];
    wp->desc.name = name;
    wp->desc.wbase = THD_WORKING_AREA_BASE(wa);
    wp->desc.wend = THD_WORKING_AREA_END(wa + wa_size);
    wp->desc.prio = prio;
    wp->desc.funcp = funcp;
    wp->desc.arg = arg;

    return num_workers++;
}

void oresat_init(uint8_t node_id, uint16_t bitrate)
{
    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chSysInit();

    /* If node ID is not overridden, set node ID from user data byte 0 */
    if (node_id == ORESAT_DEFAULT_ID) {
        node_id = _FLD2VAL(FLASH_OBR_DATA0, FLASH->OBR);
    }

    //TODO: If node ID is invalid, get new node ID
    if (node_id > 0x7F) {
        node_id = 0x7F;
    }
    OD_CANNodeID = node_id;
    OD_CANBitRate = bitrate;

    return;
}

void oresat_start(CANDriver *cand)
{
    CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
    thread_t *can_tp;
    uint16_t sleep_ms;
    systime_t prev_time, cur_time, diff_time;

    while (reset != CO_RESET_APP) {
        CO_ReturnError_t err;

        /* Initialize CAN Subsystem */
        err = CO_init((uint32_t)cand, OD_CANNodeID, OD_CANBitRate);
        if (err != CO_ERROR_NO) {
            CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err);
        }

        can_tp = chThdCreateStatic(can_wa, sizeof(can_wa), HIGHPRIO, can, CO);

        cand->rxfull_cb = CO_CANrx_cb;
        cand->txempty_cb = CO_CANtx_cb;

        CO_CANsetNormalMode(CO->CANmodule[0]);

        /* Start app workers */
        for (uint32_t i = 0; i < num_workers; i++) {
            workers[i].tp = chThdCreate(&workers[i].desc);
        }

        reset = CO_RESET_NOT;
        prev_time = chVTGetSystemTimeX();
        while (reset == CO_RESET_NOT) {
            sleep_ms = 50U;
            diff_time = chTimeDiffX(prev_time, cur_time = chVTGetSystemTimeX());
            prev_time = cur_time;
            reset = CO_process(CO, chTimeI2MS(diff_time), &sleep_ms);
            chThdSleepMilliseconds(sleep_ms);
        }

        for (uint32_t i = 0; i < num_workers; i++) {
            chThdTerminate(workers[i].tp);
            chThdWait(workers[i].tp);
        }
        chThdTerminate(can_tp);
        chThdWait(can_tp);
    }

    CO_delete((uint32_t)cand);

    /* Reset */
    NVIC_SystemReset();
    return;
}
