#ifndef CAMERAPREVIEW_H
#define CAMERAPREVIEW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <ctime>
#include <vector>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/opencv/cv_image.h>
#include <opencv2/opencv.hpp>

enum FilterSelected {
    
    NoFilter,
    MoustacheFilter,
    FlowerFilter,
    HarryPotterFilter    
};

namespace Ui {
class CameraPreview;
}

class CameraPreview : public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraPreview(QWidget *parent = 0);
    ~CameraPreview();
    
    void resizeWindowWithFrame(const double&);

private:
    
    Ui::CameraPreview *ui;
    cv::VideoCapture videoCapture;
    cv::Mat frame;
    cv::Mat moustacheImage;
    cv::Mat harryPotterImage[2];
    cv::Mat flowerImage;
    QTimer * frameTimer;
    dlib::frontal_face_detector frontalFaceDetector;
    dlib::shape_predictor shapePredictor;
    FilterSelected currentFilter;
    bool loadAndResizeFilter;
    
    void frameProcess();
    void startWebcam();
    void detectFaces();
    void captureImage();
    void stopFilters();
    void assignFilterSelected();
    void overlayFilter(const cv::Mat&, const cv::Mat&, cv::Mat&, cv::Point);
    
    void addFilterOne(const dlib::full_object_detection&);
    void addFilterTwo(const dlib::full_object_detection&);
    void addFilterThree(const dlib::full_object_detection&);
    
    
};

#endif // CAMERAPREVIEW_H
