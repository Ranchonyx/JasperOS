#ifndef TIMER_H
#define TIMER_H
#include "ISR.h"
#include "jaspdef.h"

void delay(uint32_t millis);
void timer_callback(cpu_state_t *state);
void init_timer(uint32_t hertz);
uint32_t timer_getticks();


#endif
