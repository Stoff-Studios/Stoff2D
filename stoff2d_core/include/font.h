#pragma once

#include <defines.h>
#include <stdbool.h>

typedef struct {
    Frame texRegion; // subregion where the glyph is located in the font bitmap
    i32   width;     // width of the glyph in pixels
    i32   height;    // height of the glyph in pixels
    i32   bearingX;  // offset from baseline to topleft of glyph
    i32   bearingY;
    u32   advance;   // how much to move right for next character
} s2dChar;

typedef struct {
    char*   fontName;
    u32     fontTexID;  // font bitmap
    s2dChar chars[128]; // first 128 ASCII characters for now.
} s2dFont;

bool     font_init();
void     font_shutdown();
s2dFont* font_get_font(const char* fontName);
