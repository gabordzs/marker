#include "marker.h"
#include <stdio.h>

int main()
{
    MARKER_INIT;

    printf("Hello Squidboy\n");

    MARKER_START;
    MARKER_BEGIN(2,1);
    printf("Do Work\n");
    MARKER_INFO(3,"work");
    MARKER_END(2,1);
    MARKER_STOP;

    return 0;
}
