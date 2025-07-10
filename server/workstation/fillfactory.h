#ifndef FILLFACTORY_H
#define FILLFACTORY_H

#include <QObject>
#include <QMap>
#include <QPixmap>

#include "gemTypes.h"
#include "properties.h"

/*****************************************************************************\
|* Forward declarations
\*****************************************************************************/
class Workstation;

class FillFactory : public QObject
	{
	Q_OBJECT
	NON_COPYABLE_NOR_MOVEABLE(FillFactory)

	public:
		/*********************************************************************\
		|* Typedefs and defines
		\*********************************************************************/

	private:
		/*********************************************************************\
		|* Private state
		\*********************************************************************/
		QMap<int, QImage> _patterns;

		/*********************************************************************\
		|* Private constructor
		\*********************************************************************/
		explicit FillFactory(QObject *parent = nullptr);

	public:
		/*********************************************************************\
		|* Get a reference to the font manager
		\*********************************************************************/
		static FillFactory& sharedInstance(void)
			{
			static FillFactory _instance;			// The actual shared instance
			return _instance;
			}

		/*********************************************************************\
		|* Get the QImage for a recognised combination of type and style. This
		|* will always return *something*, and will create known variants as
		|* needed
		\*********************************************************************/
		QImage& patternFor(int type, int style);

		/*********************************************************************\
		|* Allow setting of the user-defined fill-pattern
		\*********************************************************************/
		bool setUserFill(MFDB *mfdb, Workstation *ws);

		/*********************************************************************\
		|* Create an image from an MFDB
		\*********************************************************************/
		QImage * imageFromMFDB(MFDB *mfdb, Workstation *ws);
		QPixmap bitmapFromMFDB(MFDB *mfdb, Palette palette);


	private:
		/*********************************************************************\
		|* Actually create an image and store it
		\*********************************************************************/
		QImage& _create(int type, int style, uint8_t *src, int planes=1);

		/*********************************************************************\
		|* Different depths of MFDB to create an image from
		\*********************************************************************/
		QImage * _imageFromMFDB1(MFDB *mfdb, Workstation *ws);
		QImage * _imageFromMFDB4(MFDB *mfdb, Workstation *ws);
		QImage * _imageFromMFDB8(MFDB *mfdb, Workstation *ws);
	};

#endif // FILLFACTORY_H
