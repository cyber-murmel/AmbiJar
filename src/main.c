#define NUM_OF_LED (25)
#define FADE_STEPS (128)
#define FADE_DELAY (20)
#define BRIGHT_MIN (0)
#define BRIGHT_MAX (256)
#define RAND_MAX (1<<8)

#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include "soft_spi.h"

typedef struct color{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} color_t;

color_t old_buf[NUM_OF_LED] = {0};
color_t cur_buf[NUM_OF_LED] = {0};
color_t new_buf[NUM_OF_LED] = {0};

enum div_pal {
  a = 1,
  b = 4,
  c = 16
};

const uint8_t div_arr [6][3] = {
  {a,b,c},
  {a,c,b},
  {b,a,c},
  {b,c,a},
  {c,a,b},
  {c,b,a},
};

inline void fill_new_buf() {
  for(int i = 0; i < NUM_OF_LED; i++){
    int div_idx = rand()%sizeof(div_arr)/sizeof(div_arr[0]);
    new_buf[i] = (color_t)
      {
        .r = (BRIGHT_MIN + (rand() % BRIGHT_MAX))/div_arr[div_idx][0],
        .g = (BRIGHT_MIN + (rand() % BRIGHT_MAX))/div_arr[div_idx][1],
        .b = (BRIGHT_MIN + (rand() % BRIGHT_MAX))/div_arr[div_idx][2]
      };
  }
}

inline void render_cur_buf(unsigned int cur_step) {
  if (cur_step > FADE_STEPS) {
    cur_step = FADE_STEPS;
  }
  for(int i = 0; i < NUM_OF_LED; i++){
    int r_curr = (((new_buf[i].r - old_buf[i].r)*cur_step)/FADE_STEPS)+old_buf[i].r;
    int g_curr = (((new_buf[i].g - old_buf[i].g)*cur_step)/FADE_STEPS)+old_buf[i].g;
    int b_curr = (((new_buf[i].b - old_buf[i].b)*cur_step)/FADE_STEPS)+old_buf[i].b;
    cur_buf[i] = (color_t)
      {
        .r = r_curr,
        .g = g_curr,
        .b = b_curr
      };
  }
}

int main (void) {
    spi_init();
    while(1){
        memcpy(old_buf, new_buf, NUM_OF_LED*sizeof(color_t));
        fill_new_buf();
        for(int i = 0; i < FADE_STEPS; i++){
            render_cur_buf(i);
            spi_send_buf(cur_buf, NUM_OF_LED*sizeof(color_t));
            _delay_ms (FADE_DELAY);
        }
    }
    return 0;
}
