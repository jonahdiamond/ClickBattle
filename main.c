// Main file for Click Battle, an ECE243 Project
// by Jonah Diamond & Daniel Neagu
// Created 03/18/24

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define PS2_BASE 0xFF200020;
#define P1INPUT_HEX 0x2D;
#define P2INPUT_HEX 0x32;
#define LED_BASE 0xFF2000000;
#define red 0xf800
#define blue 0x001f

bool click();
bool startScreen();  // title screen
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

void drawCLICKBATTLE();
void deleteCLICKBATTLE();



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
  startScreen();
    wait_for_sync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);//switch back to back buffer 
  volatile int* PS2_ptr = (int*)PS2_BASE;
  volatile int* LED_ptr = (int*)LED_BASE;

  int P1key = P1INPUT_HEX;
  int P2key = P2INPUT_HEX;
  int PS2_data, RVALID;
  int keydata = 0;
  *(PS2_ptr) = 0xFF;  // resets the input

  while (1) {
    while (startScreen()){
        PS2_data = *(PS2_ptr);       // read the Data register in the PS/2 port
        RVALID = PS2_data & 0x8000;  // extract the RVALID field
        if (RVALID) {

        }
    }
    deleteCLICKBATTLE();
    while (/*someting?*/1){

    }    PS2_data = *(PS2_ptr);       // read the Data register in the PS/2 port
        RVALID = PS2_data & 0x8000;  // extract the RVALID field
        if (RVALID) {
        keydata = PS2_data & 0xFF;
        if (keydata == P1key) {
            *(LED_ptr) = 0x1;  // could increment a counter or something instead
        }
        if (keydata == P2key) {
            *(LED_ptr) = 0x10;  // led commands are just here as a place holder
        }
        }
  }

  return 0;
}

bool startScreen() {
    // start screen message
    for (int i = 0; i < 160; i++) {
        for (int j = 0; j < 240; j++) {
            plot_pixel(i, j, blue);
        }
    }
    for (int i = 160; i < 320; i++) {
        for (int j = 0; j < 240; j++) {
            plot_pixel(i, j, red);
        }
    }
    drawCLICKBATTLE();
    int PS2_data, RVALID;
    volatile int* PS2_ptr = (int*)PS2_BASE;

    PS2_data = *(PS2_ptr);       // read the Data register in the PS/2 port
        RVALID = PS2_data & 0x8000;  // extract the RVALID field
        if (RVALID) {

        }
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
    if (x1 == x0){
        x = x0;
        for (int y = y0; y <= y1; y++){
            plot_pixel(x, y, line_color);
        }
        return;
    }
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


void drawCLICKBATTLE(){
    // code for drawing a C
    // top of C
    draw_line(54, 30, 69, 30, 0xffff);
    draw_line(54, 31, 69, 31, 0xffff);
    // upper curve of C
    draw_line(54, 30, 50, 38, 0xffff);
    draw_line(54, 31, 50, 39, 0xffff);
    // left line of C
    draw_line(50, 38, 50, 50, 0xffff);
    draw_line(51, 38, 51, 50, 0xffff);
    // lower curve of C
    draw_line(50, 50, 54, 58, 0xffff);
    draw_line(51, 50, 54, 58, 0xffff);
    // lower line of C
    draw_line(54, 58, 69, 58, 0xffff);
    draw_line(54, 59, 69, 59, 0xffff);

    // code for drawing an L
    // vertical line
    draw_line(74, 30, 74, 59, 0xffff);
    draw_line(75, 30, 75, 59, 0xffff);
    // horizontal line
    draw_line(74, 58, 90, 58, 0xffff);
    draw_line(74, 59, 90, 59, 0xffff);

    // code for drawing an I
    draw_line(95, 30, 95, 59, 0xffff);
    draw_line(96, 30, 96, 59, 0xffff);

    // code for drawing a C
    // top of C
    draw_line(105, 30, 120, 30, 0xffff);
    draw_line(105, 31, 120, 31, 0xffff);
    // upper curve of C
    draw_line(105, 30, 101, 38, 0xffff);
    draw_line(105, 31, 101, 39, 0xffff);
    // left line of C
    draw_line(101, 38, 101, 50, 0xffff);
    draw_line(102, 38, 102, 50, 0xffff);
    // lower curve of C
    draw_line(101, 50, 105, 58, 0xffff);
    draw_line(102, 50, 105, 58, 0xffff);
    // lower line of C
    draw_line(105, 58, 120, 58, 0xffff);
    draw_line(105, 59, 120, 59, 0xffff);

    // code for drawing a K
    // vertical line
    draw_line(125, 30, 125, 59, 0xffff);
    draw_line(126, 30, 126, 59, 0xffff);
    // upper diagonal
    draw_line(125, 45, 135, 30, 0xffff);
    draw_line(126, 45, 136, 30, 0xffff);
    // lower diagonal
    draw_line(126, 45, 136, 59, 0xffff);
    draw_line(125, 45, 135, 59, 0xffff);

    // code for drawing a B
    // vertical
    draw_line(162 + 22, 30, 162 + 22, 59, 0xffff);
    draw_line(163 + 22, 30, 163 + 22, 59, 0xffff);
    // upper curve
    draw_line(162 + 22, 30, 167 + 22, 30, 0xffff);
    draw_line(162 + 22, 31, 167 + 22, 31, 0xffff);
    draw_line(167 + 22, 30, 172 + 22, 36, 0xffff);
    draw_line(167 + 22, 31, 172 + 22, 37, 0xffff);
    draw_line(172 + 22, 36, 172 + 22, 41, 0xffff);
    draw_line(173 + 22, 36, 173 + 22, 41, 0xffff);
    draw_line(171 + 22, 41, 166 + 22, 45, 0xffff);
    draw_line(173 + 22, 41, 167 + 22, 45, 0xffff);
    // lower curve
    draw_line(167 + 22, 45, 172 + 22, 51, 0xffff);
    draw_line(167 + 22, 46, 172 + 22, 52, 0xffff);
    draw_line(172 + 22, 51, 172 + 22, 56, 0xffff);
    draw_line(173 + 22, 51, 173 + 22, 56, 0xffff);
    draw_line(172 + 22, 56, 166 + 22, 59, 0xffff);
    draw_line(173 + 22, 56, 167 + 22, 59, 0xffff);
    draw_line(162 + 22, 58, 167 + 22, 58, 0xffff);
    draw_line(162 + 22, 59, 167 + 22, 59, 0xffff);

    // code for drawing an A
    // left diagonal
    draw_line(177 + 22, 59, 184 + 22, 30, 0xffff);
    draw_line(178 + 22, 59, 185 + 22, 30, 0xffff);
    // right diagonal
    draw_line(191 + 22, 59, 184 + 22, 30, 0xffff);
    draw_line(192 + 22, 59, 185 + 22, 30, 0xffff);
    // middle line
    draw_line(181 + 22, 44, 188 + 22, 44, 0xffff);
    draw_line(181 + 22, 45, 188 + 22, 45, 0xffff);

    // code for drawing a T
    // horizontal
    draw_line(195 + 22, 30, 205 + 22, 30, 0xffff);
    draw_line(195 + 22, 31, 205 + 22, 31, 0xffff);
    // vertical
    draw_line(200 + 22, 30, 200 + 22, 59, 0xffff);
    draw_line(201 + 22, 30, 201 + 22, 59, 0xffff);

    // code for drawing a T
    // horizontal
    draw_line(210 + 22, 30, 220 + 22, 30, 0xffff);
    draw_line(210 + 22, 31, 220 + 22, 31, 0xffff);
    // vertical
    draw_line(215 + 22, 30, 215 + 22, 59, 0xffff);
    draw_line(216 + 22, 30, 216 + 22, 59, 0xffff);

    // code for drawing an L
    // vertical line
    draw_line(225 + 22, 30, 225 + 22, 59, 0xffff);
    draw_line(226 + 22, 30, 226 + 22, 59, 0xffff);
    // horizontal line
    draw_line(225 + 22, 58, 241 + 22, 58, 0xffff);
    draw_line(225 + 22, 59, 241 + 22, 59, 0xffff);

    // code for drawing an E
    // vertical line
    draw_line(246 + 22, 30, 246 + 22, 59, 0xffff);
    draw_line(247 + 22, 30, 247 + 22, 59, 0xffff);
    // bottom line
    draw_line(246 + 22, 58, 262 + 22, 58, 0xffff);
    draw_line(246 + 22, 59, 262 + 22, 59, 0xffff);
    // top line
    draw_line(246 + 22, 30, 262 + 22, 30, 0xffff);
    draw_line(246 + 22, 31, 262 + 22, 31, 0xffff);
    // middle line
    draw_line(246 + 22, 44, 262 + 22, 44, 0xffff);
    draw_line(246 + 22, 45, 262 + 22, 45, 0xffff);
}

void deleteCLICKBATTLE(){
    // code for drawing a C
    // top of C
    draw_line(54, 30, 69, 30, red);
    draw_line(54, 31, 69, 31, red);
    // upper curve of C
    draw_line(54, 30, 50, 38, red);
    draw_line(54, 31, 50, 39, red);
    // left line of C
    draw_line(50, 38, 50, 50, red);
    draw_line(51, 38, 51, 50, red);
    // lower curve of C
    draw_line(50, 50, 54, 58, red);
    draw_line(51, 50, 54, 58, red);
    // lower line of C
    draw_line(54, 58, 69, 58, red);
    draw_line(54, 59, 69, 59, red);

    // code for drawing an L
    // vertical line
    draw_line(74, 30, 74, 59, red);
    draw_line(75, 30, 75, 59, red);
    // horizontal line
    draw_line(74, 58, 90, 58, red);
    draw_line(74, 59, 90, 59, red);

    // code for drawing an I
    draw_line(95, 30, 95, 59, red);
    draw_line(96, 30, 96, 59, red);

    // code for drawing a C
    // top of C
    draw_line(105, 30, 120, 30, red);
    draw_line(105, 31, 120, 31, red);
    // upper curve of C
    draw_line(105, 30, 101, 38, red);
    draw_line(105, 31, 101, 39, red);
    // left line of C
    draw_line(101, 38, 101, 50, red);
    draw_line(102, 38, 102, 50, red);
    // lower curve of C
    draw_line(101, 50, 105, 58, red);
    draw_line(102, 50, 105, 58, red);
    // lower line of C
    draw_line(105, 58, 120, 58, red);
    draw_line(105, 59, 120, 59, red);

    // code for drawing a K
    // vertical line
    draw_line(125, 30, 125, 59, red);
    draw_line(126, 30, 126, 59, red);
    // upper diagonal
    draw_line(125, 45, 135, 30, red);
    draw_line(126, 45, 136, 30, red);
    // lower diagonal
    draw_line(126, 45, 136, 59, red);
    draw_line(125, 45, 135, 59, red);

    // code for drawing a B
    // vertical
    draw_line(162 + 22, 30, 162 + 22, 59, blue);
    draw_line(163 + 22, 30, 163 + 22, 59, blue);
    // upper curve
    draw_line(162 + 22, 30, 167 + 22, 30, blue);
    draw_line(162 + 22, 31, 167 + 22, 31, blue);
    draw_line(167 + 22, 30, 172 + 22, 36, blue);
    draw_line(167 + 22, 31, 172 + 22, 37, blue);
    draw_line(172 + 22, 36, 172 + 22, 41, blue);
    draw_line(173 + 22, 36, 173 + 22, 41, blue);
    draw_line(171 + 22, 41, 166 + 22, 45, blue);
    draw_line(173 + 22, 41, 167 + 22, 45, blue);
    // lower curve
    draw_line(167 + 22, 45, 172 + 22, 51, blue);
    draw_line(167 + 22, 46, 172 + 22, 52, blue);
    draw_line(172 + 22, 51, 172 + 22, 56, blue);
    draw_line(173 + 22, 51, 173 + 22, 56, blue);
    draw_line(172 + 22, 56, 166 + 22, 59, blue);
    draw_line(173 + 22, 56, 167 + 22, 59, blue);
    draw_line(162 + 22, 58, 167 + 22, 58, blue);
    draw_line(162 + 22, 59, 167 + 22, 59, blue);

    // code for drawing an A
    // left diagonal
    draw_line(177 + 22, 59, 184 + 22, 30, blue);
    draw_line(178 + 22, 59, 185 + 22, 30, blue);
    // right diagonal
    draw_line(191 + 22, 59, 184 + 22, 30, blue);
    draw_line(192 + 22, 59, 185 + 22, 30, blue);
    // middle line
    draw_line(181 + 22, 44, 188 + 22, 44, blue);
    draw_line(181 + 22, 45, 188 + 22, 45, blue);

    // code for drawing a T
    // horizontal
    draw_line(195 + 22, 30, 205 + 22, 30, blue);
    draw_line(195 + 22, 31, 205 + 22, 31, blue);
    // vertical
    draw_line(200 + 22, 30, 200 + 22, 59, blue);
    draw_line(201 + 22, 30, 201 + 22, 59, blue);

    // code for drawing a T
    // horizontal
    draw_line(210 + 22, 30, 220 + 22, 30, blue);
    draw_line(210 + 22, 31, 220 + 22, 31, blue);
    // vertical
    draw_line(215 + 22, 30, 215 + 22, 59, blue);
    draw_line(216 + 22, 30, 216 + 22, 59, blue);

    // code for drawing an L
    // vertical line
    draw_line(225 + 22, 30, 225 + 22, 59, blue);
    draw_line(226 + 22, 30, 226 + 22, 59, blue);
    // horizontal line
    draw_line(225 + 22, 58, 241 + 22, 58, blue);
    draw_line(225 + 22, 59, 241 + 22, 59, blue);

    // code for drawing an E
    // vertical line
    draw_line(246 + 22, 30, 246 + 22, 59, blue);
    draw_line(247 + 22, 30, 247 + 22, 59, blue);
    // bottom line
    draw_line(246 + 22, 58, 262 + 22, 58, blue);
    draw_line(246 + 22, 59, 262 + 22, 59, blue);
    // top line
    draw_line(246 + 22, 30, 262 + 22, 30, blue);
    draw_line(246 + 22, 31, 262 + 22, 31, blue);
    // middle line
    draw_line(246 + 22, 44, 262 + 22, 44, blue);
    draw_line(246 + 22, 45, 262 + 22, 45, blue);
}