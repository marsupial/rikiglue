/*
   screenshot.h
*/

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QtGui/QPixmap>
#include <QtGui/QWidget>
#include <QtGui/QLabel>

class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QVBoxLayout;


class Screenshot : public QWidget
{
	Q_OBJECT

public:
	Screenshot();
	~Screenshot();

protected:
	void resizeEvent(QResizeEvent *event);

private slots:
	void saveScreenshot();
	void shootScreen();
	void updateCheckBox();

private:
	void createOptionsGroupBox();
	void createButtonsLayout();
	QPushButton *createButton(const QString &text, QWidget *receiver,
							  const char *member);
	void updateScreenshotLabel( const QRect &area );

	QPixmap originalPixmap;

	QLabel *screenshotLabel;
	QGroupBox *optionsGroupBox;
	QSpinBox *delaySpinBox;
	QLabel *delaySpinBoxLabel;
	QCheckBox *hideThisWindowCheckBox;
	QPushButton *saveScreenshotButton;
	QPushButton *quitScreenshotButton;

	QVBoxLayout *mainLayout;
	QGridLayout *optionsGroupBoxLayout;
	QHBoxLayout *buttonsLayout;
	
	QTimer *timer;
};

#endif