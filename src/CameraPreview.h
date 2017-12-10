#ifndef CAMERAPREVIEW_H
#define CAMERAPREVIEW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <opencv2/opencv.hpp>

namespace Ui {
class CameraPreview;
}

class CameraPreview : public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraPreview(QWidget *parent = 0);
    ~CameraPreview();

private:
    
    Ui::CameraPreview *ui;
    cv::VideoCapture videoCapture;
    cv::Mat frame;
    QTimer * frameTimer;
    
    
    void frameProcess();
    void startWebcam();
    
    void captureImage();
    void addFilterOne();
    void addFilterTwo();
    void addFilterThree();
    void addFilterFour();
    void addFilterFive();
    
};

#endif // CAMERAPREVIEW_H
