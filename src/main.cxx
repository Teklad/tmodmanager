#include "util/appdata.h"
#include "util/filesystem.h"
#include "util/tmodfile.h"

#include <iostream>
#include <fstream>
#include <iterator>

using namespace TMM;
int main(int argc, char **argv)
{
    std::string modDir = AppData::GetModDirectory();
    for (auto entry : AppData::GetModList(modDir)) {
        std::string fpath(modDir);
        fpath.push_back('/');
        fpath.append(entry);
        TmodFile tmod(fpath);
        tmod.Read();
        for (auto f : tmod.GetFiles()) {
            std::string fpath("dump/");
            fpath.append(tmod.GetName());
            fpath.push_back('/');
            fpath.append(f);
            auto dat = tmod.GetFileData(f);
            if (!FileSystem::CreateDirectory(FileSystem::BaseDirectory(fpath))) {
                std::cout << "Failed to create directory: " << fpath << '\n';
                continue;
            }
            std::ofstream of(fpath);
            std::ostream_iterator<uint8_t> oi(of);
            std::copy(dat.begin(), dat.end(), oi);
        }
    }
    return 0;
}
