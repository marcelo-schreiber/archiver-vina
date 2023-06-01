#include "unpack.h"
#include "pack.h"

int main(void)
{
    pack("vina.vpp");
    unpack("vina.vpp");
    return 0;
}