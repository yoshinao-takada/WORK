#include "base_l/BL_fsm.h"

void BL_fsm_ah_do_nothing(pBL_fsm_t fsm) {}

BL_fsm_result_t BL_fsm_eh_do_nothing(pBL_fsm_t fsm, pBL_fsm_msg_t msg) { return BL_fsm_result_ignore; }

void BL_fsm_eval(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
{
    pcBL_fsm_state_t state_save = fsm->state;
    BL_fsm_result_t result = fsm->state->eh(fsm, msg);
    if (result == BL_fsm_result_transit)
    {
        state_save->ah_exit(fsm);
        fsm->msg = msg;
        fsm->state->ah_enter(fsm);
    }
}
