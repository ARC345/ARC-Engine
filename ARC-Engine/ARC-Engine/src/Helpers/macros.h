#pragma once

#include <helpers.h>

#define _ASSERT_DB(x) if(!(x)) __debugbreak();

#define ifnr(x) if(!(x)) return;
#define ifr(x) if((x)) return;
#define ifnr_callback(x, y) if(!(x)){y; return;}