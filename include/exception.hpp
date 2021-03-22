#pragma once

#include <nusys.h>

struct reg_desc {
    u32 mask;
    u32 value;
    char *string;
};

extern const reg_desc causeDesc[];
extern const reg_desc srDesc[];
extern const reg_desc fpcsrDesc[];


void debug_draw_char(u16* buffer, u32 x, u32 y, char c);
void debug_draw_string(u16* buffer, u32 x, u32 y, char* string);
void debug_printreg(u16* buffer, u32 x, u32 y, u32 value, char *name, reg_desc *desc);