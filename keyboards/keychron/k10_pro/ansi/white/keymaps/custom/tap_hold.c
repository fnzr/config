typedef struct {
    int16_t tap;
    int16_t hold;
    int16_t held;
} tap_dance_tap_hold_t;

void tap_dance_tap_hold_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;
 
    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {
            register_code16(tap_hold->hold);
            tap_hold->held = tap_hold->hold;
        } else {
            register_code16(tap_hold->tap);
            tap_hold->held = tap_hold->tap;
        }
    }
}

void tap_dance_tap_hold_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)user_data;

    if (tap_hold->held) {
        unregister_code16(tap_hold->held);
        tap_hold->held = 0;
    }
}

#define ACTION_TAP_DANCE_TAP_HOLD(tap, hold) { .fn = {NULL, tap_dance_tap_hold_finished, tap_dance_tap_hold_reset}, .user_data = (void *)&((tap_dance_tap_hold_t){tap, hold, 0}), }

typedef void (*tap_dance_macro_function_t)(tap_dance_state_t*);
typedef struct {
    tap_dance_macro_function_t tap;
    tap_dance_macro_function_t hold;
} tap_dance_tap_hold_macro_t;

void tap_dance_tap_hold_macro_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_tap_hold_macro_t *tap_hold = (tap_dance_tap_hold_macro_t *)user_data;

    if (state->pressed) {
        if (state->count == 1
#ifndef PERMISSIVE_HOLD
            && !state->interrupted
#endif
        ) {            
            (tap_hold->hold)(state);
        }
    }
}

#define ACTION_TAP_HOLD_MACRO(tap, hold) { .fn = {NULL, tap_dance_tap_hold_macro_finished, NULL}, .user_data = (void *)&((tap_dance_tap_hold_macro_t){tap, hold}), }