#ifndef DBGHELPER_H
#define DBGHELPER_H

#include <QtCore/QtDebug>

#ifdef OBV_NO_DEBUG_OUTPUT

#ifndef QT_NO_DEBUG
#define QT_NO_DEBUG
#endif

#ifndef QT_NO_DEBUG_OUTPUT
#define QT_NO_DEBUG_OUTPUT
#endif

#endif

#define myDebug() qDebug() << Q_FUNC_INFO << " "
#define myWarning() qWarning() << "W: "<< Q_FUNC_INFO << " "

/*#ifdef OBV_NO_DEBUG_OUTPUT
#define DEBUG_FUNC_NAME
#else*/
#define DEBUG_FUNC_NAME DbgHelper dbgHelper(Q_FUNC_INFO);
/*#endif*/


#include <QtCore/QString>
/**
 * DbgHelper is a debug helper, it shows the debug output more clearly
 */

class DbgHelper
{
public:
    DbgHelper(const QString &t);
    ~DbgHelper();
private:
    QString txt;
    static int indent;
    static int colorIndex;
    int myColor;
};
#endif
