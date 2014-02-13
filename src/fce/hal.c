/*
HAL是硬件抽象层(hardware abstract layer)。
**所有**与平台相关的代码都集中在这里。
在这个实现中，我们用到了allegro游戏开发库绘制图形和监听键盘事件。

在这里实现以下抽象函数：
1) nes_hal_init()
    HAL的初始化。这个函数会在模拟器完成初始化后调用。在这里实现模拟器的
    初始化工作
2) nes_set_bg_color(r, g, b, c)
    设置背景色，r, g, b分别是红、绿、蓝分量的数值(0-255)。
    c是54色表示的内部代码，可以考虑使用
3) nes_draw_pixel(*p)
    在视频缓冲区中绘制Pixel p，其p->x, p->y是点坐标(左上角为原点，横边
    为x轴)。
    r, g, b, c为颜色信息。
4) nes_flip_display()
    将视频缓冲区内的内容显示到屏幕上，然后用设置好的背景色填充视频缓冲区。
5) wait_for_frame()
    你首先需要设置一个1/FPS (FPS=60)s的定时器。
    然后，模拟器会不断执行wait_for_frame，每一帧执行、绘制完毕后，会调用
    wait_for_frame()，在这个函数里你需要等待直到下次定时器事件触发。
    换句话说，你需要保证下面程序里的do_something()以每秒60次的频率执行：
    while (1) {
        wait_for_frame();
        do_something();
    }
6) int nes_key_state(int b) 
    询问按键状态，返回0代表松开，1代表按下。不同的b代表询问不同的按键：
    0 - Power，总是为1
    1 - 手柄上的A键
    2 - 手柄上的B键
    3 - 手柄上的SELECT键
    4 - 手柄上的START键
    5 - 手柄上的UP键
    6 - 手柄上的DOWN键
    7 - 手柄上的LEFT键
    8 - 手柄上的RIGHT键
*/
#include "nes/hal.h"
#include "nes/fce.h"
#include "nes/common.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_EVENT_QUEUE *fce_event_queue;
ALLEGRO_TIMER *fce_timer = NULL;
static ALLEGRO_VERTEX vtx[1000000];
int vtx_sz = 0;

  
/* 控制时间的函数，函数每次完成计算后会被调用，保证每秒执行FPS次 */
void wait_for_frame()
{
    while (1)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(fce_event_queue, &event);
        if (event.type == ALLEGRO_EVENT_TIMER) break;
    }
    asm volatile ("nop");
}

/* 设置背景色为(r, g, b)，等价于NES内部颜色代码c
   如果不设置，Super Mario将看到黑色而不是蓝色的背景 */
void nes_set_bg_color(int r, int g, int b, int c)
{
    al_clear_to_color(al_map_rgb(r, g, b));
}

/* 在屏幕上绘制一个像素点
   但在这个实现里，我们只是保存这个像素点，在flip_display时统一绘制 */
void nes_draw_pixel(Pixel *p)
{
      ALLEGRO_VERTEX px;
    px.x = p->x;
    px.y = p->y;
    px.z = 0;
    px.color = al_map_rgb(p->r, p->g, p->b);
    memcpy(&vtx[vtx_sz++], &px, sizeof(ALLEGRO_VERTEX));
}

/* 初始化一切需要的内容。
   在这里主要是调用allegro库，初始化定时器、键盘和屏幕 */
void nes_hal_init()
{
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    fce_timer = al_create_timer(1.0 / FPS);
    fce_event_queue = al_create_event_queue();
    al_register_event_source(fce_event_queue, al_get_timer_event_source(fce_timer));
    al_start_timer(fce_timer);
}

/* 这个函数每秒会被调用FPS次，每次调用时更新屏幕内容 */
void nes_flip_display()
{
    al_draw_prim(vtx, NULL, NULL, 0, vtx_sz, ALLEGRO_PRIM_POINT_LIST);
     al_flip_display();
    vtx_sz = 0;
}

/* 询问编码为b的按键是否被按下(返回1为按下) */
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

