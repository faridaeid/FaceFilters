//
//  main.cpp
//  FaceFilters
//
//  Created by Farida Eid on 12/8/17.
//  Copyright © 2017 Farida Eid. All rights reserved.
//

#include <QApplication>
#include "src/CameraPreview.h"

int main(int argc, char *argv[]) {
    
    QApplication application(argc, argv);
    CameraPreview cameraPreview;
    
    cameraPreview.setStyleSheet("QMainWindow {background-color: #000000;}");
    
    cameraPreview.resizeWindowWithFrame(1.2);
        
    cameraPreview.show();
    
    return application.exec();
    
}
