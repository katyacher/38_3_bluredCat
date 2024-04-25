#include <QtGui/QtGui>
#include <QApplication>
//
#include <QImage>
#include <QLabel>

#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QPainter>
//
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
//
#include <QFileDialog>


QImage blurImage(QImage source, int blurRadius){
    if(source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));
    //
    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurRadius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(),
                 QRectF(0, 0, source.width(), source.height()));
    return result;

};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget imageWindow;
    QVBoxLayout vbox(&imageWindow);


    auto imageLabel = new QLabel(&imageWindow);
    imageLabel->setMinimumSize(400, 400);
    imageLabel->setAlignment(Qt::AlignCenter);
    vbox.addWidget(imageLabel);

    QSlider *blurSlider = new QSlider(&imageWindow);
    blurSlider->setOrientation(Qt::Horizontal);
    blurSlider->setMaximum(0);
    blurSlider->setMaximum(10);
    vbox.addWidget(blurSlider);

    auto *openFileButton = new QPushButton("Open", &imageWindow);
    vbox.addWidget(openFileButton);



    QString filePath;
    QPixmap *image_pxmp;

    QObject::connect(openFileButton, &QPushButton::clicked, [&filePath, &image_pxmp, &imageLabel](){
        filePath = QFileDialog::getOpenFileName(nullptr,
                                     "Open Images",
                                     "",
                                     "Image files (*.jpg, *.JPG, *.png, *.PNG, *.jpeg)");
        if(filePath.isEmpty()) return;
        image_pxmp = new QPixmap(filePath);
        imageLabel->setPixmap(image_pxmp->scaled(imageLabel->width(), imageLabel->height(), Qt::KeepAspectRatio));
    });


    QObject::connect(blurSlider, &QSlider::valueChanged, [&imageLabel, &image_pxmp](quint64 newValue){
        imageLabel->setPixmap(QPixmap::fromImage(blurImage(image_pxmp->toImage(), newValue).scaled(
                    imageLabel->width(),
                    imageLabel->height(), Qt::KeepAspectRatio)));
    });

    imageWindow.show();
    delete image_pxmp;
    return a.exec();

}
