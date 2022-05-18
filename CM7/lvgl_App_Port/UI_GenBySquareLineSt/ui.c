// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Button1;
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Button2;
lv_obj_t * ui_Arc1;
lv_obj_t * ui_Arc3;
lv_obj_t * ui_TextArea1;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 32
    #error "LV_COLOR_DEPTH should be 32bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "#error LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
static void ui_event_Button1(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(event == LV_EVENT_PRESSED) {
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0);
    }
}
static void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(event == LV_EVENT_PRESSED) {
        _ui_screen_change(ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 10, 0);
    }
}

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void)
{

    // ui_Screen1

    ui_Screen1 = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_scrollbar_mode(ui_Screen1, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x00FFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Button1

    ui_Button1 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button1, 182);
    lv_obj_set_height(ui_Button1, 96);

    lv_obj_set_x(ui_Button1, -224);
    lv_obj_set_y(ui_Button1, 320);

    lv_obj_set_align(ui_Button1, LV_ALIGN_TOP_MID);

    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);

}
void ui_Screen2_screen_init(void)
{

    // ui_Screen2

    ui_Screen2 = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(ui_Screen2, lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Button2

    ui_Button2 = lv_btn_create(ui_Screen2);

    lv_obj_set_width(ui_Button2, 184);
    lv_obj_set_height(ui_Button2, 118);

    lv_obj_set_x(ui_Button2, 260);
    lv_obj_set_y(ui_Button2, 2);

    lv_obj_set_align(ui_Button2, LV_ALIGN_TOP_MID);

    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_ALL, NULL);
    lv_obj_set_style_bg_color(ui_Button2, lv_color_hex(0x0000FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button2, 200, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Arc1

    ui_Arc1 = lv_arc_create(ui_Screen2);

    lv_obj_set_width(ui_Arc1, 150);
    lv_obj_set_height(ui_Arc1, 150);

    lv_obj_set_x(ui_Arc1, -157);
    lv_obj_set_y(ui_Arc1, 303);

    lv_obj_set_align(ui_Arc1, LV_ALIGN_CENTER);

    lv_arc_set_range(ui_Arc1, 0, 100);
    lv_arc_set_bg_angles(ui_Arc1, 120, 60);

    // ui_Arc3

    ui_Arc3 = lv_arc_create(ui_Screen2);

    lv_obj_set_width(ui_Arc3, 150);
    lv_obj_set_height(ui_Arc3, 150);

    lv_obj_set_x(ui_Arc3, 225);
    lv_obj_set_y(ui_Arc3, 296);

    lv_obj_set_align(ui_Arc3, LV_ALIGN_CENTER);

    lv_arc_set_range(ui_Arc3, 0, 100);
    lv_arc_set_bg_angles(ui_Arc3, 120, 60);

    // ui_TextArea1

    ui_TextArea1 = lv_textarea_create(ui_Screen2);

    lv_obj_set_width(ui_TextArea1, 218);
    lv_obj_set_height(ui_TextArea1, 85);

    lv_obj_set_x(ui_TextArea1, -256);
    lv_obj_set_y(ui_TextArea1, -143);

    lv_obj_set_align(ui_TextArea1, LV_ALIGN_CENTER);

    if("" == "") lv_textarea_set_accepted_chars(ui_TextArea1, NULL);
    else lv_textarea_set_accepted_chars(ui_TextArea1, "");

    lv_textarea_set_text(ui_TextArea1, "");
    lv_textarea_set_placeholder_text(ui_TextArea1, "HELLO LVGL");

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

