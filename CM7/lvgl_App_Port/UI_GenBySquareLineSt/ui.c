// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: TestYY

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Arc1;
lv_obj_t * ui_Arc2;
lv_obj_t * ui_Button2;
lv_obj_t * ui_Keyboard1;
lv_obj_t * ui_Button3;
lv_obj_t * ui_Arc3;
lv_obj_t * ui_Image3;
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Button5;
lv_obj_t * ui_Spinner1;
lv_obj_t * ui_Chart1;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "#error LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
static void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(event == LV_EVENT_CLICKED) {
        _ui_flag_modify(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
    }
}
static void ui_event_Keyboard1(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
}
static void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(event == LV_EVENT_PRESSED) {
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
    }
}
static void ui_event_Button5(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(event == LV_EVENT_PRESSED) {
        _ui_screen_change(ui_Screen1, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
    }
}

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void)
{

    // ui_Screen1

    ui_Screen1 = lv_obj_create(NULL);

    lv_obj_set_style_bg_img_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Arc1

    ui_Arc1 = lv_arc_create(ui_Screen1);

    lv_obj_set_width(ui_Arc1, 150);
    lv_obj_set_height(ui_Arc1, 150);

    lv_obj_set_x(ui_Arc1, -99);
    lv_obj_set_y(ui_Arc1, 553);

    lv_obj_set_align(ui_Arc1, LV_ALIGN_CENTER);

    lv_arc_set_range(ui_Arc1, 0, 100);
    lv_arc_set_bg_angles(ui_Arc1, 120, 60);

    // ui_Arc2

    ui_Arc2 = lv_arc_create(ui_Screen1);

    lv_obj_set_width(ui_Arc2, 150);
    lv_obj_set_height(ui_Arc2, 150);

    lv_obj_set_x(ui_Arc2, 139);
    lv_obj_set_y(ui_Arc2, 569);

    lv_obj_set_align(ui_Arc2, LV_ALIGN_CENTER);

    lv_arc_set_range(ui_Arc2, 0, 100);
    lv_arc_set_bg_angles(ui_Arc2, 120, 60);

    // ui_Button2

    ui_Button2 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button2, 100);
    lv_obj_set_height(ui_Button2, 50);

    lv_obj_set_x(ui_Button2, -91);
    lv_obj_set_y(ui_Button2, -155);

    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_ALL, NULL);
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0xDCDCDC), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Keyboard1

    ui_Keyboard1 = lv_keyboard_create(ui_Screen1);

    lv_obj_set_width(ui_Keyboard1, 470);
    lv_obj_set_height(ui_Keyboard1, 298);

    lv_obj_set_x(ui_Keyboard1, -136);
    lv_obj_set_y(ui_Keyboard1, 73);

    lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_Keyboard1, ui_event_Keyboard1, LV_EVENT_ALL, NULL);
    lv_obj_set_style_shadow_color(ui_Keyboard1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_Keyboard1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Button3

    ui_Button3 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button3, 100);
    lv_obj_set_height(ui_Button3, 50);

    lv_obj_set_x(ui_Button3, 317);
    lv_obj_set_y(ui_Button3, -180);

    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_Button3, ui_event_Button3, LV_EVENT_ALL, NULL);

    // ui_Arc3

    ui_Arc3 = lv_arc_create(ui_Screen1);

    lv_obj_set_width(ui_Arc3, 150);
    lv_obj_set_height(ui_Arc3, 150);

    lv_obj_set_x(ui_Arc3, -544);
    lv_obj_set_y(ui_Arc3, 8);

    lv_obj_set_align(ui_Arc3, LV_ALIGN_CENTER);

    lv_arc_set_range(ui_Arc3, 0, 100);
    lv_arc_set_bg_angles(ui_Arc3, 120, 60);

    // ui_Image3

    ui_Image3 = lv_img_create(ui_Screen1);

    lv_obj_set_width(ui_Image3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Image3, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Image3, 251);
    lv_obj_set_y(ui_Image3, 10);

    lv_obj_set_align(ui_Image3, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Image3, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Image3, LV_OBJ_FLAG_SCROLLABLE);

}
void ui_Screen2_screen_init(void)
{

    // ui_Screen2

    ui_Screen2 = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Screen2, LV_OBJ_FLAG_SCROLLABLE);

    // ui_Button5

    ui_Button5 = lv_btn_create(ui_Screen2);

    lv_obj_set_width(ui_Button5, 100);
    lv_obj_set_height(ui_Button5, 50);

    lv_obj_set_x(ui_Button5, 129);
    lv_obj_set_y(ui_Button5, -158);

    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button5, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button5, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_Button5, ui_event_Button5, LV_EVENT_ALL, NULL);

    // ui_Spinner1

    ui_Spinner1 = lv_spinner_create(ui_Screen2, 1000, 90);

    lv_obj_set_width(ui_Spinner1, 387);
    lv_obj_set_height(ui_Spinner1, 354);

    lv_obj_set_x(ui_Spinner1, -143);
    lv_obj_set_y(ui_Spinner1, 20);

    lv_obj_set_align(ui_Spinner1, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_Spinner1, LV_OBJ_FLAG_CLICKABLE);

    // ui_Chart1

    ui_Chart1 = lv_chart_create(ui_Screen2);

    lv_obj_set_width(ui_Chart1, 180);
    lv_obj_set_height(ui_Chart1, 100);

    lv_obj_set_x(ui_Chart1, 178);
    lv_obj_set_y(ui_Chart1, -14);

    lv_obj_set_align(ui_Chart1, LV_ALIGN_CENTER);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    lv_disp_load_scr(ui_Screen1);
}

