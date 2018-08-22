#include <opencv2/opencv.hpp>
#include <iostream>
#include "bgfg_amed.hpp"

using namespace std;
using namespace cv;

int main(int /*argc*/, char** /*argv[]*/)
{
    
    namedWindow("frame",CV_WINDOW_KEEPRATIO);
    namedWindow("foreground",CV_WINDOW_KEEPRATIO);
    
    Mat frame, foreground, foreground2;
    char key;
    
    VideoCapture cap(-1); 
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    
    
    Ptr<BackgroundSubtractor> pAMED;
    pAMED = createBackgroundSubtractorAMED(20, 3, true, true); // not updating where foreground object are present
    Ptr<BackgroundSubtractor> pAMED2;
    pAMED2 = createBackgroundSubtractorAMED(20, 5, true, false);
    
    int frame_id{1};
    cap >> frame;
    double t = (double)getTickCount(); // start timer
    while(!frame.empty()){
        imshow("frame", frame);
        
        // update both background models
        pAMED->apply(frame, foreground, 0.1); // slow update
        pAMED2->apply(frame, foreground2, 2); // fast update
        
        //combine ouput
        bitwise_and(foreground, foreground2,foreground);

        imshow("foreground", foreground);
        
        frame_id++;
        if(frame_id%30 == 0){
            t = ((double)getTickCount() - t)/getTickFrequency();
            cout << "\r" << 30/t << "\tFPS\t" << flush;
            t = (double)getTickCount(); // restart timer
        }
        cap >> frame;
        
        key=(char) waitKey(1);
        if(key == (char)27){
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    
    return 0;
}


