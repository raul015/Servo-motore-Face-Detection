#include "opencv4/opencv2/objdetect.hpp"
#include "opencv4/opencv2/highgui.hpp"
#include "opencv4/opencv2/imgproc.hpp"
#include "opencv4/opencv2/videoio.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );
/** Global variables */
CascadeClassifier face_cascade;

/** @function main */
int main( int argc, const char** argv )
{
    
    CommandLineParser parser(argc, argv,
                             "{help h||}"
                             "{face_cascade|/home/raul/Scrivania/face_detection/haarcascade_frontalface_alt.xml|Path to face cascade.}"
                             "{camera|0|Camera device number.}");
                             

    parser.about( "\nThis program demonstrates using the cv::CascadeClassifier class to detect objects (Face) in a video stream.\n"
                  "You can use Haar or LBP features.\n\n" );
    parser.printMessage();

    String face_cascade_name = samples::findFile( parser.get<String>("face_cascade") );
    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
 
    int camera_device = parser.get<int>("camera");
    VideoCapture capture;
    //-- 2. Read the video stream
    capture.open( camera_device );
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }

    Mat frame;
    
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }

        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );

        if( waitKey(10) == 27 )
        {
            break; // escape
        }
    }
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    Scalar green(0,255,0); 
    Scalar red(0,0,255); 

    //-- Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale( frame_gray, faces );

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        

        if(faces[i].width>150 && faces[i].height>150)
        {

        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 0 ), 4 );

        Point p3 (faces[i].x+ faces[i].width/2,faces[i].y+ faces[i].height/2);
        Point p2 (0,faces[i].y+ faces[i].height/2);

        Point p4 (faces[i].x + faces[i].width/2,faces[i].y+ faces[i].height/2);
        Point p1(640,faces[i].y+ faces[i].height/2);

        int thicknessLine = 2;
        if(faces[i].x+faces[i].width/2<250 )    {
            arrowedLine(frame, p3, p2, red, thicknessLine);
            arrowedLine(frame, p4, p1, green, thicknessLine);
                                                }
        if(faces[i].x+faces[i].width/2 >400)   {
            arrowedLine(frame, p4, p1, red, thicknessLine);
            arrowedLine(frame, p3, p2, green, thicknessLine);
                                                    }
        else if(faces[i].x+faces[i].width/2>=250 && faces[i].x+faces[i].width/2 <=400)    {
            arrowedLine(frame, p3, p2, green, thicknessLine);
            arrowedLine(frame, p4, p1, green, thicknessLine);
                                                                                            }
         string disp = "distanza: " + std::to_string(faces[i].x+faces[i].width/2);
         putText(frame, disp, Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 200, 200), 4);

        }

    }

    //-- Show what you got
    imshow( "Capture - Face detection", frame );
}
