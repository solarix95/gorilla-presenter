#include "goprcontroller.h"
#include "goprabstractsource.h"
#include "goprstandardrenderer.h"
#include "goprfilesource.h"
#include "goprplayer.h"
#include "goprstackmodel.h"
#include "ui_gopreditor.h"
#include <QFileDialog>

//-------------------------------------------------------------------------------------------------
GoPrController::GoPrController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoPrEditor),
    mRenderer(new GoPrStandardRenderer()),
    mOpenGlViewer(NULL)

{
    ui->setupUi(this);
    setWindowIcon(QIcon(":resources/icons8-gorilla-50.png"));

    connect(ui->btnOpenLiveImage, SIGNAL(clicked(bool)), this, SLOT(selectLiveImage()));
    connect(ui->btnLogo,          SIGNAL(clicked(bool)), this, SLOT(selectLogo()));

    mPlayer.setRenderer(mRenderer);
    ui->stackView->setModel(new GoPrStackModel(this,&mPlayer));

    mOpenGlViewer = new GoPrOpenglViewer();

    QObject::connect(this, SIGNAL(requestImage(QString)), mOpenGlViewer, SLOT(showLiveImage(QString)));
    QObject::connect(mOpenGlViewer, SIGNAL(viewerReady()), this, SLOT(init()));

    QObject::connect(player(), SIGNAL(slideTo(QImage)), mOpenGlViewer, SLOT(toSlide(QImage)));
    QObject::connect(player(), SIGNAL(slideFrom(QImage)), mOpenGlViewer, SLOT(fromSlide(QImage)));
    QObject::connect(player(), SIGNAL(slideTo(GoPrImageStream*)), mOpenGlViewer, SLOT(toStream(GoPrImageStream*)));
    QObject::connect(player(), SIGNAL(slideFrom(GoPrImageStream*)), mOpenGlViewer, SLOT(fromStream(GoPrImageStream*)));
    QObject::connect(player(), SIGNAL(requestBackground(QColor)), mOpenGlViewer, SLOT(toColor(QColor)));
    QObject::connect(mOpenGlViewer, SIGNAL(requestNextSlide()), player(), SLOT(nextSlide()));
    QObject::connect(mOpenGlViewer, SIGNAL(requestPreviousSlide()), player(), SLOT(prevSlide()));
    QObject::connect(mOpenGlViewer, SIGNAL(requestNextPresentation()), player(), SLOT(nextPresentation()));
    QObject::connect(mOpenGlViewer, SIGNAL(requestPreviousPresentation()), player(), SLOT(prevPresentation()));
    mOpenGlViewer->show();
}

//-------------------------------------------------------------------------------------------------
GoPrController::~GoPrController()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
GoPrPlayer *GoPrController::player()
{
    return &mPlayer;
}

//-------------------------------------------------------------------------------------------------
void GoPrController::init()
{
    GoPrPresentation *newPresentation = new GoPrPresentation(new GoPrFileSource(":/demos/hello.json"));
    newPresentation->init();
    mPlayer.addPresentation(newPresentation);

    foreach(QString arg, qApp->arguments()) {
        if (arg.startsWith("--logo=")) {
            QStringList parts = arg.split("=");
            if (parts.count() != 2)
                continue;
            mOpenGlViewer->loadLogoByJson(parts[1]);
            continue;
        }

        if (arg.endsWith(".json")) {
            GoPrPresentation *newPresentation = new GoPrPresentation(new GoPrFileSource(arg));
            newPresentation->init();
            mPlayer.addPresentation(newPresentation);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void GoPrController::selectLiveImage()
{
    QString fname = QFileDialog::getOpenFileName(this,"","");
    if (fname.toLower().endsWith(".jpg"))
        emit requestImage(fname);
    if (fname.toLower().endsWith(".png"))
        emit requestImage(fname);
    if (fname.toLower().endsWith(".json")) {
        GoPrPresentation *newPresentation = new GoPrPresentation(new GoPrFileSource(fname));
        newPresentation->init();
        mPlayer.addPresentation(newPresentation);
    }
}

void GoPrController::selectLogo()
{
    QString fname = QFileDialog::getOpenFileName(this,"","");
    if (fname.toLower().endsWith(".json")) {
        mOpenGlViewer->loadLogoByJson(fname);
    }
}
