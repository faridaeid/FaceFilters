#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void overlayImage(const cv::Mat &background, const cv::Mat &foreground, cv::Mat &output, cv::Point2i location)
{
  background.copyTo(output);


  // start at the row indicated by location, or at row 0 if location.y is negative.
  for(int y = std::max(location.y , 0); y < background.rows; ++y)
  {
    int fY = y - location.y; // because of the translation

    // we are done of we have processed all rows of the foreground image.
    if(fY >= foreground.rows)
      break;

    // start at the column indicated by location, 

    // or at column 0 if location.x is negative.
    for(int x = std::max(location.x, 0); x < background.cols; ++x)
    {
      int fX = x - location.x; // because of the translation.

      // we are done with this row if the column is outside of the foreground image.
      if(fX >= foreground.cols)
        break;

      // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
      double opacity =
        ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])

        / 255.;


      // and now combine the background and foreground pixel, using the opacity, 

      // but only if opacity > 0.
      for(int c = 0; opacity > 0 && c < output.channels(); ++c)
      {
        unsigned char foregroundPx =
          foreground.data[fY * foreground.step + fX * foreground.channels() + c];
        unsigned char backgroundPx =
          background.data[y * background.step + x * background.channels() + c];
        output.data[y*output.step + output.channels()*x + c] =
          backgroundPx * (1.-opacity) + foregroundPx * opacity;
      }
    }
  }
}


/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
String nose_cascade_name = "haarcascade_mcs_nose.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
CascadeClassifier noses_cascade;

Mat imgMustache = imread("mustache.png" , IMREAD_UNCHANGED);  
Mat imgGlasses = imread("glasses.png" , IMREAD_UNCHANGED);    
  
int mustacheHeight = imgMustache.rows;
int mustacheWidth = imgMustache.cols;


int glassesHeight = imgGlasses.rows;
int glassesWidth = imgGlasses.cols;

String window_name = "Capture - Face detection";

/** @function main */
int main( void )
{
    VideoCapture capture;
    Mat frame;

    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("Error loading face cascade\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("Error loading eyes cascade\n"); return -1; };
    if( !noses_cascade.load( nose_cascade_name ) ){ printf("Error loading noss cascade\n"); return -1; };

    //-- 2. Read the video stream
    capture.open( -1 );
    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }



    while (  capture.read(frame) )
    {
        if( frame.empty() )
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }

        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );

        int c = waitKey(10);
        if( (char)c == 27 ) { break; } // escape
    }
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat out;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

    for( size_t i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;
        std::vector<Rect> noses;


        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30));
        noses_cascade.detectMultiScale(faceROI, noses, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30));


        int newGlassesWidth, newGlassesHeight;
        int x1_eye, x2_eye, y1_eye, y2_eye;
        Mat glasses;
        for( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            // int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            // circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );

            newGlassesWidth = 3 * eyes[j].width;
            newGlassesHeight = newGlassesWidth * glassesHeight / glassesWidth;

            x1_eye = eyes[j].x - (newGlassesWidth/4);
            x2_eye = eyes[j].x + eyes[j].width + (newGlassesWidth/4);
            y1_eye = eyes[j].y + eyes[j].height - (newGlassesHeight/2);
            y2_eye = eyes[j].y + eyes[j].height + (newGlassesHeight/2);

            // if (x1 < 0)
            //     x1 = 0;
            
            // if (y1 < 0)
            //     y1 = 0;
            
            // if (x2 > faces[i].width)
            //     x2 = faces[i].width;
            
            // if (y2 > faces[i].height)
            //     y2 = faces[i].height;

            // newMustacheWidth = x2 - x1;
            // newMustacheHeight = y2 - y1;


            // resize(imgMustache, must, Size(newMustacheWidth, newMustacheHeight), INTER_AREA);




        }

        int newMustacheWidth, newMustacheHeight;
        int x1, x2, y1, y2;
        Mat must;
        for( size_t j = 0; j < noses.size(); j++ )
        {
            Point nose_center( faces[i].x + noses[j].x + noses[j].width/2, faces[i].y + noses[j].y + noses[j].height/2);
            // int radius = cvRound( (noses[j].width + noses[j].height)*0.25);
            // circle( frame, nose_center, radius, Scalar( 250, 0, 50 ), 4, 8, 0);

            newMustacheWidth = 3 * noses[j].width;
            newMustacheHeight = newMustacheWidth * mustacheHeight / mustacheWidth;

            x1 = noses[j].x - (newMustacheWidth/4);
            x2 = noses[j].x + noses[j].width + (newMustacheWidth/4);
            y1 = noses[j].y + noses[j].height - (newMustacheHeight/2);
            y2 = noses[j].y + noses[j].height + (newMustacheHeight/2);
 
    
            if (x1 < 0)
                x1 = 0;
            
            if (y1 < 0)
                y1 = 0;
            
            if (x2 > faces[i].width)
                x2 = faces[i].width;
            
            if (y2 > faces[i].height)
                y2 = faces[i].height;

            newMustacheWidth = x2 - x1;
            newMustacheHeight = y2 - y1;


            resize(imgMustache, must, Size(newMustacheWidth, newMustacheHeight), INTER_AREA);
            // printf("W : %d, H : %d" ,must.rows, must.cols);

            int m_x, m_y;

            m_y = nose_center.y;
            m_x = nose_center.x - newMustacheWidth/2;

            if((m_x + must.cols) >=0 && (m_x + must.cols) <= frame.cols && (m_y + must.rows) <= frame.rows && (m_y + must.rows) >= 0 && must.rows <= frame.rows && must.cols <= frame.cols)
            {
                // must.copyTo(frame(Rect(m_x, m_y ,must.cols, must.rows)));
                overlayImage(frame, must, frame, Point(m_x, m_y));
            }
        }



    }
    //-- Show what you got
    imshow( window_name, frame);
}
