// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: TestYY

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Arc3;
lv_obj_t * ui_Slider1;
lv_obj_t * ui_Switch1;
lv_obj_t * ui_Dropdown1;
lv_obj_t * ui_Chart1;
lv_obj_t * ui_Spinner1;
lv_obj_t * ui_Bar1;
lv_obj_t * ui_Panel1;
lv_obj_t * ui_Screen1;
lv_obj_t * ui_TextArea1;
lv_obj_t * ui_Keyboard1;
lv_obj_t * ui_Button1;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 32
    #error "LV_COLOR_DEPTH should be 32bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "#error LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
static void ui_event_Panel1(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(event == LV_EVENT_PRESSED) {
        _ui_screen_change(ui_Screen1, LV_SCR_LOAD_ANIM_FADE_ON, 1, 0);
    }
}
static void ui_event_Button1(lv_event_t * e)
{
    lv_event_code_t event = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(event == LV_EVENT_PRESSED) {
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 1, 0);
    }
}

///////////////////// SCREENS ////////////////////
void ui_Screen2_screen_init(void)
{

    // ui_Screen2

    ui_Screen2 = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(ui_Screen2, lv_color_hex(0x7C3333), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Arc3

    ui_Arc3 = lv_arc_create(ui_Screen2);

    lv_obj_set_width(ui_Arc3, 150);
    lv_obj_set_height(ui_Arc3, 150);

    lv_obj_set_x(ui_Arc3, -291);
    lv_obj_set_y(ui_Arc3, -129);

    lv_obj_set_align(ui_Arc3, LV_ALIGN_CENTER);

    lv_arc_set_range(ui_Arc3, 0, 100);
    lv_arc_set_bg_angles(ui_Arc3, 120, 60);

    // ui_Slider1

    ui_Slider1 = lv_slider_create(ui_Screen2);
    lv_slider_set_range(ui_Slider1, 0, 100);

    lv_obj_set_width(ui_Slider1, 150);
    lv_obj_set_height(ui_Slider1, 10);

    lv_obj_set_x(ui_Slider1, -198);
    lv_obj_set_y(ui_Slider1, 173);

    lv_obj_set_align(ui_Slider1, LV_ALIGN_CENTER);

    // ui_Switch1

    ui_Switch1 = lv_switch_create(ui_Screen2);

    lv_obj_set_width(ui_Switch1, 50);
    lv_obj_set_height(ui_Switch1, 25);

    lv_obj_set_x(ui_Switch1, -26);
    lv_obj_set_y(ui_Switch1, 168);

    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);

    // ui_Dropdown1

    ui_Dropdown1 = lv_dropdown_create(ui_Screen2);
    lv_dropdown_set_options(ui_Dropdown1, "Option 1\nOption 2\nOption 3");
    lv_dropdown_set_text(ui_Dropdown1, "");

    lv_obj_set_width(ui_Dropdown1, 150);
    lv_obj_set_height(ui_Dropdown1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_Dropdown1, 224);
    lv_obj_set_y(ui_Dropdown1, 129);

    lv_obj_set_align(ui_Dropdown1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Dropdown1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    // ui_Chart1

    ui_Chart1 = lv_chart_create(ui_Screen2);

    lv_obj_set_width(ui_Chart1, 180);
    lv_obj_set_height(ui_Chart1, 100);

    lv_obj_set_x(ui_Chart1, -84);
    lv_obj_set_y(ui_Chart1, 41);

    lv_obj_set_align(ui_Chart1, LV_ALIGN_CENTER);

    // ui_Spinner1

    ui_Spinner1 = lv_spinner_create(ui_Screen2, 1000, 90);

    lv_obj_set_width(ui_Spinner1, 80);
    lv_obj_set_height(ui_Spinner1, 80);

    lv_obj_set_x(ui_Spinner1, 89);
    lv_obj_set_y(ui_Spinner1, 100);

    lv_obj_set_align(ui_Spinner1, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_Spinner1, LV_OBJ_FLAG_CLICKABLE);

    // ui_Bar1

    ui_Bar1 = lv_bar_create(ui_Screen2);
    lv_bar_set_range(ui_Bar1, 0, 100);
    lv_bar_set_value(ui_Bar1, 25, LV_ANIM_OFF);

    lv_obj_set_width(ui_Bar1, 150);
    lv_obj_set_height(ui_Bar1, 10);

    lv_obj_set_x(ui_Bar1, 199);
    lv_obj_set_y(ui_Bar1, -12);

    lv_obj_set_align(ui_Bar1, LV_ALIGN_CENTER);

    // ui_Panel1

    ui_Panel1 = lv_obj_create(ui_Screen2);

    lv_obj_set_width(ui_Panel1, 225);
    lv_obj_set_height(ui_Panel1, 105);

    lv_obj_set_x(ui_Panel1, 193);
    lv_obj_set_y(ui_Panel1, -158);

    lv_obj_set_align(ui_Panel1, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_Panel1, ui_event_Panel1, LV_EVENT_ALL, NULL);
    lv_obj_set_style_bg_color(ui_Panel1, lv_color_hex(0x131091), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

}
void ui_Screen1_screen_init(void)
{

    // ui_Screen1

    ui_Screen1 = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x332FBB), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_TextArea1

    ui_TextArea1 = lv_textarea_create(ui_Screen1);

    lv_obj_set_width(ui_TextArea1, 478);
    lv_obj_set_height(ui_TextArea1, 70);

    lv_obj_set_x(ui_TextArea1, -126);
    lv_obj_set_y(ui_TextArea1, -184);

    lv_obj_set_align(ui_TextArea1, LV_ALIGN_CENTER);

    if("" == "") lv_textarea_set_accepted_chars(ui_TextArea1, NULL);
    else lv_textarea_set_accepted_chars(ui_TextArea1, "");

    lv_textarea_set_text(ui_TextArea1, "");
    lv_textarea_set_placeholder_text(ui_TextArea1, "Placeholder...");

    // ui_Keyboard1

    ui_Keyboard1 = lv_keyboard_create(ui_Screen1);

    lv_obj_set_width(ui_Keyboard1, 775);
    lv_obj_set_height(ui_Keyboard1, 309);

    lv_obj_set_x(ui_Keyboard1, -1);
    lv_obj_set_y(ui_Keyboard1, 51);

    lv_obj_set_align(ui_Keyboard1, LV_ALIGN_CENTER);

    // ui_Button1

    ui_Button1 = lv_btn_create(ui_Screen1);

    lv_obj_set_width(ui_Button1, 196);
    lv_obj_set_height(ui_Button1, 93);

    lv_obj_set_x(ui_Button1, 264);
    lv_obj_set_y(ui_Button1, -182);

    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);

    // POST CALLS
    lv_keyboard_set_textarea(ui_Keyboard1, ui_TextArea1);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen2_screen_init();
    ui_Screen1_screen_init();
    lv_disp_load_scr(ui_Screen2);
}

