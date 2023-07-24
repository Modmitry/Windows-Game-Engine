#pragma once
#include <stdint.h>

#define WIDTH_SCREEN 1024
#define HEIGHT_SCREEN 768

// backbuffer
extern uint32_t buff[HEIGHT_SCREEN][WIDTH_SCREEN];

#ifndef VK_ESCAPE
#  define VK_ESCAPE 0x1B
#  define VK_SPACE  0x20
#  define VK_LEFT   0x25
#  define VK_UP     0x26
#  define VK_RIGHT  0x27
#  define VK_DOWN   0x28
#  define VK_RETURN 0x0D
#endif

// VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, ...
bool is_key_pressed(int button_vk_code);

// 0 - left button, 1 - right button
bool is_mouse_button_pressed(int button);

// cursor pos
int get_cursor_x();
int get_cursor_y();

bool is_window_active();

void clear_buffer();

void Init();
void Finish();
void Act(float dt);
void Draw();
void Quit();
