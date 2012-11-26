#include "ImageDataCreator.h"
#include "Options.h"
#include <QImage>
#include <QDir>

#include "Random.h"

void ImageDataCreator::CreateImageData(QString dataDirName)
{
    data.clear();

    QDir dataDir(dataDirName);
    QStringList dataFiles = dataDir.entryList(QStringList("*.bmp"));

    for(uint n = 0; n < dataFiles.size(); n++)
    {
        QString imagePath = dataDir.absoluteFilePath(dataFiles[n]);
        QImage image(imagePath);

        DataSample sample;

        sample.type = QString(dataFiles[n][0]).toInt();

        for(uint i = 0; i < FIELD_Y; i++)
            for(uint j = 0; j < FIELD_X; j++)
            {
                QRgb colour = image.pixel(j, i);
                if(colour != 0xffffffff)
                    sample.data[i][j] = 1;
                else
                    sample.data[i][j] = 0;
            }

        data << sample;

        //if(trainData)
        //{
        //            for(uint s = 0; s < SACCADE_N - 1; s++)
        //            {
        //                DataSample saccadeSample;

        //                int sx, sy;
        //                do
        //                {
        //                    sx = Random::Range(- SACCADE_X_SHIFT, SACCADE_X_SHIFT);
        //                    sy = Random::Range(- SACCADE_Y_SHIFT, SACCADE_Y_SHIFT);
        //                }
        //                while(sx == 0 && sy == 0);

        //                for(uint i = 0; i < FIELD_Y; i++)
        //                    for(uint j = 0; j < FIELD_X; j++)
        //                    {
        //                        saccadeSample.data[i][j] = sample.data[(i + sy + FIELD_Y) % FIELD_Y][(j + sx + FIELD_X) % FIELD_X];
        //                    }

        //                saccadeSample.type = sample.type;

        //                data << saccadeSample;
        //            }
        //}
    }
}

const QList<DataSample> & ImageDataCreator::GetData() const
{
    return data;
}
