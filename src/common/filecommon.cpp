#include "filecommon.h"

#include "src/debug/debughelper.h"

//------------------------------------------------------------------------------
QStringList recursiveFind(QString directory)
{
    //// подумать, нужен ли чистый рекурсивный поиск
    //// если нет, то сюда вставить обработку getModuleFilesList
    QStringList list;
    QDirIterator iterator (directory, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    while(iterator.hasNext())
    {
        iterator.next();
        list << iterator.fileInfo().absoluteFilePath();
    }
    return list;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
