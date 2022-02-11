#include QMK_KEYBOARD_H

enum layer_names {
    _QWERTY,
    _FUN,
    _UPPER,
};

enum atreus_keycodes {
    MK_BOOT,    // reset after hold timeout
};

#define MK_ALT  MT(MOD_LALT, KC_RALT)
#define MK_ESC  LT(_UPPER, KC_ESC)
#define MK_FUN  LT(_FUN, KC_ESC)
#define MK_SPC  MT(MOD_LALT, KC_SPC)
#define MK_BSPC MT(MOD_LCTL, KC_BSPC)
#define MK_DEL  MT(MOD_LCTL, KC_DEL)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT( /* Qwerty */
        KC_Q,   KC_W,   KC_E,    KC_R,    KC_T,                      KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,
        KC_A,   KC_S,   KC_D,    KC_F,    KC_G,                      KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN,
        KC_Z,   KC_X,   KC_C,    KC_V,    KC_B,    KC_GRV,  KC_BSLS, KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH,
        MK_ESC, KC_TAB, KC_LGUI, KC_LSFT, MK_BSPC, KC_LCTL, MK_ALT,  MK_SPC, MK_FUN, KC_MINS, KC_QUOT, KC_ENT
    ),
    [_FUN] = LAYOUT(
        KC_EXLM, KC_AT,   KC_UP,   KC_DLR,  KC_PERC,                   KC_PGUP, KC_7,    KC_8,   KC_9, KC_BSPC,
        KC_LPRN, KC_LEFT, KC_DOWN, KC_RGHT, KC_RPRN,                   KC_PGDN, KC_4,    KC_5,   KC_6, KC_COLN,
        KC_LBRC, KC_RBRC, KC_HASH, KC_LCBR, KC_RCBR, KC_CIRC, KC_AMPR, KC_ASTR, KC_1,    KC_2,   KC_3, KC_PLUS,
        MK_ESC,  KC_INS,  KC_LGUI, KC_LSFT, MK_DEL,  KC_LCTL, KC_LALT, MK_SPC,  _______, KC_DOT, KC_0, KC_EQL
    ),
    [_UPPER] = LAYOUT(
        KC_INS,  KC_PGUP, KC_UP,   KC_PGDN, KC_HOME,                   KC_VOLU, KC_F7,  KC_F8,   KC_F9,   KC_F10,
        KC_DEL,  KC_LEFT, KC_DOWN, KC_RGHT, KC_END,                    KC_VOLD, KC_F4,  KC_F5,   KC_F6,   KC_F11,
        _______, KC_CUT,  KC_COPY, KC_PASTE,MK_BOOT, _______, KC_PSCR, KC_MUTE, KC_F1,  KC_F2,   KC_F3,   KC_F12,
        _______, KC_MENU, KC_LGUI, KC_LSFT, MK_BSPC, KC_LCTL, KC_LALT, MK_SPC,  MK_FUN, KC_MPRV, KC_MNXT, KC_PAUS
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case MK_BOOT: // restart into bootloader after hold timeout
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 1500) {
                    reset_keyboard();
                }
            }
            return false;
        default:
            return true; // process all other keycodes normally
    }
}
