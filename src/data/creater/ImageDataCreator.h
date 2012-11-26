#ifndef IMAGE_DATA_CREATOR_H
#define IMAGE_DATA_CREATOR_H

#include "DataSample.h"
#include <QList>

class ImageDataCreator
{
public:
    void CreateImageData(QString dataDirName);
    const QList<DataSample> & GetData() const;

private:
    QList<DataSample> data;
};

#endif
