#include "src/CameraPreview.h"
#include "src/ui_CameraPreview.h"

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
    connect(ui->captureButton, &QPushButton::clicked, this, &CameraPreview::captureImage);

    
    QString pushButtonStyleSheet = "QPushButton {color: #ffffff;"
                                                "font-family: Georgia Regular;"
                                                "font-size: 12px;"
                                                "background-color: #2089D6;"
                                                "border-radius: 10px;"
                                                "border-style: outset;"
                                                "padding: 5px;}";
    
    QString captureButtonStyleSheet = "QPushButton { "
                                        "border-radius: 10px;"
                                        "border-style: outset;}";
    
    
    ui->filterButtonOne->setStyleSheet(pushButtonStyleSheet);
    ui->filterButtonTwo->setStyleSheet(pushButtonStyleSheet);
    ui->filterButtonThree->setStyleSheet(pushButtonStyleSheet);
    ui->filterButtonFour->setStyleSheet(pushButtonStyleSheet);
    ui->filterButtonFive->setStyleSheet(pushButtonStyleSheet);
    ui->captureButton->setStyleSheet(captureButtonStyleSheet);
    
    ui->filterButtonOne->setText("Dog");
    ui->filterButtonTwo->setText("Moustache");
    ui->filterButtonThree->setText("Hat");
    ui->filterButtonFour->setText("Rainbow");
    ui->filterButtonFive->setText("Cat");
    
    QIcon icon("Styling/captureImage2.png");
    ui->captureButton->setIcon(icon);
    ui->captureButton->setIconSize(QSize(80, 60));
    
    
    startWebcam();
    
}

CameraPreview::~CameraPreview()
{
    frameTimer->stop();
    delete ui;
}

void CameraPreview::startWebcam() {
    
    const int framePeriod = 1000/30;
    frameTimer = new QTimer(this);
    frameTimer->setInterval(framePeriod);
    connect(frameTimer, &QTimer::timeout, this, &CameraPreview::frameProcess);
    
    videoCapture.open(CV_CAP_ANY);
    frameTimer->start();
    
}

void CameraPreview::resizeWindowWithFrame(const double& ratio) {
    
    videoCapture>>frame;
    
    int newWidth = frame.cols / ratio;
    int newHeight = frame.rows / ratio;
    
    this->resize(newWidth, newHeight);
    
    ui->horizontalLayoutWidget->setGeometry(QRect(5, 5, newWidth - 15, newHeight - 35));
        
}

void CameraPreview::frameProcess() {
    
    videoCapture>>frame;
    
//    ui->webcamPreview->resize(ui->webcamPreview->width() + 2, ui->webcamPreview->height() + 2);
        
    cv::resize(frame, frame, cv::Size(ui->webcamPreview->width() - 1, ui->webcamPreview->height() - 1));
    
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    
    cv::Mat invertedImage;
    
    cv::flip(frame, invertedImage, 1);
    
    QImage imageFrame = QImage((uchar*) invertedImage.data, frame.cols, frame.rows, QImage::Format_RGB888);
    
    QPixmap pixmapFrame = QPixmap::fromImage(imageFrame);
        
    ui->webcamPreview->setPixmap(pixmapFrame);
    
}

void CameraPreview::captureImage() {
    
    std::time_t rawTime;
    std::tm* timeInfo;
    char buff[80];
    
    std::time(&rawTime);
    timeInfo = std::localtime(&rawTime);
    
    std::strftime(buff, 80, "%Y-%m-%d-%H-%M-%S", timeInfo);
    
    std::string pictureName(buff);
    
    cv::Mat invertedImage;
    
    cv::flip(frame, invertedImage, 1);
    
    cv::cvtColor(invertedImage, invertedImage, CV_BGR2RGB);
    
    cv::imwrite("Pictures/" + pictureName + ".jpg", invertedImage);
    
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


