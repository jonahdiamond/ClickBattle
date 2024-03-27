// Click Battle, an ECE243 Project
// by Jonah Diamond & Daniel Neagu
// Created 03/18/24

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define PS2_BASE 0xFF200020;
#define R_INPUT_HEX 0x2D;  // R
#define B_INPUT_HEX 0x32;  // B
#define LED_BASE 0xFF2000000;
#define red 0xf800
#define blue 0x001f
#define SCREENSTART 0
#define SCREENEND 319
#define CLICK_INCREMENT 32

bool click();
// bool startScreen();  // title screen
void startScreen();
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

int gameLoc;

void drawCLICKBATTLE();
void deleteCLICKBATTLE();

void drawBToStart();
void deleteBToStart();
void drawReadyB();
void deleteReadyR();
void drawRToStart();
void deleteRToStart();
void drawReadyR();
void deleteReadyB();
void drawBWins();
void drawRWins();

const short int CLICKBATTLE[1104];
const short int PRESSRTOREADYr[360];
const short int PRESSBTOREADYb[360];
const short int READYr[140];
const short int READYb[140];
const short int RWINSr[140];
const short int BWINSb[140];
int main(void) {
    volatile int* pixel_ctrl_ptr = (int*)0xFF203020;
    *(pixel_ctrl_ptr + 1) = (int)&Buffer1;
    wait_for_sync();  // waits for vsync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    clear_screen();
    startScreen();
    *(pixel_ctrl_ptr + 1) = (int)&Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // we draw on the back buffer
    clear_screen();  // pixel_buffer_start points to the pixel buffer
    startScreen();

    wait_for_sync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // switch back to back buffer
    volatile int* PS2_ptr = (int*)PS2_BASE;
    volatile int* LED_ptr = (int*)LED_BASE;

    int Rkey = R_INPUT_HEX;
    int Bkey = B_INPUT_HEX;

    int xWins = 146, yWins = 120, dx, dy;

    int PS2_data, RVALID;
    int keydata = 0;
    *(PS2_ptr) = 0xFF;  // resets the input

    volatile int* KEYs = (int*)0xff200050;
  	volatile int *LEDs = (int *)0xff200000;

    int edgeCap;
    gameLoc = (SCREENSTART+SCREENEND)/2;
    printf(gameLoc);
    while (1) {
       //while (gameLoc >= SCREENSTART && gameLoc <= SCREENEND) {
        //   PS2_data = *(PS2_ptr);       // read the Data register in the PS/2 port
        //   RVALID = PS2_data & 0x8000;  // extract the RVALID field
            printf(gameLoc);

            edgeCap = *(KEYs + 3);	
            if (edgeCap == 1){
                *LEDs = 0x3ff;				//all LEDs -> 1
                for (int newPixel = gameLoc + CLICK_INCREMENT; newPixel < gameLoc;
                newPixel++) {
                    for (int y = 0; y < 240; y++) {
                        plot_pixel(newPixel, y, red);
                    }
                }
                gameLoc += CLICK_INCREMENT;
                wait_for_sync();
                pixel_buffer_start = *(pixel_ctrl_ptr + 1);
            } else if (edgeCap == 2){
                *LEDs = 0x0;
                for (int newPixel = gameLoc - CLICK_INCREMENT; newPixel < gameLoc;
                newPixel++) {
                    for (int y = 0; y < 240; y++) {
                        plot_pixel(newPixel, y, blue);
                    }
                }
                gameLoc -= CLICK_INCREMENT;
                wait_for_sync();
                pixel_buffer_start = *(pixel_ctrl_ptr + 1);
            }
            *(KEYs + 3) = 1;	//reset edge capture register
            
            
        //}
        // wait_for_sync();
        // pixel_buffer_start = *(pixel_ctrl_ptr + 1);  // switch back to back buffer
    }
    // post game message of who won
    startScreen();
    wait_for_sync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    // poll for space bar to play again 
    return 0;
}

 

void startScreen() {
  for (int i = 0; i < 160; i++) {
    for (int j = 0; j < 240; j++) {
      plot_pixel(i, j, red);
    }
  }
  for (int i = 160; i < 320; i++) {
    for (int j = 0; j < 240; j++) {
      plot_pixel(i, j, blue);
    }
  }
}

// bool startScreen() {
//   // start screen message
//   for (int i = 0; i < 160; i++) {
//     for (int j = 0; j < 240; j++) {
//       plot_pixel(i, j, red);
//     }
//   }
//   for (int i = 160; i < 320; i++) {
//     for (int j = 0; j < 240; j++) {
//       plot_pixel(i, j, blue);
//     }
//   }
//   drawCLICKBATTLE();
//   int PS2_data, RVALID;
//   volatile int* PS2_ptr = (int*)PS2_BASE;

//   PS2_data = *(PS2_ptr);       // read the Data register in the PS/2 port
//   RVALID = PS2_data & 0x8000;  // extract the RVALID field
//   if (RVALID) {
//   }
// }

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
  if (x1 == x0) {
    x = x0;
    for (int y = y0; y <= y1; y++) {
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
