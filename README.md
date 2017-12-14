# Face Filter App

* An app that add image filters on faces using the laptop webcam
* Easy to use GUI with multiple filters to choose from and an option to capture and save the images


Requirements
* Opencv 
* QT5
* dlib 

How it works

* Opencv is used to capture a live feed from the webcam
* The frames extracted from the webcam are then sent to a Frontal Face Detector which detects faces
* Each face detected is passed to a Shape Predictor that is trained to provide 68 landmarks on the human face
* The 68 landmarks are then used to get the size of the filter image and adjust it on the face
* To reduce processing time per frame, the filter are resized only when the buttons are clicked, so the user has to re-click the button to adjust the filter size if needed. 

Created by
Farida Eid & Omar El Nawawy

Fall 2017 