#include QMK_KEYBOARD_H
#include "tap_hold.c"


void leader_end_user(void) {
	if (leader_sequence_one_key(KC_D)) SEND_STRING("dragon-drop $f");
	else if (leader_sequence_one_key(KC_P)) SEND_STRING(":w" SS_TAP(X_ENT));
	else if (leader_sequence_one_key(KC_Y)) SEND_STRING(":q" SS_TAP(X_ENT));
	else if (leader_sequence_two_keys(KC_P, KC_Y)) SEND_STRING(":wq" SS_TAP(X_ENT));
	else if (leader_sequence_two_keys(KC_Y, KC_Y)) SEND_STRING(":q!" SS_TAP(X_ENT));
}

enum custom_keycodes {
	EK_SFTLOCK = SAFE_RANGE,
	EK_ACUTE_A,
	EK_ACUTE_E,
	EK_ACUTE_I,
	EK_ACUTE_O,
	EK_CIRCUNFLEX_A,
	EK_CIRCUNFLEX_E,
	EK_CIRCUNFLEX_O,
	EK_TILDE_A,
	EK_TILDE_O,
	EK_GRAVE_A,
	// Other custom keys...
};

const uint16_t PROGMEM combo_EK_ACUTE_A[] = {KC_A, KC_T, COMBO_END};
const uint16_t PROGMEM combo_EK_ACUTE_E[] = {KC_E, KC_T, COMBO_END};
const uint16_t PROGMEM combo_EK_ACUTE_I[] = {KC_I, KC_T, COMBO_END};
const uint16_t PROGMEM combo_EK_ACUTE_O[] = {KC_O, KC_T, COMBO_END};
const uint16_t PROGMEM combo_EK_CIRCUNFLEX_A[] = {KC_A, KC_P, COMBO_END};
const uint16_t PROGMEM combo_EK_CIRCUNFLEX_E[] = {KC_E, KC_P, COMBO_END};
const uint16_t PROGMEM combo_EK_CIRCUNFLEX_O[] = {KC_O, KC_P, COMBO_END};
const uint16_t PROGMEM combo_EK_TILDE_A[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM combo_EK_TILDE_O[] = {KC_O, KC_S, COMBO_END};
const uint16_t PROGMEM combo_EK_GRAVE_A[] = {KC_A, KC_Y, COMBO_END};
const uint16_t PROGMEM combo_C[] = {KC_C, KC_N, COMBO_END};


combo_t key_combos[] = {
	COMBO(combo_EK_ACUTE_A, EK_ACUTE_A),
	COMBO(combo_EK_ACUTE_E, EK_ACUTE_E),
	COMBO(combo_EK_ACUTE_I, EK_ACUTE_I),
	COMBO(combo_EK_ACUTE_O, EK_ACUTE_O),
	COMBO(combo_EK_CIRCUNFLEX_A, EK_CIRCUNFLEX_A),
	COMBO(combo_EK_CIRCUNFLEX_E, EK_CIRCUNFLEX_E),
	COMBO(combo_EK_CIRCUNFLEX_O, EK_CIRCUNFLEX_O),
	COMBO(combo_EK_TILDE_A, EK_TILDE_A),
	COMBO(combo_EK_TILDE_O, EK_TILDE_O),
	COMBO(combo_EK_GRAVE_A, EK_GRAVE_A),
	COMBO(combo_C, ALGR(KC_COMM)),	
};

int COMBO_LEN = sizeof(key_combos)/sizeof(key_combos[0]);

enum {
	//TD_NAV,
	TD_CAPS,
	TD_SPC,
	TD_COPY
};

uint16_t get_diacritic_key(uint16_t keycode) {
	switch (keycode) {
		case EK_ACUTE_A: return KC_A;
		case EK_ACUTE_E: return KC_E;
		case EK_ACUTE_I: return KC_I;
		case EK_ACUTE_O: return KC_O;
		case EK_CIRCUNFLEX_A: return KC_A;
		case EK_CIRCUNFLEX_E: return KC_E;
		case EK_CIRCUNFLEX_O: return KC_O;
		case EK_TILDE_A: return KC_A;
		case EK_GRAVE_A: return KC_A;
		case EK_TILDE_O: return KC_O;
	}
	return 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
	tap_dance_action_t *action;
	uint8_t mod_state = get_mods();
	uint8_t oneshot_mod_state = get_oneshot_mods();
	switch (keycode) {
	case EK_ACUTE_A ... EK_ACUTE_O:
		if (record->event.pressed) {
				del_mods(MOD_MASK_SHIFT);
				del_oneshot_mods(MOD_MASK_SHIFT);
				tap_code16(ALGR(KC_QUOT));
				set_mods(mod_state);
				set_oneshot_mods(oneshot_mod_state);
				tap_code(get_diacritic_key(keycode));
			}
		return false;
	case EK_CIRCUNFLEX_A ... EK_CIRCUNFLEX_O:
		if (record->event.pressed) {
				del_mods(MOD_MASK_SHIFT);
				del_oneshot_mods(MOD_MASK_SHIFT);
				tap_code16(ALGR(KC_6));
				set_mods(mod_state);
				set_oneshot_mods(oneshot_mod_state);
				tap_code(get_diacritic_key(keycode));
			}
		return false;
	case EK_TILDE_A ... EK_TILDE_O:
		if (record->event.pressed) {
				del_mods(MOD_MASK_SHIFT);
				del_oneshot_mods(MOD_MASK_SHIFT);
				tap_code16(ALGR(LSFT(KC_GRV)));
				set_mods(mod_state);
				set_oneshot_mods(oneshot_mod_state);
				tap_code(get_diacritic_key(keycode));
			}
		return false;
	case EK_GRAVE_A:
			if (record->event.pressed) {
				del_mods(MOD_MASK_SHIFT);
				del_oneshot_mods(MOD_MASK_SHIFT);
				tap_code16(ALGR(KC_GRV));
				set_mods(mod_state);
				set_oneshot_mods(oneshot_mod_state);
				tap_code(KC_A);
			}
			return false;
	case EK_SFTLOCK:  // Shift Lock handler.
		if (record->event.pressed) {
		// `static` to remember the value across calls.
		static bool locked = false;
		locked = !locked;  // Flip the lock.

		if (locked) {
			register_mods(MOD_BIT(KC_LSFT));  // Hold Shift.
		} else {
			unregister_mods(MOD_BIT(KC_LSFT));  // Release Shift.
		}
		}
		return false;
	case TD(TD_SPC):  // list all tap dance keycodes with tap-hold configurations
	case TD(TD_COPY):
	// case TD(TD_CAPS):
		action = &tap_dance_actions[TD_INDEX(keycode)];
		if (!record->event.pressed && action->state.count && !action->state.finished) {
			tap_dance_tap_hold_t *tap_hold = (tap_dance_tap_hold_t *)action->user_data;
			tap_code16(tap_hold->tap);
		}
		break;
	case TD(TD_CAPS):	
	//case TD(TD_NAV):
		action = &tap_dance_actions[TD_INDEX(keycode)];
		if (!record->event.pressed && action->state.count && !action->state.finished) {
			tap_dance_tap_hold_macro_t *tap_hold = (tap_dance_tap_hold_macro_t *)action->user_data;
			(tap_hold->tap)(&action->state);
		}
		break;
	}
	return true;
}

void shift_lock(tap_dance_state_t *state) {
	if (state->pressed) {
        // `static` to remember the value across calls.
        static bool locked = false;
        locked = !locked;  // Flip the lock.

        if (locked) {
          register_mods(MOD_BIT(KC_LSFT));  // Hold Shift.
        } else {
          unregister_mods(MOD_BIT(KC_LSFT));  // Release Shift.
		}
	}
}

void lock_lgui(tap_dance_state_t *state) {
	if (state->pressed) {
        // `static` to remember the value across calls.
        static bool locked = false;
        locked = !locked;  // Flip the lock.

        if (locked) {
          register_mods(MOD_BIT(KC_LGUI));  // Hold Shift.
        } else {
          unregister_mods(MOD_BIT(KC_LGUI));  // Release Shift.
		}
	}
}

void toggle_caps(tap_dance_state_t *state) { tap_code(KC_CAPS); }
void toggle_caps_word(tap_dance_state_t *state) { caps_word_toggle(); }

void dance_paste_copy_cut(tap_dance_state_t *state, void *user_data) {	
	switch (state->count) {
		case 1:
			tap_code16(C(KC_V));
			break;
		case 2:
			tap_code16(C(KC_C));
			break;
		case 3:
			tap_code16(C(KC_X));
			break;
	}
	reset_tap_dance(state);
}

tap_dance_action_t tap_dance_actions[] = {
	[TD_SPC] = ACTION_TAP_DANCE_TAP_HOLD(KC_SPC, KC_LALT),
	[TD_COPY] = ACTION_TAP_DANCE_TAP_HOLD(C(KC_C), C(KC_X)),
	[TD_CAPS] = ACTION_TAP_HOLD_MACRO(toggle_caps_word, toggle_caps),
	//[TD_NAV] = ACTION_TAP_HOLD_MACRO(osm_lctl, move_layer_1)		
};

// const key_override_t key_override_u = ko_make_basic(MOD_MASK_ALT, KC_O, KC_UP);
// const key_override_t key_override_d = ko_make_basic(MOD_MASK_ALT, KC_E, KC_DOWN);
// const key_override_t key_override_l = ko_make_basic(MOD_MASK_ALT, KC_N, KC_LEFT);
// const key_override_t key_override_r = ko_make_basic(MOD_MASK_ALT, KC_I, KC_RIGHT);
// const key_override_t key_override_backspace = ko_make_basic(MOD_MASK_ALT, KC_M, KC_BSPC);
// const key_override_t key_override_delete = ko_make_basic(MOD_MASK_ALT, KC_H, KC_DEL);
// const key_override_t key_override_home = ko_make_basic(MOD_MASK_ALT, KC_F, KC_HOME);
// const key_override_t key_override_end = ko_make_basic(MOD_MASK_ALT, KC_A, KC_END);
// const key_override_t key_override_c_right = ko_make_basic(MOD_MASK_ALT, KC_T, C(KC_RIGHT));
// const key_override_t key_override_c_left = ko_make_basic(MOD_MASK_ALT, KC_S, C(KC_LEFT));
// const key_override_t key_override_sc_right = ko_make_basic(MOD_MASK_ALT, KC_P, S(C(KC_RIGHT)));
// const key_override_t key_override_sc_left = ko_make_basic(MOD_MASK_ALT, KC_Y, S(C(KC_LEFT)));
// //const key_override_t key_override_sc_copy = ko_make_basic(MOD_MASK_CS, KC_C, C(KC_X));

// // This globally defines all key overrides to be used
// const key_override_t **key_overrides = (const key_override_t *[]){
// 	&key_override_u,
// 	&key_override_d,
// 	&key_override_l,
// 	&key_override_r,
// 	&key_override_backspace,
// 	&key_override_delete,
// 	&key_override_end,
// 	&key_override_home,	
// 	&key_override_sc_right,
// 	&key_override_sc_left,
// 	&key_override_c_right,
// 	&key_override_c_left,
// 	//&key_override_sc_copy,
// 	NULL // Null terminate the array of overrides!
// };


#include "keymap.g.c"

