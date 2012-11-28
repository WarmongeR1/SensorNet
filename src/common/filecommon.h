#ifndef __FILECOMMON__H
#define __FILECOMMON__H



#include <QString>
#include <QProcess>
#include <QDir>
#include <QTextCodec>

#include <QDirIterator> // for recoursiveFind
#include <QStringList>

#include <QDebug>

/**
 * @brief recursiveFind
 * @param directory
 * @return
 */
QStringList recursiveFind(QString directory);

#endif // __FILECOMMON__H
