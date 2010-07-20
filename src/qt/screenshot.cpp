/*
   screenshot.cpp
*/

#include <QtGui/QtGui>

#include "screenshot.h"
#include "rikiFrame.h"

Screenshot::Screenshot() : timer( new QTimer(this) )
{
	screenshotLabel = new QLabel;
	screenshotLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	screenshotLabel->setAlignment(Qt::AlignCenter);
	screenshotLabel->setMinimumSize(240, 160);

	createOptionsGroupBox();
	createButtonsLayout();

	mainLayout = new QVBoxLayout;
	mainLayout->addWidget(screenshotLabel);
	mainLayout->addWidget(optionsGroupBox);
	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);

	hideThisWindowCheckBox->setCheckState(Qt::Unchecked);
	delaySpinBox->setValue(1000);

	setWindowTitle(tr("Screenshot"));
	resize(300, 200);

	connect(timer, SIGNAL(timeout()), this, SLOT(shootScreen()));
	timer->start(1000);

	// setWindowFlags( windowFlags() | Qt::FramelessWindowHint );
	//setAttribute(Qt::WA_TranslucentBackground, true);
	//setAttribute(Qt::WA_NoSystemBackground, true);
}

Screenshot::~Screenshot()
{
	delete timer;
}

static QRect
getScreenArea( QLabel      *screenshotLabel,
               const QRect &pRect )
{
	return ( screenshotLabel->geometry().translated( pRect.x(), pRect.y()) );
}

static void
setCutout( QWidget    *widget,
           QWidget    *cutOut )
{
	QBitmap bmap( widget->rect().size() );
	QPainter bPainter(&bmap);
	if ( cutOut )
	{
		bPainter.fillRect(widget->rect(), Qt::black);
		bPainter.fillRect(cutOut->geometry(), Qt::white);
	}
	else
		bPainter.fillRect(widget->rect(), Qt::white);

	widget->setMask(bmap);
}

void Screenshot::resizeEvent(QResizeEvent * /* event */)
{
#ifdef Q_WS_MAC
	extern OSWindowRef qt_mac_window_for(const QWidget *);
	ChangeWindowAttributes((WindowRef)qt_mac_window_for(this), kWindowNoShadowAttribute, 0);
#endif

	shootScreen();
}

void Screenshot::saveScreenshot()
{
	QString format = "png";
	QString initialPath = QDir::currentPath() + tr("/untitled.") + format;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
							   initialPath,
							   tr("%1 Files (*.%2);;All Files (*)")
							   .arg(format.toUpper())
							   .arg(format));
	if (!fileName.isEmpty())
		originalPixmap.save(fileName, format.toAscii());
}

void Screenshot::shootScreen()
{
	const QRect screenRect = geometry();
	const QRect area = getScreenArea(screenshotLabel, screenRect);
	if ( hideThisWindowCheckBox->isChecked() )
		hide();
	else
		setCutout(this, screenshotLabel);

	originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());

	updateScreenshotLabel(area);

	if ( hideThisWindowCheckBox->isChecked() )
	{
		show();
		setGeometry(screenRect);
	}
	else
		setCutout(this, NULL);
}

void Screenshot::updateCheckBox()
{
	timer->stop();
	timer->start( delaySpinBox->value() );
}

void Screenshot::createOptionsGroupBox()
{
	optionsGroupBox = new QGroupBox(tr("Options"));

	delaySpinBox = new QSpinBox;
	delaySpinBox->setSuffix(tr(" ms"));
	delaySpinBox->setMinimum(1);
	delaySpinBox->setMaximum(1000);
	connect(delaySpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCheckBox()));

	delaySpinBoxLabel = new QLabel(tr("Screenshot Delay:"));

	hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"));

	optionsGroupBoxLayout = new QGridLayout;
	optionsGroupBoxLayout->addWidget(delaySpinBoxLabel, 0, 0);
	optionsGroupBoxLayout->addWidget(delaySpinBox, 0, 1);
	optionsGroupBoxLayout->addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);
	optionsGroupBox->setLayout(optionsGroupBoxLayout);
}

void Screenshot::createButtonsLayout()
{
	saveScreenshotButton = createButton(tr("Save Screenshot"), this, SLOT(saveScreenshot()));
	quitScreenshotButton = createButton(tr("Quit"), this, SLOT(close()));

	buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(saveScreenshotButton);
	buttonsLayout->addWidget(quitScreenshotButton);
}

QPushButton *Screenshot::createButton(const QString &text, QWidget *receiver,
									  const char *member)
{
	QPushButton *button = new QPushButton(text);
	button->connect(button, SIGNAL(clicked()), receiver, member);
	return button;
}

void Screenshot::updateScreenshotLabel( const QRect &area )
{
	QPixmap crop = originalPixmap.copy(area);

	QImage img = crop.toImage();

	const QImage::Format f = QImage::Format_RGB32;
	if ( img.format() != f )
		img = img.convertToFormat(f);

	rikiGlue::Frame frame(img.bits(), img.bytesPerLine(), img.height());
	frame.decryptLut();

	screenshotLabel->setPixmap( QPixmap::fromImage(img) );
}