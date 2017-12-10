#include "CameraPreview.h"
#include "ui_CameraPreview.h"

CameraPreview::CameraPreview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraPreview)
{
    ui->setupUi(this);
    
    
    connect(ui->filterButtonOne, &QPushButton::clicked, this, &CameraPreview::addFilterOne);
    connect(ui->filterButtonTwo, &QPushButton::clicked, this, &CameraPreview::addFilterTwo);
    connect(ui->filterButtonThree, &QPushButton::clicked, this, &CameraPreview::addFilterThree);
    connect(ui->filterButtonFour, &QPushButton::clicked, this, &CameraPreview::addFilterFour);
    connect(ui->filterButtonFive, &QPushButton::clicked, this, &CameraPreview::addFilterFive);
    
    startWebcam();
    
}

CameraPreview::~CameraPreview()
{
    frameTimer->stop();
    delete ui;
}

void CameraPreview::startWebcam() {
    
    const int framePeriod = 1000/40;
    frameTimer = new QTimer(this);
    frameTimer->setInterval(framePeriod);
    connect(frameTimer, &QTimer::timeout, this, &CameraPreview::frameProcess);
    
    videoCapture.open(CV_CAP_ANY);
    frameTimer->start();
    
}

void CameraPreview::frameProcess() {
    
    videoCapture>>frame;
    
    cv::resize(frame, frame, cv::Size(ui->webcamPreview->width() - 10, ui->webcamPreview->height() - 10));
        
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    
    cv::Mat invertedImage;
    
    cv::flip(frame, invertedImage, 1);
    
    QImage imageFrame = QImage((uchar*) invertedImage.data, invertedImage.cols, invertedImage.rows, QImage::Format_RGB888);
    
    QPixmap pixmapFrame = QPixmap::fromImage(imageFrame);
        
    ui->webcamPreview->setPixmap(pixmapFrame);
    
}

void CameraPreview::addFilterOne() {


    printf("Pressed Filter One button\n");
    
}

void CameraPreview::addFilterTwo() {
    
    printf("Pressed Filter Two button\n");
    
}

void CameraPreview::addFilterThree() {
    
    printf("Pressed Filter Three button\n");
    
}

void CameraPreview::addFilterFour() {
    
    printf("Pressed Filter Four button\n");
    
}

void CameraPreview::addFilterFive() {
    
    printf("Pressed Filter Five button\n");
    
}


