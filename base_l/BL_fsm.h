#ifndef BL_FSM_H_
#define BL_FSM_H_
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

    struct BL_fsm;

    typedef struct BL_fsm* pBL_fsm_t;
    typedef const struct BL_fsm* pcBL_fsm_t;

    typedef enum {
        BL_fsm_result_ignore,
        BL_fsm_result_no_transit,
        BL_fsm_result_transit
    } BL_fsm_result_t;

    typedef struct {
        intptr_t msgid;
        void* payload;
    } BL_fsm_msg_t, *pBL_fsm_msg_t;

    typedef const BL_fsm_msg_t *pcBL_fsm_msg_t;

    typedef void (*BL_fsm_action_handler)(pBL_fsm_t fsm);
    typedef BL_fsm_result_t (*BL_fsm_event_handler)(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    typedef struct {
        BL_fsm_action_handler ah_enter,ah_exit;
        BL_fsm_event_handler eh;
    } BL_fsm_state_t, *pBL_fsm_state_t;

    typedef const BL_fsm_state_t* pcBL_fsm_state_t;

    typedef struct BL_fsm {
        pcBL_fsm_state_t state;
        pBL_fsm_msg_t msg;
        void* context;
    } BL_fsm_t;

    /*!
    \brief dummy action handler which does nothing.
    */
    void BL_fsm_ah_do_nothing(pBL_fsm_t fsm);

    /*!
    \brief dummy event handler which does nothing.
    */
    BL_fsm_result_t BL_fsm_eh_do_nothing(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    /*!
    \brief evaluate state and message
    */
    void BL_fsm_eval(pBL_fsm_t fsm, pBL_fsm_msg_t msg);
#ifdef __cplusplus
}
#endif
#endif /* BL_FSM_H_ */
