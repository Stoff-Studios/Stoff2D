#pragma once

#include <defines.h>
#include <stdbool.h>

bool     font_init();
void     font_shutdown();
s2dFont* font_get_font(const char* fontName);
