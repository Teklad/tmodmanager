#include "util/appdata.h"
#include "util/filesystem.h"
#include "util/tmodfile.h"
#include <QDebug>

int main(int argc, char **argv)
{
    qDebug() << AppData::GetModList();
	return 0;
}
