/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 16 Jan 2022 1:08:15am
    Author:  Keepin' Soul

  ==============================================================================
*/

#pragma once

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel(){};
    virtual void drawRotarySlider    (juce::Graphics &     g,
                                     int     x,
                                     int     y,
                                     int     width,
                                     int     height,
                                     float     sliderPosProportional,
                                     float     rotaryStartAngle,
                                     float     rotaryEndAngle,
                                     juce::Slider &     slider) override
    {
        juce::Image knob = juce::ImageCache::getFromMemory (BinaryData::knob_juno_red_png, BinaryData::knob_juno_red_pngSize);
        const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
        const int nFrames = knob.getHeight()/knob.getWidth(); // number of frames
        const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
        
        const float radius = juce::jmin (width / 2.0f, height / 2.0f) ;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius - 1.0f;
        const float ry = centreY - radius;
        // draw image draws part of an image, rescaling it to fit in a given target region. The specified area of the source image is rescaled and drawn to fill the specified destination rectangle.
        g.drawImage (knob,                      //const Image &imageToDraw. the image to overlay
                     (int) rx,                  //int destX. the left of the destination rectangle
                     (int) ry,                  //int destY. the top of the destination rectangle
                     2 * (int)radius,//width,                     //int destWidth. the width of the destination rectangle
                     2 * (int)radius,//height,                    //int destHeight. the height of the destination rectangle
                     0,                         //int sourceX. the left of the rectangle to copy from the source image
                     frameIdx*knob.getWidth(),  //int sourceY. the top of the rectangle to copy from the source image
                     knob.getWidth(),           //int sourceWidth
                     knob.getWidth(),           //int sourceHeight
                     false);                    //bool fillAlphaChannelWithCurrentBrush=false
    };
    
    virtual void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                           float sliderPos,
                                           float minSliderPos,
                                           float maxSliderPos,
                                           const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        juce::Image fader = juce::ImageCache::getFromMemory (BinaryData::fader_juno_white_png, BinaryData::fader_juno_white_pngSize);
        //const double move = (sliderPos - minSliderPos) / (maxSliderPos - minSliderPos);

        const double moveFraction = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
        const int nFrames = 128; // number of frames
        const int frameIdx = (int)ceil(moveFraction * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
        
        int faderHeight = fader.getHeight() / nFrames;
        int posY = frameIdx * faderHeight;
        
        int sliderWidth = slider.getWidth();
        int sliderHeight = slider.getHeight();
        
        g.drawImage (fader,         //const Image &imageToDraw. the image to overlay
        0,                          //int destX. the left of the destination rectangle
        0,                          //int destY. the top of the destination rectangle
        sliderWidth,                //int destWidth. the width of the destination rectangle
        sliderHeight,               //int destHeight. the height of the destination rectangle
        0,                          //int sourceX. the left of the rectangle to copy from the source image
        posY,                       //int sourceY. the top of the rectangle to copy from the source image
        fader.getWidth(),                //int sourceWidth
        faderHeight,                //int sourceHeight
        false);                     //bool fillAlphaChannelWithCurrentBrush=false
        
        
        /*int centerX = x + width /2;
        int centerY = sliderPos - fader.getHeight()/128/2;
        
        g.drawImageAt(fader,
                      centerX - fader.getWidth()/2,
                      centerY - fader.getHeight()/2);*/
    }
    
    virtual void drawToggleButton (juce::Graphics& g,
                                   juce::ToggleButton& button,
                                   bool shouldDrawButtonAsHighlighted,
                                   bool shouldDrawButtonAsDown) override
    {
        juce::Image junoButtonOn = juce::ImageCache::getFromMemory (BinaryData::button_juno_on_png, BinaryData::button_juno_on_pngSize);
        juce::Image junoButtonOff = juce::ImageCache::getFromMemory (BinaryData::button_juno_off_png, BinaryData::button_juno_off_pngSize);
        juce::Image junoButton;
        
//        if(shouldDrawButtonAsDown)
//        {
//            junoButton = junoButtonOn;
//        }
//        else
//        {
//            junoButton = junoButtonOff;
//        }
        if(button.getState() == juce::Button::ButtonState::buttonDown)
            junoButton = junoButtonOn;
        else
            junoButton = junoButtonOff;
                
        
        g.drawImage (junoButton,                      //const Image &imageToDraw. the image to overlay
                    0,                  //int destX. the left of the destination rectangle
                    0,                  //int destY. the top of the destination rectangle
                    button.getWidth(),//width,                     //int destWidth. the width of the destination rectangle
                    button.getHeight(),//height,                    //int destHeight. the height of the destination rectangle
                    0,                         //int sourceX. the left of the rectangle to copy from the source image
                    0,  //int sourceY. the top of the rectangle to copy from the source image
                    junoButton.getWidth(),           //int sourceWidth
                    junoButton.getHeight(),           //int sourceHeight
                    false);                    //bool fillAlphaChannelWithCurrentBrush=false
    }
};
