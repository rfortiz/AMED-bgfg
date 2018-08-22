////////////////////////////////////////////////////////////////////////////////////
//The Clear BSD License
//
//Copyright (c) 2018 Raphael Ortiz
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted (subject to the limitations in the disclaimer
//below) provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//     * Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
//NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
//THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
//CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
//IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////////

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


