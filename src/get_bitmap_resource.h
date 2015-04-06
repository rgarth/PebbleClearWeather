#pragma once

#include "pebble.h"

// Broke this out here, as two 48 line case statements are a little ugly
int get_bitmap(int code);
int get_sm_bitmap(int code);