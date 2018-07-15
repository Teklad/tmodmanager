#include "util/appdata.h"
#include "util/tmodfile.h"
#include <QFile>
#include <QDebug>
#include <iostream>

int main(int argc, char **argv)
{
    QString test = AppData::GetModDirectory() + "/" + AppData::GetModList()[0];
    TMM::TmodFile f(test);
    f.Read();
    qDebug() << f.ListFiles();
	return 0;
}
