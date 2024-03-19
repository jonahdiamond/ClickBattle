// Main file for Click Battle, an ECE243 Project
// by Jonah Diamond & Daniel Neagu
// Created 03/18/24

#include <stdbool.h>
#include <stdio.h>

bool click();
bool start();

int main(void) {
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
  // start screen function
}
