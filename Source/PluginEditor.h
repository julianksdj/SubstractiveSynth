/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

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
    void waveMenuChanged();
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SubstractiveSynthAudioProcessor& audioProcessor;
    
    //==============================================================================
    
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
    
    //Waveform selection
    juce::Label waveLabel;
    juce::ComboBox waveMenu;

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
    
    juce::AudioDeviceManager deviceManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubstractiveSynthAudioProcessorEditor)
};
