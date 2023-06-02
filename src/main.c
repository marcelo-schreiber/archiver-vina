#include "unpack.h"
#include "pack.h"

int main(void)
{
    new_pack("vina.vpp", (char *[]){"./pasta/new_LEIAME.md", "./pasta/new_.gitignore"}, 2);
    unpack("vina.vpp");

    return 0;
}