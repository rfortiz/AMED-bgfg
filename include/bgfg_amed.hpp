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
    @param If true, regions with foreground objects are are not updated. Only applies after ramping learning rate.
    Only small/gradual changes (i.e. below 'threshold') will be incorporated in the background.
    When true, objects removed from the background, will create permanent ghosts.
    
 */
Ptr<BackgroundSubtractorAMED>
    createBackgroundSubtractorAMED(double seg_threshold=20, int seg_blur_size=3, bool ramp_init=true, bool masked_update=false);

} //cv

#endif /* BGFG_AMED_HPP */ 
