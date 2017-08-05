

#include "gui/guiapp_resources.h"
#include "gui/guiapp_specifications.h"

#include "main_thread.h"
#include <stdio.h>
#include <stdlib.h>


static bool button_enabled = false;

extern GX_WINDOW_ROOT * p_window_root;

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old);
static void update_text_id(GX_WIDGET * p_widget, GX_RESOURCE_ID id, UINT string_id);

UINT window1_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;
    const uint32_t freq_in_hz = 2;
    const uint32_t delay = bsp_delay_units/freq_in_hz;

    bsp_leds_t leds;

    R_BSP_LedsGet(&leds);

    UINT result = gx_window_event_process(widget, event_ptr);
/** change stuff here */

    if(0 != leds.led_count) {

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_BUTTONENABLER, GX_EVENT_TOGGLE_ON):
        g_ioport.p_api->pinWrite(leds.p_leds[1], IOPORT_LEVEL_HIGH);
        update_text_id(widget->gx_widget_parent, ID_BUTTONENABLER, GX_STRING_ID_REDOFF);
        break;
    case GX_SIGNAL(ID_BUTTONENABLER, GX_EVENT_TOGGLE_OFF):
        update_text_id(widget->gx_widget_parent, ID_BUTTONENABLER, GX_STRING_ID_REDON);
        g_ioport.p_api->pinWrite(leds.p_leds[1], IOPORT_LEVEL_LOW);
        break;
    case GX_SIGNAL(ID_TOGGLEGREEN, GX_EVENT_TOGGLE_ON):
         g_ioport.p_api->pinWrite(leds.p_leds[0], IOPORT_LEVEL_HIGH);
         update_text_id(widget->gx_widget_parent, ID_TOGGLEGREEN, GX_STRING_ID_GREENOFF);
         break;
    case GX_SIGNAL(ID_TOGGLEGREEN, GX_EVENT_TOGGLE_OFF):
         g_ioport.p_api->pinWrite(leds.p_leds[0], IOPORT_LEVEL_LOW);
         update_text_id(widget->gx_widget_parent, ID_TOGGLEGREEN, GX_STRING_ID_GREENON);
         break;
    case GX_SIGNAL(ID_TOGGLEORANGE, GX_EVENT_TOGGLE_ON):
         g_ioport.p_api->pinWrite(leds.p_leds[2], IOPORT_LEVEL_HIGH);
         update_text_id(widget->gx_widget_parent, ID_TOGGLEORANGE, GX_STRING_ID_ORANGEOFF);
         break;
    case GX_SIGNAL(ID_TOGGLEORANGE, GX_EVENT_TOGGLE_OFF):
             g_ioport.p_api->pinWrite(leds.p_leds[2], IOPORT_LEVEL_LOW);
             update_text_id(widget->gx_widget_parent, ID_TOGGLEORANGE, GX_STRING_ID_ORANGEON);
             break;
    default:
        gx_window_event_process(widget, event_ptr);
        break;
    }
    }
    return result;
}

UINT window2_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    UINT result = gx_window_event_process(widget, event_ptr);

    switch (event_ptr->gx_event_type){
        case GX_EVENT_PEN_UP:
            show_window((GX_WINDOW*)&window1, (GX_WIDGET*)widget, true);
            break;
        default:
            result = gx_window_event_process(widget, event_ptr);
            break;
    }
    return result;
}

static UINT show_window(GX_WINDOW * p_new, GX_WIDGET * p_widget, bool detach_old)
{
    UINT err = GX_SUCCESS;

    if (!p_new->gx_widget_parent)
    {
        err = gx_widget_attach(p_window_root, p_new);
    }
    else
    {
        err = gx_widget_show(p_new);
    }

    gx_system_focus_claim(p_new);

    GX_WIDGET * p_old = p_widget;
    if (p_old && detach_old)
    {
        if (p_old != (GX_WIDGET*)p_new)
        {
            gx_widget_detach(p_old);
        }
    }

    return err;
}

static void update_text_id(GX_WIDGET * p_widget, GX_RESOURCE_ID id, UINT string_id)
{
    GX_PROMPT * p_prompt = NULL;

    ssp_err_t err = gx_widget_find(p_widget, (USHORT)id, GX_SEARCH_DEPTH_INFINITE, (GX_WIDGET**)&p_prompt);
    if (TX_SUCCESS == err)
    {
        gx_prompt_text_id_set(p_prompt, string_id);
    }
}

