#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

int main(int argc, char** argv)
{
    doctest::Context ctx(argc, argv);
    ctx.setOption("order-by", "name");
    return ctx.run();
}
