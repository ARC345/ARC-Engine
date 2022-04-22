#pragma once

#define ECS_FWD(mX) ::std::forward<decltype(mX)>(mX)
#define ECS_TYPE(mX) typename decltype(mX)::type