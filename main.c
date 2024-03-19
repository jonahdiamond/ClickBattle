// Main file for Click Battle, an ECE243 Project
// by Jonah Diamond & Daniel Neagu
// Created 03/18/24

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool click();
bool start();  // title screen
bool game();

void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void swap(int* x, int* y);
int abs(int x);
void draw_line(int x0, int y0, int x1, int y1, short int line_color);
void wait_for_sync();
void draw_box(int x, int y, int width, short int colour);
// header methods
int pixel_buffer_start;       // global variable
short int Buffer1[240][512];  // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];

int main(void) {
  volatile int* pixel_ctrl_ptr =
      (int*)0xFF203020;  // obtains the pointer to the vga controller
  *(pixel_ctrl_ptr + 1) =
      (int)&Buffer1;  // set the back buffer to the address of the buffer array
  wait_for_sync();    // waits for vsync
  pixel_buffer_start = *pixel_ctrl_ptr;        // points to buffer 1
  clear_screen();                              // clears buffer
  *(pixel_ctrl_ptr + 1) = (int)&Buffer2;       // sets the back buffer to buffer
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // points to buffer 2 back
  //*(pixel_ctrl_ptr+1) = *pixel_ctrl_ptr;
  clear_screen();  // clears buffer

  while (1) {
    while (!start()) {
    }
    while (!game()) {
    }
  }

  return 0;
}

bool click() {
  if (true) {
    return false;
  }
  return true;
}

bool start() {
  // start screen message
  for (int i = 0; i < 160; i++) {
    for (int j = 0; j < 240; j++) {
      plot_pixel(i, j, 0x001f);
    }
  }
  for (int i = 160; i < 320; i++) {
    for (int j = 0; j < 240; j++) {
      plot_pixel(i, j, 0xf800);
    }
  }
  // code for drawing a C
  // top of C
  draw_line(54, 30, 69, 30, 0xffff);
  draw_line(54, 31, 69, 31, 0xffff);
  // upper curve of C
  draw_line(54, 30, 50, 38, 0xffff);
  draw_line(54, 31, 50, 39, 0xffff);
  draw_line(50, 38, 50, 50, 0xffff);
  draw_line(51, 38, 51, 50, 0xffff);
  draw_line(50, 50, 54, 58, 0xffff);
  draw_line(51, 50, 54, 58, 0xffff);
  draw_line(54, 58, 69, 58, 0xffff);
  draw_line(54, 59, 69, 59, 0xffff);
}

bool game() {
  // game logic
}

void draw_box(int x, int y, int width, short int colour) {
  for (int i = 0; i < width; i++) {
    draw_line(x, y + i, x + width, y + i, colour);
  }
}
void wait_for_sync() {  // waits for V-sync and rendering to finish
  volatile int* pixel_ctrl_ptr = (int*)0xFF203020;
  int status;
  *pixel_ctrl_ptr = 1;
  status = *(pixel_ctrl_ptr + 3);
  while ((status & 0x01) != 0) {
    status = *(pixel_ctrl_ptr + 3);
  }
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color) {
  int x, y;

  int is_steep = abs(y1 - y0) / abs(x1 - x0);
  if (is_steep > 1) {
    swap(&x0, &y0);
    swap(&x1, &y1);
  }
  if (x0 > x1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
  }

  for (x = x0; x <= x1; x++) {
    y = y0 + (10 * (y1 - y0) * (x - x0) / (x1 - x0) + 5) / 10;
    if (is_steep > 1) {
      plot_pixel(y, x, line_color);
    } else {
      plot_pixel(x, y, line_color);
    }
  }
}

void swap(int* x, int* y) {
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

int abs(int x) {
  if (x < 0) {
    return x = x * -1;
  } else {
    return x;
  }
}

void plot_pixel(int x, int y, short int line_color) {
  volatile short int* one_pixel_address =
      pixel_buffer_start + (y << 10) + (x << 1);
  *one_pixel_address = line_color;
}

void clear_screen() {
  int x, y;
  for (x = 0; x < 320; x++) {
    for (y = 0; y < 240; y++) {
      plot_pixel(x, y, 0);
    }
  }
}
