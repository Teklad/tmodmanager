#ifndef  __APPDATA_H__
#define  __APPDATA_H__
#include <QString>
#include <QStringList>
#include <vector>
/**
 * @brief Very basic class for the few filesystem operations we need
 *        to perform.
 */
class AppData {
    public:
        static QString GetAppDataDirectory();
        static QString GetModDirectory();
        static QStringList GetModList();

};
#endif //__APPDATA_H__
