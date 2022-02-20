/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/**
*/
class SubstractiveSynthAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                               public juce::Slider::Listener,
                                               //public juce::ComboBox::Listener,
                                               private juce::MidiKeyboardStateListener
{
public:
    SubstractiveSynthAudioProcessorEditor (SubstractiveSynthAudioProcessor&);
    ~SubstractiveSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void sliderValueChanged (juce::Slider *slider) override;
    void handleNoteOn (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
    void updateToggleState (juce::Button* button, juce::String name);
    
private:
    
    CustomLookAndFeel customLook;
    SubstractiveSynthAudioProcessor& audioProcessor;

    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::background_png, BinaryData::background_pngSize);
    
    //Amp Envelope
    juce::Slider carrierASlider;
    juce::Label  carrierALabel;
    juce::Slider carrierDSlider;
    juce::Label  carrierDLabel;
    juce::Slider carrierSSlider;
    juce::Label  carrierSLabel;
    juce::Slider carrierRSlider;
    juce::Label  carrierRLabel;
    
    //Filter
    juce::Slider cutKnob;
    juce::Label cutLabel;
    juce::Slider resKnob;
    juce::Label resLabel;

    //Fitler envelope
    juce::Slider envKnob;
    juce::Label envLabel;
    juce::Slider filterASlider;
    juce::Label  filterALabel;
    juce::Slider filterDSlider;
    juce::Label  filterDLabel;
    juce::Slider filterSSlider;
    juce::Label  filterSLabel;
    juce::Slider filterRSlider;
    juce::Label  filterRLabel;

    juce::MidiKeyboardState keyboardState;
    juce::MidiKeyboardComponent keyboardComponent;
    
    //waveform buttons
    juce::ToggleButton pulseButton1;
    juce::ToggleButton sawButton1;
    juce::ToggleButton sinButton1;
    juce::ToggleButton pulseButton2;
    juce::ToggleButton sawButton2;
    juce::ToggleButton sinButton2;
    int waveLed1;
    int waveLed2;
    juce::Image ledOn = juce::ImageCache::getFromMemory (BinaryData::led_on_png, BinaryData::led_on_pngSize);
    juce::Image ledOff = juce::ImageCache::getFromMemory (BinaryData::led_off_png, BinaryData::led_off_pngSize);
    juce::Label waveLabel;
    
    //mixer
    juce::Slider mixSlider;
    juce::Label mixLabel1;
    juce::Label mixLabel2;
    
    //pitch control
    juce::Label octLabel;
    juce::Label semiLabel;
    juce::Label fineLabel;
    juce::Slider octKnob1;
    juce::Slider semiKnob1;
    juce::Slider fineKnob1;
    juce::Slider octKnob2;
    juce::Slider semiKnob2;
    juce::Slider fineKnob2;
    
    //lfo
    juce::Slider lfoFreqSlider;
    juce::Slider lfoAmpSlider;
    juce::Slider lfoFilterSlider;
    juce::Label lfoFreqLabel;
    juce::Label lfoAmpLabel;
    juce::Label lfoFilterLabel;
    
    //delay
    juce::Slider delayTimeSlider;
    juce::Slider delayFeedSlider;
    juce::Slider delayDrySlider;
    juce::ToggleButton delaySyncButton;
    bool delaySyncLed;
    juce::Label delayTimeLabel;
    juce::Label delayFeedLabel;
    juce::Label delayDryLabel;
    juce::Label delaySyncLabel;
    
    //logo
    juce::Image logo = juce::ImageCache::getFromMemory (BinaryData::logo_png, BinaryData::logo_pngSize);
    
    
    juce::AudioDeviceManager deviceManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubstractiveSynthAudioProcessorEditor)
};
