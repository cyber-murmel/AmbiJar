#include <stdint.h>
#include <util/delay.h>
#include <string.h>
#include "soft_spi.h"
#include "color.h"

#define NUM_OF_LED (8)
#define FADE_STEPS (512)
#define FADE_DELAY (0)
#define TIME_DELAY (5000)
#define GLOB_SATURA (100)
#define GLOB_BRIGHT (100)
#include <stdlib.h>

uint16_t old_hue[NUM_OF_LED];// = {0};
color_t  cur_col[NUM_OF_LED];// = {0};
uint16_t new_hue[NUM_OF_LED];// = {0};

// uint16_t hue = 0;

inline void fill_new_hue() {
  for(uint16_t i = 0; i < NUM_OF_LED; i++){
    new_hue[i] = rand()%HUE_MAX;
  }
}

inline void render_cur_col(unsigned int cur_step) {
//  if (cur_step > FADE_STEPS) {
//    cur_step = FADE_STEPS;
//  }
  for(int i = 0; i < NUM_OF_LED; i++){
    for(uint8_t j = 0; j < sizeof(cur_col[0].u8)/sizeof(cur_col[0].u8[0]); j++){
      hsb_to_color(inter_hue(old_hue[i], new_hue[i], FADE_STEPS, cur_step),
                   GLOB_SATURA, GLOB_BRIGHT, &cur_col[i]);
    }
  }
}

int main (void) {
    spi_init();
    while(1){
        memcpy(old_hue, new_hue, NUM_OF_LED*sizeof(old_hue[0]));
        fill_new_hue();
        for(int i = 0; i < FADE_STEPS; i++){
            render_cur_col(i);
            spi_send_buf(cur_col, NUM_OF_LED*sizeof(cur_col[0]));
            _delay_ms (FADE_DELAY);
        }
        //_delay_ms(TIME_DELAY);
    }
    return 0;
}
