#include "bgfg_amed.hpp"

const static double default_learning_rate = 1;

namespace cv {
    
class BackgroundSubtractorAMEDImpl: public BackgroundSubtractorAMED{
    public:
       
        BackgroundSubtractorAMEDImpl(){}
        BackgroundSubtractorAMEDImpl(double _seg_threshold=20, int _seg_blur_size=3, bool _ramp_init=true, bool _masked_update=false);
        virtual ~BackgroundSubtractorAMEDImpl() {}
    
        // BackgroundSubtractor interface
        virtual void apply(InputArray image, OutputArray fgmask, double learningRate);
        virtual void getBackgroundImage(OutputArray backgroundImage) const;
        
        // BackgroundSubtractorAMED interface
        virtual double getThreshold() const{return seg_threshold;}
        virtual void setThreshold(double _seg_threshold){seg_threshold = _seg_threshold;}
        
        virtual int getSegBlurSize() const{return seg_blur_size;}
        virtual void setSegBlurSize(int _seg_blur_size){seg_blur_size=_seg_blur_size;}
        
        virtual bool getMaskedUpdate() const{return masked_update;}
        virtual void setMaskedUpdate(bool _masked_update){masked_update=_masked_update;}
        
    private:
        void extractForeground(const Mat& current_frame, Mat& foreground_mask);
        
        double seg_threshold;
        int seg_blur_size;
        bool ramp_init;
        bool masked_update;
        double learning_rate{1};
        double ramp_learning_rate{5};
        bool is_init{false};
        int frame_type{0};
        
        Mat background;// background model converted to uchar (to compute difference with current frame and 'getBackgroundImage')
        Mat background_float;// background model
        Mat above_background;
        Mat above_background_float;
        Mat below_background;
        Mat below_background_float;
        
};
    
BackgroundSubtractorAMEDImpl::BackgroundSubtractorAMEDImpl(double _seg_threshold, int _seg_blur_size, bool _ramp_init, bool _masked_update)
:seg_threshold(_seg_threshold),
seg_blur_size(_seg_blur_size),
ramp_init(_ramp_init),
masked_update(_masked_update)
{}

void BackgroundSubtractorAMEDImpl::apply(InputArray _image, OutputArray _fgmask, double _learningRate){
    Mat current_frame = _image.getMat();
    
    // check if already initialized
    if(!is_init){         
        CV_Assert(current_frame.type() == CV_8UC1 || current_frame.type() == CV_8UC3);
        background = current_frame.clone();
        background.convertTo(background_float, CV_32FC3);
        frame_type = current_frame.type();
        is_init = true;
    }
    CV_Assert(frame_type == current_frame.type());
    CV_Assert(current_frame.size() == background.size());
    
    // prepare foreground mask ouput
    _fgmask.create(_image.size(), CV_8U); 
    Mat foreground_mask = _fgmask.getMat();
    
    // check if learning rate provided
    if(_learningRate >= 0){
        learning_rate = _learningRate;
    }
    else{
        learning_rate = default_learning_rate;
    }
    
    // check if finished ramping rate
    if(ramp_init && ramp_learning_rate > learning_rate){
        ramp_learning_rate = ramp_learning_rate/1.016;// reach the default learning in ~100 frames; 5/(1.016^100)
    }
    else{
        ramp_learning_rate = learning_rate;
    }
    
    // compute foreground before updating so it can be used as update mask
    extractForeground(current_frame, foreground_mask);
    
    // compare current frame to background
    above_background = current_frame > background;
    below_background = current_frame < background;
    above_background.convertTo(above_background_float, CV_32FC(current_frame.channels()) );
    below_background.convertTo(below_background_float, CV_32FC(current_frame.channels()) );
    
    // update background
    if(masked_update){ // don't update where there are foreground objects
        Mat update_mask;
        bitwise_not(foreground_mask, update_mask);
        add(background_float, above_background_float*(ramp_learning_rate /255.0), background_float, update_mask);
        subtract(background_float, below_background_float*(ramp_learning_rate /255.0), background_float, update_mask);
    }
    else{
        background_float = background_float + above_background_float*(ramp_learning_rate /255.0) - below_background_float*(ramp_learning_rate /255.0);
    }
    background_float.convertTo(background, current_frame.type());
    
}
void BackgroundSubtractorAMEDImpl::getBackgroundImage(OutputArray _backgroundImage) const{
    _backgroundImage.create(background.size(), background.type());
    Mat backgroundImage = _backgroundImage.getMat();
    background.copyTo(backgroundImage);
}

void BackgroundSubtractorAMEDImpl::extractForeground(const Mat& current_frame, Mat& foreground_mask){
    Mat diff;
    absdiff(current_frame, background, diff);
    
    if(diff.channels()==3){
        cvtColor(diff, diff, CV_BGR2GRAY);
    }
    
    CV_Assert(seg_blur_size > 0);
    blur(diff, diff, Size(seg_blur_size,seg_blur_size));
    threshold(diff, foreground_mask, seg_threshold, 255, THRESH_BINARY);
}

Ptr<BackgroundSubtractorAMED>createBackgroundSubtractorAMED(double seg_threshold, 
                                                                         int seg_blur_size, 
                                                                         bool ramp_init, 
                                                                         bool masked_update)
{
    return makePtr<BackgroundSubtractorAMEDImpl>(seg_threshold, seg_blur_size, ramp_init, masked_update);
}



    
} //cv
