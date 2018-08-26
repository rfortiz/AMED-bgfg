
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include "bgfg_amed.hpp"

TEST(BackgroundSubtractorAMED, CustomHyperparamtersInit)
{
    cv::Ptr<cv::BackgroundSubtractorAMED> pAMED;
    pAMED = cv::createBackgroundSubtractorAMED(11, 7, true, true);
    
    EXPECT_DOUBLE_EQ(pAMED->getThreshold(), 11.0);
    EXPECT_EQ(pAMED->getSegBlurSize(), 7);
    EXPECT_EQ(pAMED->getMaskedUpdate(), true);
    
    pAMED = cv::createBackgroundSubtractorAMED(11, 7, true, false);
    EXPECT_EQ(pAMED->getMaskedUpdate(), false);
}

TEST(BackgroundSubtractorAMED, HyperparamtersSet)
{
    cv::Ptr<cv::BackgroundSubtractorAMED> pAMED;
    pAMED = cv::createBackgroundSubtractorAMED(3, 1, true, false);
    
    pAMED->setThreshold(11);
    pAMED->setSegBlurSize(7);
    pAMED->setMaskedUpdate(true);
    
    EXPECT_DOUBLE_EQ(pAMED->getThreshold(), 11.0);
    EXPECT_EQ(pAMED->getSegBlurSize(), 7);
    EXPECT_EQ(pAMED->getMaskedUpdate(), true);
}

TEST(BackgroundSubtractorAMED, TestApproxMedian){
    uchar data[4] = { 15, 240, 45, 174 };
    cv::Mat frame1 = cv::Mat(2, 2, CV_8U, data);
    uchar data2[4] = { 221, 63, 45, 57 };
    cv::Mat frame2 = cv::Mat(2, 2, CV_8U, data2);
    cv::Mat foreground, background;
    
    cv::Ptr<cv::BackgroundSubtractorAMED> pAMED;
    // seg_threshold=1, blur_size=1
    pAMED = cv::createBackgroundSubtractorAMED(1,1);
    
    // given enough frames, with 0.1 refresh rate background should converge to the median of repeated 3 frames sequence
    // last frame is foreground
    for(size_t i=0; i<=5004; i++){
        
        if( i%3 == 0){
            pAMED->apply(frame2, foreground,0.1);
        }
        else{
            pAMED->apply(frame1, foreground,0.1);
        }
    }
    pAMED->getBackgroundImage(background);
    
    EXPECT_EQ(background.at<uchar>(0,0), 15);
    EXPECT_EQ(background.at<uchar>(0,1), 240);
    EXPECT_EQ(background.at<uchar>(1,0), 45);
    EXPECT_EQ(background.at<uchar>(1,1), 174);
    
    EXPECT_EQ(foreground.at<uchar>(0,0), 255);
    EXPECT_EQ(foreground.at<uchar>(0,1), 255);
    EXPECT_EQ(foreground.at<uchar>(1,0), 0);
    EXPECT_EQ(foreground.at<uchar>(1,1), 255);
}

TEST(BackgroundSubtractorAMED, TestBackgroundType){
    // greyscale
    cv::Mat frame = cv::Mat(2, 2, CV_8U);
    randu(frame, cv::Scalar::all(0), cv::Scalar::all(255));
    cv::Mat foreground, background;
    cv::Ptr<cv::BackgroundSubtractorAMED> pAMED;
    pAMED = cv::createBackgroundSubtractorAMED();
    pAMED->apply(frame, foreground,0.1);
    pAMED->getBackgroundImage(background);
    
    EXPECT_EQ(background.type(), CV_8U);
    
    // color
    frame = cv::Mat(2, 2, CV_8UC3);
    randu(frame, cv::Scalar::all(0), cv::Scalar::all(255));
    pAMED = cv::createBackgroundSubtractorAMED();
    pAMED->apply(frame, foreground,0.1);
    pAMED->getBackgroundImage(background);
    
    EXPECT_EQ(background.type(), CV_8UC3);
    
}





