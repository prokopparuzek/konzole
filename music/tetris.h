#pragma once
#include "tones.h"
// tetris song
// 140 bpm
#define CTVRT 428 // v ms

const uint16_t song [] PROGMEM = {
    e2, CTVRT, h1, CTVRT/2, c2, CTVRT/2, d2, CTVRT/2, e2, CTVRT/4, d2, CTVRT/4, c2, CTVRT/2, h1, CTVRT/2,
    a1, CTVRT, a1, CTVRT/2, c2, CTVRT/2, e2, CTVRT, d2, CTVRT/2, c2, CTVRT/2
};
