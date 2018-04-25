#include "util/tmodfile.h"
#include "util/appdata.h"
#include <iostream>
int main()
{
    std::cout << AppData::GetModDirectory() << '\n';
    return 0;
}
