#pragma once

#define _ASSERT_DB(x) if(!(x)) __debugbreak();

#define ifnr(x) if(!(x)) return;
#define ifr(x) if((x)) return;
#define ifnr_callback(x, y) if(!(x)){y; return;}

#define COMBINE4(a,b,c,d) a##b##c##d
#define COMBINE3(a,b,c) a##b##c
#define COMBINE2(a,b) a##b

#define BIND_FN(x) std::bind(x, this, std::placeholders::_1)
