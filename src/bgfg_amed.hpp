#ifndef BGFG_AMED_HPP
#define BGFG_AMED_HPP

#include "opencv2/video.hpp"

namespace cv {

/** @brief Adaptive median Background/Foreground Segmentation Algorithm.
 * 
 * The class implements the adaptive median background estimation described in
 * McFarlane, N.J. and Schofield, C.P., 1995. Segmentation and tracking of piglets in images. 
 * Machine vision and applications, 8(3), pp.187-193.
 * 
 * In addition it supports variable update rate and rate ramping for faster initialization in presence of foreground objects.
 */
class BackgroundSubtractorAMED: public BackgroundSubtractor {
    public:
        
        /// @see createBackgroundSubtractorAMED()
        CV_WRAP virtual double getThreshold() const = 0;
        
        /// @see createBackgroundSubtractorAMED()
        CV_WRAP virtual void setThreshold(double seg_threshold) = 0;
        
        /// @see createBackgroundSubtractorAMED()
        CV_WRAP virtual int getSegBlurSize() const = 0;
        
        /// @see createBackgroundSubtractorAMED()
        CV_WRAP virtual void setSegBlurSize(int seg_blur_size) = 0;
        
        /// @see createBackgroundSubtractorAMED()
        CV_WRAP virtual bool getMaskedUpdate() const = 0;
        
        /// @see createBackgroundSubtractorAMED()
        CV_WRAP virtual void setMaskedUpdate(bool masked_update) = 0;
    
    private:
        /* add your private declarations */
};

/** @brief Creates AMED Background Subtractor
    @param Intensity threshold to segment foreground objects.
    @param Size of box blur applied on abs(current_frame-background) before thresholding. 
    Reduces noise but reduces precision of object boundaries. Set to '1' to disable.
    @param If true, starts with high learning rate and ramps down for fast initialization. 
    set to 'false' if the first frame does not contain foreground objects.
    @param If true, regions with foreground objects are are not updated. 
    Only small/gradual changes (i.e. below 'threshold') will be incorporated in the background.
    When true, objects removed from the background, will create permanent ghosts.
    
 */
Ptr<BackgroundSubtractorAMED>
    createBackgroundSubtractorAMED(double seg_threshold=20, int seg_blur_size=3, bool ramp_init=true, bool masked_update=false);

} //cv

#endif /* BGFG_AMED_HPP */ 
