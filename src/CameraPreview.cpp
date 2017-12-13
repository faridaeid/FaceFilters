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
//    connect(ui->filterButtonFour, &QPushButton::clicked, this, &CameraPreview::assignFilterSelected);
//    connect(ui->filterButtonFive, &QPushButton::clicked, this, &CameraPreview::assignFilterSelected);
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
//    ui->filterButtonFour->setStyleSheet(pushButtonStyleSheet);
//    ui->filterButtonFive->setStyleSheet(pushButtonStyleSheet);
    ui->captureButton->setStyleSheet(captureButtonStyleSheet);
    
    ui->filterButtonOne->setText("Moustahce");
    ui->filterButtonTwo->setText("Dog");
    ui->filterButtonThree->setText("Harry Potter");
//    ui->filterButtonFour->setText("Rainbow");
//    ui->filterButtonFive->setText("Cat");
    
    QIcon icon("Styling/captureImage2.png");
    ui->captureButton->setIcon(icon);
    ui->captureButton->setIconSize(QSize(80, 60));
    
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
    
    ui->horizontalLayoutWidget->setGeometry(QRect(5, 5, newWidth - 15, newHeight - 35));
        
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
    
    if (detections.size()) {       // only detects one face
        
        dlib::full_object_detection faceLandmark = shapePredictor(detectionImageDlib, detections[0]);
        
        for (int j = 0; j < faceLandmark.num_parts(); j++) {
            
            cv::circle(frame, cv::Point(faceLandmark.part(j).x() * 4, faceLandmark.part(j).y() * 4) , 2, cv::Scalar(255,0,0), 2);
            
        }
        
        switch (currentFilter) {
                
            case MoustacheFilter:
                addFilterOne(faceLandmark);
                break;
                
            case HarryPotterFilter:
                addFilterTwo(faceLandmark);
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
        
        currentFilter = DogFilter;
        
    } else  if (buttonSelected == ui->filterButtonThree) {
        
        currentFilter = HarryPotterFilter;
    }
}

void CameraPreview::addFilterOne(const dlib::full_object_detection& faceLandmark) {
    
    int x = faceLandmark.part(49).x() * 4;
    int y = faceLandmark.part(36).y() * 4;
    
    if (loadAndResizeFilter) {
        
        moustacheImage = cv::imread("Filters/moustache.png");
        
        int x = faceLandmark.part(55).x();
        
        int imageLength = (faceLandmark.part(55).x() - faceLandmark.part(49).x()) * 4;
        int imageHeight = (faceLandmark.part(63).y() - faceLandmark.part(36).y()) * 4;
        
        cv::resize(moustacheImage, moustacheImage, cv::Size(imageLength, imageHeight));
        
        loadAndResizeFilter = false;
    
    }
    
    moustacheImage.copyTo(frame(cv::Rect(x, y, moustacheImage.cols, moustacheImage.rows)));

}

void CameraPreview::addFilterTwo(const dlib::full_object_detection& faceLandmark) {
    
}

void CameraPreview::addFilterThree(const dlib::full_object_detection& faceLandmark) {
    
    
}

void CameraPreview::addFilterFour(const dlib::full_object_detection& faceLandmark) {
    
}

void CameraPreview::addFilterFive(const dlib::full_object_detection& faceLandmark) {
    
}


