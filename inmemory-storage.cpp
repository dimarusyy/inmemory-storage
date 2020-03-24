#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

int main(int argc, char** argv)
{
    doctest::Context context;
    context.applyCommandLine(argc, argv);

    auto res = context.run();
    if (context.shouldExit())
        return res;

    return res;
}
