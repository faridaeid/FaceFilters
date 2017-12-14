#include "src/CameraPreview.h"
#include "src/ui_CameraPreview.h"

CameraPreview::CameraPreview(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraPreview)
{
    ui->setupUi(this);
    
    connect(ui->filterButtonOne, &QPushButton::clicked, this, &CameraPreview::assignFilterSelected);
    connect(ui->filterButtonTwo, &QPushButton::clicked, this, &CameraPreview::assignFilterSelected);
    connect(ui->filterButtonThree, &QPushButton::clicked, this, &CameraPreview::assignFilterSelected);
    connect(ui->captureButton, &QPushButton::clicked, this, &CameraPreview::captureImage);
    connect(ui->closeButton, &QPushButton::clicked, this, &CameraPreview::stopFilters);


    
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
    ui->captureButton->setStyleSheet(captureButtonStyleSheet);
    ui->closeButton->setStyleSheet(captureButtonStyleSheet);

    
    ui->filterButtonOne->setText("MOUSTACHE");
    ui->filterButtonTwo->setText("FLOWER");
    ui->filterButtonThree->setText("HARRY POTTER");

    QIcon icon("Styling/captureImage2.png");
    ui->captureButton->setIcon(icon);
    ui->captureButton->setIconSize(QSize(80, 60));
    
    QIcon icon2("Styling/closeImage.png");
    ui->closeButton->setIcon(icon2);
    ui->closeButton->setIconSize(QSize(40, 40));
    
    frontalFaceDetector = dlib::get_frontal_face_detector();
    
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> shapePredictor;
    
    currentFilter = NoFilter;
    loadAndResizeFilter = false;
    
    startWebcam();
    
}

CameraPreview::~CameraPreview()
{
    frameTimer->stop();
    delete ui;
}

void CameraPreview::startWebcam() {
    
    const int framePeriod = 1000/5;
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
    
    ui->horizontalLayoutWidget->setGeometry(QRect(5, 5, newWidth - 20, newHeight - 35));
        
}

void CameraPreview::frameProcess() {
    
    videoCapture>>frame;
            
    cv::resize(frame, frame, cv::Size(ui->webcamPreview->width() - 1, ui->webcamPreview->height() - 1));
    
    detectFaces();
    
    cv::cvtColor(frame, frame, CV_BGR2RGB);
    
    cv::Mat invertedImage;
    
    cv::flip(frame, invertedImage, 1);
    
    QImage imageFrame = QImage((uchar*) invertedImage.data, frame.cols, frame.rows, QImage::Format_RGB888);
    
    QPixmap pixmapFrame = QPixmap::fromImage(imageFrame);
        
    ui->webcamPreview->setPixmap(pixmapFrame);
    
}

void CameraPreview::detectFaces() {
    
    cv::Mat detectionImage;
    
    cv::resize(frame, detectionImage, cv::Size(frame.cols / 4, frame.rows / 4));
    
    cv::cvtColor(detectionImage, detectionImage, CV_BGR2GRAY);
        
    dlib::cv_image<uchar> detectionImageDlib(detectionImage);
    
    std::vector<dlib::rectangle> detections = frontalFaceDetector(detectionImageDlib);
    
    for (int i = 0; i < detections.size(); i++) {       // only detects one face
        
        dlib::full_object_detection faceLandmark = shapePredictor(detectionImageDlib, detections[i]);
        
        switch (currentFilter) {
                
            case MoustacheFilter:
                addFilterOne(faceLandmark);
                break;
                
            case HarryPotterFilter:
                addFilterTwo(faceLandmark);
                break;
                
            case FlowerFilter:
                addFilterThree(faceLandmark);
                break;
                
            default:
                break;
        }

    }
    
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

void CameraPreview::assignFilterSelected() {
    
    QObject * buttonSelected = QObject::sender();
    
    loadAndResizeFilter  = true;
    
    if (buttonSelected == ui->filterButtonOne) {
        
        currentFilter = MoustacheFilter;
        
    } else if (buttonSelected == ui->filterButtonTwo) {
        
        currentFilter = FlowerFilter;
        
    } else  if (buttonSelected == ui->filterButtonThree) {
        
        currentFilter = HarryPotterFilter;
    }
}


void CameraPreview::overlayFilter(const cv::Mat& background,
                                  const cv::Mat& foreground,
                                  cv::Mat& output,
                                  cv::Point upperLeftCorner) {
    
    background.copyTo(output);
    
    
    for (int y = 0; y < foreground.rows; y++) {
        
        for (int x = 0; x < foreground.cols; x++) {
            
            double opacity = ((double)foreground.data[y * foreground.step + x * foreground.channels() + 3]) / 255.;
            
            int offsetx = x + upperLeftCorner.x;
            int offsety = y + upperLeftCorner.y;
            
            if (opacity > 0) {
                
                for (int channel = 0; channel < output.channels(); channel++) {
                    
                    unsigned char foregroundPixel = foreground.data[y * foreground.step + x * foreground.channels() + channel];
                    
                    unsigned char backgroundPixel = background.data[offsety * background.step + offsetx * background.channels() + channel];
                    
                    output.data[offsety * output.step + offsetx * output.channels() + channel] =
                    backgroundPixel * (1.0 - opacity) + foregroundPixel * opacity;
                    
                }
            }
        }
    }
}

void CameraPreview::addFilterOne(const dlib::full_object_detection& faceLandmark) {
    
    int x = faceLandmark.part(48).x() * 4;
    int y = faceLandmark.part(33).y() * 4;
    
    if (loadAndResizeFilter) {
        
        moustacheImage = cv::imread("Filters/moustache.png", cv::IMREAD_UNCHANGED);
        
        int imageLength = (faceLandmark.part(54).x() - faceLandmark.part(48).x()) * 4;
        int imageHeight = (faceLandmark.part(51).y() - faceLandmark.part(33).y()) * 4;
        
        cv::resize(moustacheImage, moustacheImage, cv::Size(imageLength, imageHeight));
        
        loadAndResizeFilter = false;
    
    }
    
    overlayFilter(frame, moustacheImage, frame, cv::Point(x,y));

}

void CameraPreview::addFilterTwo(const dlib::full_object_detection& faceLandmark) {
    
    int x0 = faceLandmark.part(0).x() * 4;
    int y0 = faceLandmark.part(21).y() * 4;
    
    int x1 = faceLandmark.part(22).x() * 4;
    int y1 = faceLandmark.part(24).y() * 4 - 40;
    
    if (loadAndResizeFilter) {
        
        harryPotterImage[0] = cv::imread("Filters/glasses.png", cv::IMREAD_UNCHANGED);
        harryPotterImage[1] = cv::imread("Filters/scar.png", cv::IMREAD_UNCHANGED);
        
        int imageLength0 = (faceLandmark.part(16).x() - faceLandmark.part(0).x()) * 4;
        int imageHeight0 = (faceLandmark.part(30).y() - faceLandmark.part(21).y()) * 4;
        int imageLength1 = (faceLandmark.part(20).x() - faceLandmark.part(17).x()) * 4;
        int imageHeight1 = 50;

        cv::resize(harryPotterImage[0], harryPotterImage[0], cv::Size(imageLength0, imageHeight0));
        cv::resize(harryPotterImage[1], harryPotterImage[1], cv::Size(imageLength1, imageHeight1));
        
        loadAndResizeFilter = false;
        
    }
    
    overlayFilter(frame, harryPotterImage[0], frame, cv::Point(x0,y0));
    overlayFilter(frame, harryPotterImage[1], frame, cv::Point(x1,y1));

    
}

void CameraPreview::addFilterThree(const dlib::full_object_detection& faceLandmark) {
    
    int x = faceLandmark.part(0).x() * 4;
    int y = faceLandmark.part(20 ).y() * 4 - 100;
    
    if (loadAndResizeFilter) {
        
        flowerImage = cv::imread("Filters/flowers.png", cv::IMREAD_UNCHANGED);
        
        int imageLength = (faceLandmark.part(16).x() - faceLandmark.part(0).x()) * 4;
        int imageHeight = 100;
        
        cv::resize(flowerImage, flowerImage, cv::Size(imageLength, imageHeight));
        
        loadAndResizeFilter = false;
        
    }
    
    overlayFilter(frame, flowerImage, frame, cv::Point(x,y));
    
    
}

void CameraPreview::stopFilters() {
    
    currentFilter = NoFilter;
    
}

