#include QMK_KEYBOARD_H

enum alt_keycodes {
    U_T_AUTO = SAFE_RANGE, //USB Extra Port Toggle Auto Detect / Always Active
    U_T_AGCR,              //USB Toggle Automatic GCR control
    DBG_TOG,               //DEBUG Toggle On / Off
    DBG_MTRX,              //DEBUG Toggle Matrix Prints
    DBG_KBD,               //DEBUG Toggle Keyboard Prints
    DBG_MOU,               //DEBUG Toggle Mouse Prints
    MD_BOOT,               //Restart into bootloader after hold timeout
    MKRGBTOG,              //RGB Toggle the Second
};

#define MK_CAPS MT(MOD_LCTL, KC_ESC)
#define MK_LCTL LT(1, KC_ESC)
#define MK_RCTL MT(MOD_RCTL, KC_RGUI)
#define MK_LSFT MT(MOD_LSFT, KC_MPRV)
#define MK_RSFT MT(MOD_RSFT, KC_MNXT)
#define MK_RALT LT(1, KC_RALT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_65_ansi_blocker(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,  \
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, \
        MK_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP, \
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,        KC_UP,   KC_PGDN, \
        MK_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             MK_RALT, MK_RCTL, KC_LEFT, KC_DOWN, KC_RGHT  \
    ),
    [1] = LAYOUT_65_ansi_blocker(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_MUTE, \
        _______, RGB_SPD, RGB_VAI, RGB_SPI, RGB_HUI, RGB_SAI, _______, _______, KC_INS,  _______, KC_PSCR, KC_SLCK, KC_PAUS, _______, KC_END,  \
        _______, RGB_RMOD,RGB_VAD, RGB_MOD, RGB_HUD, RGB_SAD, _______, _______, _______, _______, KC_INS,  KC_DEL,           _______, KC_VOLU, \
        MK_LSFT, RGB_TOG, MKRGBTOG,_______, _______, MD_BOOT, _______, KC_MUTE, KC_MPRV, KC_MNXT, KC_MPLY, MK_RSFT,          KC_PGUP, KC_VOLD, \
        _______, _______, _______,                            _______,                            _______, _______, KC_HOME, KC_PGDN, KC_END   \
    ),
    /*
    [X] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, \
        _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______  \
    ),
    */
};

#define MODS_SHIFT  (get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT))
#define MODS_CTRL  (get_mods() & MOD_BIT(KC_LCTL) || get_mods() & MOD_BIT(KC_RCTRL))
#define MODS_ALT  (get_mods() & MOD_BIT(KC_LALT) || get_mods() & MOD_BIT(KC_RALT))
#define MODS_GUI  (get_mods() & MOD_BIT(KC_LGUI) || get_mods() & MOD_BIT(KC_RGUI))

#define LED_FLAG_KEYS (LED_FLAG_KEYLIGHT | LED_FLAG_MODIFIER | LED_FLAG_INDICATOR)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint32_t key_timer;

    switch (keycode) {
        case U_T_AUTO:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_extra_manual, "USB extra port manual mode");
            }
            return false;
        case U_T_AGCR:
            if (record->event.pressed && MODS_SHIFT && MODS_CTRL) {
                TOGGLE_FLAG_AND_PRINT(usb_gcr_auto, "USB GCR auto mode");
            }
            return false;
        case DBG_TOG:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_enable, "Debug mode");
            }
            return false;
        case DBG_MTRX:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_matrix, "Debug matrix");
            }
            return false;
        case DBG_KBD:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_keyboard, "Debug keyboard");
            }
            return false;
        case DBG_MOU:
            if (record->event.pressed) {
                TOGGLE_FLAG_AND_PRINT(debug_mouse, "Debug mouse");
            }
            return false;
        case MD_BOOT: // restart into bootloader after hold timeout
            if (record->event.pressed) {
                key_timer = timer_read32();
            } else {
                if (timer_elapsed32(key_timer) >= 1500) {
                    reset_keyboard();
                }
            }
            return false;
        case RGB_TOG: // toggle rgb backlight
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                        rgb_matrix_set_color_all(0, 0, 0);
                    }
                    break;
                    case (LED_FLAG_KEYS): {
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_disable_noeeprom();
                    }
                    break;
                    case LED_FLAG_UNDERGLOW: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                        // rgb_matrix_set_color_all(0, 0, 0);
                    }
                    break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_KEYS);
                        rgb_matrix_enable_noeeprom();
                    }
                    break;
                }
            }
            return false;
        case MKRGBTOG: // toggle rgb underglow
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_KEYS);
                        rgb_matrix_set_color_all(0, 0, 0);
                    }
                    break;
                    case (LED_FLAG_KEYS): {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                        // rgb_matrix_set_color_all(0, 0, 0);
                    }
                    break;
                    case LED_FLAG_UNDERGLOW: {
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_disable_noeeprom();
                    }
                    break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
                        rgb_matrix_enable_noeeprom();
                    }
                    break;
                }
            }
            return false;
        case MK_LCTL: // imitate GRAVE when tapped with GUI
            if (!MODS_GUI)
                return true;
            if (record->event.pressed && record->tap.count) {
                register_code(KC_GRAVE);
            } else {
                unregister_code(KC_GRAVE);
            }
            return false;
        case KC_ESC: // imitate GRAVE when pressed with GUI
            if (!MODS_GUI)
                return true;
            if (record->event.pressed) {
                register_code(KC_GRAVE);
            } else {
                unregister_code(KC_GRAVE);
            }
            return false;
        default:
            return true; //Process all other keycodes normally
    }
}
