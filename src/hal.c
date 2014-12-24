/*
This file present all abstraction needed to port LiteNES.
  (The current working implementation uses allegro library.)

To port this project, replace the following functions by your own:
1) nes_hal_init()
    Do essential initialization work, including starting a FPS HZ timer.

2) nes_set_bg_color(c)
    Set the back ground color to be the NES internal color code c.

3) nes_flush_buf(*buf)
    Flush the entire pixel buf's data to frame buffer.

4) nes_flip_display()
    Fill the screen with previously set background color, and
    display all contents in the frame buffer.

5) wait_for_frame()
    Implement it to make the following code is executed FPS times a second:
        while (1) {
            wait_for_frame();
            do_something();
        }

6) int nes_key_state(int b) 
    Query button b's state (1 to be pressed, otherwise 0).
    The correspondence of b and the buttons:
      0 - Power
      1 - A
      2 - B
      3 - SELECT
      4 - START
      5 - UP
      6 - DOWN
      7 - LEFT
      8 - RIGHT
*/
#include "hal.h"
#include "fce.h"
#include "common.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_EVENT_QUEUE *fce_event_queue;
ALLEGRO_TIMER *fce_timer = NULL;
static ALLEGRO_VERTEX vtx[1000000];
ALLEGRO_COLOR color_map[64];
int vtx_sz = 0;
  
/* Wait until next allegro timer event is fired. */
void wait_for_frame()
{
    while (1)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(fce_event_queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) break;
    }
}

/* Set background color. RGB value of c is defined in fce.h */
void nes_set_bg_color(int c)
{
    al_clear_to_color(color_map[c]);
}

/* Flush the pixel buffer */
void nes_flush_buf(PixelBuf *buf) {
    int i;
    for (i = 0; i < buf->size; i ++) {
        Pixel *p = &buf->buf[i];
        int x = p->x, y = p->y;
        ALLEGRO_COLOR c = color_map[p->c];

        vtx[vtx_sz].x = x*2; vtx[vtx_sz].y = y*2;
        vtx[vtx_sz ++].color = c;
        vtx[vtx_sz].x = x*2+1; vtx[vtx_sz].y = y*2;
        vtx[vtx_sz ++].color = c;
        vtx[vtx_sz].x = x*2; vtx[vtx_sz].y = y*2+1;
        vtx[vtx_sz ++].color = c;
        vtx[vtx_sz].x = x*2+1; vtx[vtx_sz].y = y*2+1;
        vtx[vtx_sz ++].color = c;
    }
}

/* Initialization:
   (1) start a 1/FPS Hz timer. 
   (2) register fce_timer handle on each timer event */
void nes_hal_init()
{
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_create_display(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);

    fce_timer = al_create_timer(1.0 / FPS);
    fce_event_queue = al_create_event_queue();
    al_register_event_source(fce_event_queue, al_get_timer_event_source(fce_timer));
    al_start_timer(fce_timer);
}

/* Update screen at FPS rate by allegro's drawing function. 
   Timer ensures this function is called FPS times a second. */
void nes_flip_display()
{
    al_draw_prim(vtx, NULL, NULL, 0, vtx_sz, ALLEGRO_PRIM_POINT_LIST);
    al_flip_display();
    vtx_sz = 0;
    int i;
    for (i = 0; i < 64; i ++) {
        pal color = palette[i];
        color_map[i] = al_map_rgb(color.r, color.g, color.b);
    }
}

/* Query a button's state.
   Returns 1 if button #b is pressed. */
int nes_key_state(int b)
{
    ALLEGRO_KEYBOARD_STATE state;
    al_get_keyboard_state(&state);
    switch (b)
    {
        case 0: // On / Off
            return 1;
        case 1: // A
            return al_key_down(&state, ALLEGRO_KEY_K);
        case 2: // B
            return al_key_down(&state, ALLEGRO_KEY_J);
        case 3: // SELECT
            return al_key_down(&state, ALLEGRO_KEY_U);
        case 4: // START
            return al_key_down(&state, ALLEGRO_KEY_I);
        case 5: // UP
            return al_key_down(&state, ALLEGRO_KEY_W);
        case 6: // DOWN
            return al_key_down(&state, ALLEGRO_KEY_S);
        case 7: // LEFT
            return al_key_down(&state, ALLEGRO_KEY_A);
        case 8: // RIGHT
            return al_key_down(&state, ALLEGRO_KEY_D);
        default:
            return 1;
    }
}

