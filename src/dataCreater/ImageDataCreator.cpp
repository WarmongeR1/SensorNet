#include "ImageDataCreator.h"
#include "Options.h"
#include <QImage>
#include <QDir>

void ImageDataCreator::CreateImageData(QString dataDirName)
{
	QDir dataDir(dataDirName);
	QStringList dataFiles = dataDir.entryList(QStringList("*.bmp"));

	for(uint n = 0; n < dataFiles.size(); n++)
	{
		QString imagePath = dataDir.absoluteFilePath(dataFiles[n]);
		QImage image(imagePath);

		data << DataSample();

		for(uint i = 0; i < FIELD_Y; i++)
			for(uint j = 0; j < FIELD_X; j++)
			{
				QRgb colour = image.pixel(j, i);
				if(colour != 0xffffffff)
					data[n].data[i][j] = 1;
				else
					data[n].data[i][j] = 0;
			}
	}
}

const QList<DataSample> & ImageDataCreator::GetData() const
{
	return data;
}
