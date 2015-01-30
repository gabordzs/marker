#include <stdio.h>
#include <marker_stub.h>

int main()
{
    MARKER_INIT;

    printf("Hello Squidboy\n");

    MARKER_START;
    printf("Outer Loop\n");
    MARKER_BEGIN(2,1);
    printf("Do Work\n");
    MARKER_INFO(3,"work");
    MARKER_END(2,1);
    printf("Exiting inner loop\n");
    MARKER_STOP;

    return 0;
}
