/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubstractiveSynthAudioProcessorEditor::SubstractiveSynthAudioProcessorEditor (SubstractiveSynthAudioProcessor& p)
    : keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (620, 440);
    //customLook.setColour(juce::Slider::ColourIds::thumbColourId,juce::Colours::darkred);
    customLook.setColour(juce::Slider::ColourIds::thumbColourId,juce::Colour(165,60,55));
    customLook.setColour(juce::Slider::ColourIds::backgroundColourId,juce::Colour(94,87,83));
    customLook.setColour(juce::Slider::ColourIds::backgroundColourId,juce::Colour(94,87,83));
    customLook.setColour(juce::Slider::ColourIds::trackColourId,juce::Colour(128,118,111));
    
    //keyboard
    addAndMakeVisible (keyboardComponent);
    keyboardState.addListener (this);
    
    //attack slider
    carrierASlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierASlider);
    carrierASlider.setLookAndFeel(&customLook);
    // min  attack time must be 0.0001 (100us) to avoid attackSamples from being 0
    // so the division Aa = 0.125 / attackSamples is never inf
    carrierASlider.setNormalisableRange(juce::NormalisableRange<double>(0.0001f, 5.f, 0.0001f, 1.f));
    carrierASlider.addListener (this);
    carrierASlider.setValue(audioProcessor.getAttack());
    addAndMakeVisible (carrierALabel);
    carrierALabel.setText ("A", juce::dontSendNotification);
    carrierALabel.setJustificationType(juce::Justification::centred);

    //decay slider
    carrierDSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierDSlider);
    carrierDSlider.setLookAndFeel(&customLook);
    carrierDSlider.setNormalisableRange(juce::NormalisableRange<double>(0.0001f, 5.f, 0.0001f, 1.f));
    carrierDSlider.addListener (this);
    carrierDSlider.setValue(audioProcessor.getDecay());
    addAndMakeVisible (carrierDLabel);
    carrierDLabel.setText ("D", juce::dontSendNotification);
    carrierDLabel.setJustificationType(juce::Justification::centred);


    //sustain slider
    carrierSSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierSSlider);
    carrierSSlider.setLookAndFeel(&customLook);
    carrierSSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 1.f, 0.0001f, 1.f));
    carrierSSlider.addListener (this);
    carrierSSlider.setValue(audioProcessor.getSustain());
    addAndMakeVisible (carrierSLabel);
    carrierSLabel.setText ("S", juce::dontSendNotification);
    carrierSLabel.setJustificationType(juce::Justification::centred);
    
    //release slider
    carrierRSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierRSlider);
    carrierRSlider.setLookAndFeel(&customLook);
    carrierRSlider.setNormalisableRange(juce::NormalisableRange<double>(0.001f, 5.f, 0.0001f, 1.f));
    carrierRSlider.addListener (this);
    carrierASlider.setValue(audioProcessor.getRelease());
    addAndMakeVisible (carrierRLabel);
    carrierRLabel.setText ("R", juce::dontSendNotification);
    carrierRLabel.setJustificationType(juce::Justification::centred);
    
    //cutoff knob
    addAndMakeVisible (cutKnob);
    cutKnob.setLookAndFeel(&customLook);
    cutKnob.setNormalisableRange(juce::NormalisableRange<double>(20.f, 20000.f, 0.01f, 1.f));
    cutKnob.addListener (this);
    cutKnob.setValue(audioProcessor.getCut());
    cutKnob.setSkewFactorFromMidPoint(1000);
    cutKnob.setTextValueSuffix (" Hz");
    cutLabel.setText ("Cut", juce::dontSendNotification);
    cutLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (cutLabel);
    cutKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    cutKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);
    
    //resonance knob
    addAndMakeVisible (resKnob);
    resKnob.setLookAndFeel(&customLook);
    resKnob.setNormalisableRange(juce::NormalisableRange<double>(0.1f, 4.f, 0.01f, 1.f));
    resKnob.addListener (this);
    resKnob.setValue(audioProcessor.getRes());
    resLabel.setText ("Res", juce::dontSendNotification);
    resLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (resLabel);
    resKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    resKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);
    
    //env knob
    addAndMakeVisible (envKnob);
    envKnob.setLookAndFeel(&customLook);
    envKnob.setNormalisableRange(juce::NormalisableRange<double>(0.f, 1.f, 0.01f, 1.f));
    envKnob.addListener (this);
    envKnob.setValue(audioProcessor.getFilterEnv());
    envLabel.setText ("Env", juce::dontSendNotification);
    envLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (envLabel);
    envKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    envKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);

    //waveform buttons
    waveLed1 = 1;
    waveLed2 = 1;
    addAndMakeVisible (pulseButton1);
    pulseButton1.setLookAndFeel(&customLook);
    pulseButton1.onClick = [this] { updateToggleState (&pulseButton1, "Pulse1"); };
    addAndMakeVisible (sawButton1);
    sawButton1.setLookAndFeel(&customLook);
    sawButton1.onClick = [this] { updateToggleState (&sawButton1, "Saw1"); };
    addAndMakeVisible (sinButton1);
    sinButton1.setLookAndFeel(&customLook);
    sinButton1.onClick = [this] { updateToggleState (&sinButton1, "Sine1"); };
    addAndMakeVisible (pulseButton2);
    pulseButton2.setLookAndFeel(&customLook);
    pulseButton2.onClick = [this] { updateToggleState (&pulseButton2, "Pulse2"); };
    addAndMakeVisible (sawButton2);
    sawButton2.setLookAndFeel(&customLook);
    sawButton2.onClick = [this] { updateToggleState (&sawButton2, "Saw2"); };
    addAndMakeVisible (sinButton2);
    sinButton2.setLookAndFeel(&customLook);
    sinButton2.onClick = [this] { updateToggleState (&sinButton2, "Sine2"); };
    addAndMakeVisible(waveLabel);
    waveLabel.setText("Wave", juce::dontSendNotification);
    waveLabel.setJustificationType(juce::Justification::centred);
    
    //filter attack slider
    filterASlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (filterASlider);
    filterASlider.setLookAndFeel(&customLook);
    // min  attack time must be 0.0001 (100us) to avoid attackSamples from being 0
    // so the division Aa = 0.125 / attackSamples is never inf
    filterASlider.setNormalisableRange(juce::NormalisableRange<double>(0.0001f, 5.f, 0.0001f, 1.f));
    filterASlider.addListener (this);
    filterASlider.setValue(audioProcessor.getFilterAttack());
    addAndMakeVisible (filterALabel);
    filterALabel.setText ("A", juce::dontSendNotification);
    filterALabel.setJustificationType(juce::Justification::centred);

    //filter decay slider
    filterDSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (filterDSlider);
    filterDSlider.setLookAndFeel(&customLook);
    filterDSlider.setNormalisableRange(juce::NormalisableRange<double>(0.0001f, 5.f, 0.0001f, 1.f));
    filterDSlider.addListener (this);
    filterDSlider.setValue(audioProcessor.getFilterDecay());
    addAndMakeVisible (filterDLabel);
    filterDLabel.setText ("D", juce::dontSendNotification);
    filterDLabel.setJustificationType(juce::Justification::centred);

    
    //filter sustain slider
    filterSSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (filterSSlider);
    filterSSlider.setLookAndFeel(&customLook);
    filterSSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 1.f, 0.0001f, 1.f));
    filterSSlider.setValue(20000.0);
    filterSSlider.addListener (this);
    filterSSlider.setValue(audioProcessor.getFilterSustain());
    addAndMakeVisible (filterSLabel);
    filterSLabel.setText ("S", juce::dontSendNotification);
    filterSLabel.setJustificationType(juce::Justification::centred);

    
    //filter release slider
    filterRSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (filterRSlider);
    filterRSlider.setLookAndFeel(&customLook);
    filterRSlider.setNormalisableRange(juce::NormalisableRange<double>(0.001f, 5.f, 0.0001f, 1.f));
    filterRSlider.addListener (this);
    carrierASlider.setValue(audioProcessor.getFilterRelease());
    addAndMakeVisible (filterRLabel);
    filterRLabel.setText ("R", juce::dontSendNotification);
    filterRLabel.setJustificationType(juce::Justification::centred);

    //mix slider
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (mixSlider);
    mixSlider.setLookAndFeel(&customLook);
    mixSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 1.f, 0.001f, 1.f));
    mixSlider.addListener (this);
    mixSlider.setValue(audioProcessor.getMix());
    addAndMakeVisible(mixLabel1);
    mixLabel1.setText("O1", juce::dontSendNotification);
    mixLabel1.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel2);
    mixLabel2.setText("O2", juce::dontSendNotification);
    mixLabel2.setJustificationType(juce::Justification::centred);
    
    //octave knob osc 1
    addAndMakeVisible (octKnob1);
    octKnob1.setLookAndFeel(&customLook);
    octKnob1.setNormalisableRange(juce::NormalisableRange<double>(-2.f, 2.f, 1.f, 1.f));
    octKnob1.addListener (this);
    octKnob1.setValue(audioProcessor.getOct(0));
    octKnob1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    octKnob1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);
    
    //semitone knob osc 1
    addAndMakeVisible (semiKnob1);
    semiKnob1.setLookAndFeel(&customLook);
    semiKnob1.setNormalisableRange(juce::NormalisableRange<double>(-12.f, 12.f, 1.f, 1.f));
    semiKnob1.addListener (this);
    semiKnob1.setValue(audioProcessor.getSemi(0));
    semiKnob1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    semiKnob1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);
    
    //fine knob osc 1
    addAndMakeVisible (fineKnob1);
    fineKnob1.setLookAndFeel(&customLook);
    fineKnob1.setNormalisableRange(juce::NormalisableRange<double>(-50.f, 50.f, 1.f, 1.f));
    fineKnob1.addListener (this);
    fineKnob1.setValue(audioProcessor.getFine(0));
    fineKnob1.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    fineKnob1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);
    
    //octave knob osc 2
    addAndMakeVisible (octKnob2);
    octKnob2.setLookAndFeel(&customLook);
    octKnob2.setNormalisableRange(juce::NormalisableRange<double>(-2.f, 2.f, 1.f, 1.f));
    octKnob2.addListener (this);
    octKnob2.setValue(audioProcessor.getOct(1));
    octKnob2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    octKnob2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);
    
    //semitone knob osc 2
    addAndMakeVisible (semiKnob2);
    semiKnob2.setLookAndFeel(&customLook);
    semiKnob2.setNormalisableRange(juce::NormalisableRange<double>(-12.f, 12.f, 1.f, 1.f));
    semiKnob2.addListener (this);
    semiKnob2.setValue(audioProcessor.getSemi(1));
    semiKnob2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    semiKnob2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);
    
    //fine knob osc 2
    addAndMakeVisible (fineKnob2);
    fineKnob2.setLookAndFeel(&customLook);
    fineKnob2.setNormalisableRange(juce::NormalisableRange<double>(-50.f, 50.f, 1.f, 1.f));
    fineKnob2.addListener (this);
    fineKnob2.setValue(audioProcessor.getFine(1));
    fineKnob2.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    fineKnob2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 70, 14);
    
    //pitch labels
    addAndMakeVisible(octLabel);
    octLabel.setText("Oct", juce::dontSendNotification);
    octLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(semiLabel);
    semiLabel.setText("Semi", juce::dontSendNotification);
    semiLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(fineLabel);
    fineLabel.setText("Det", juce::dontSendNotification);
    fineLabel.setJustificationType(juce::Justification::centred);
    
    //lfo freq
    lfoFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (lfoFreqSlider);
    lfoFreqSlider.setLookAndFeel(&customLook);
    lfoFreqSlider.setNormalisableRange(juce::NormalisableRange<double>(0.1f, 50.f, 0.01f, 1.f));
    lfoFreqSlider.addListener (this);
    lfoFreqSlider.setValue(audioProcessor.getLfoFreq());
    addAndMakeVisible (lfoFreqLabel);
    lfoFreqLabel.setText ("LFr", juce::dontSendNotification);
    lfoFreqLabel.setJustificationType(juce::Justification::centred);
    
    //lfo amp
    lfoAmpSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (lfoAmpSlider);
    lfoAmpSlider.setLookAndFeel(&customLook);
    lfoAmpSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 1.f, 0.01f, 1.f));
    lfoAmpSlider.addListener (this);
    lfoAmpSlider.setValue(audioProcessor.getLfoAmp());
    addAndMakeVisible (lfoAmpLabel);
    lfoAmpLabel.setText ("LFa", juce::dontSendNotification);
    lfoAmpLabel.setJustificationType(juce::Justification::centred);
    
    //lfo filter
    lfoFilterSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (lfoFilterSlider);
    lfoFilterSlider.setLookAndFeel(&customLook);
    lfoFilterSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 1.f, 0.01f, 1.f));
    lfoFilterSlider.addListener (this);
    //lfoFilterSlider.setValue(audioProcessor.getLfoFilter());
    addAndMakeVisible (lfoFilterLabel);
    lfoFilterLabel.setText ("LFf", juce::dontSendNotification);
    lfoFilterLabel.setJustificationType(juce::Justification::centred);
    
    //delayTime
    delayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (delayTimeSlider);
    delayTimeSlider.setLookAndFeel(&customLook);
    delayTimeSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 2000.f, 1.f, 1.f));
    delayTimeSlider.addListener (this);
    delayTimeSlider.setValue(audioProcessor.getDelayTime());
    addAndMakeVisible (delayTimeLabel);
    delayTimeLabel.setText ("DT", juce::dontSendNotification);
    delayTimeLabel.setJustificationType(juce::Justification::centred);
    
    //delayFeedback
    delayFeedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (delayFeedSlider);
    delayFeedSlider.setLookAndFeel(&customLook);
    delayFeedSlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 100.f, 1.f,  1.f));
    delayFeedSlider.addListener (this);
    delayFeedSlider.setValue(audioProcessor.getDelayFeed());
    addAndMakeVisible (delayFeedLabel);
    delayFeedLabel.setText ("FB", juce::dontSendNotification);
    delayFeedLabel.setJustificationType(juce::Justification::centred);
    
    //delayDryWet
    delayDrySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (delayDrySlider);
    delayDrySlider.setLookAndFeel(&customLook);
    delayDrySlider.setNormalisableRange(juce::NormalisableRange<double>(0.f, 100.f, 1.f,  1.f));
    delayDrySlider.addListener (this);
    delayDrySlider.setValue(audioProcessor.getDelayDry());
    addAndMakeVisible (delayDryLabel);
    delayDryLabel.setText ("DW", juce::dontSendNotification);
    delayDryLabel.setJustificationType(juce::Justification::centred);
    /* //delay BPM sync, future feature
    addAndMakeVisible (delaySyncButton);
    delaySyncButton.setLookAndFeel(&customLook);
    delaySyncButton.onClick = [this] { updateToggleState (&delaySyncButton, "Sync"); };
    addAndMakeVisible(delaySyncLabel);
    delaySyncLabel.setText("Sy", juce::dontSendNotification);
    delaySyncLabel.setJustificationType(juce::Justification::centred);
     */
}

SubstractiveSynthAudioProcessorEditor::~SubstractiveSynthAudioProcessorEditor()
{
}

//==============================================================================
void SubstractiveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour (178, 176, 255));
    g.setColour(juce::Colours::white);
    g.setFont (15.0f);
    g.drawImageAt (background, 0, 0);
    g.drawImage(logo, 20, 345, 120, 27, 0, 0, 600, 135);
    int ledWidth = 18;
    int ledSWidth = 14;

    if (waveLed1 == 1)
    {
        g.drawImage(ledOn, 31, 66, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOff, 71, 66, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOff, 111, 66, ledWidth, ledWidth, 0, 0, 80, 80);
    }
    else if (waveLed1 == 2)
    {
        g.drawImage(ledOff, 31, 66, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOn, 71, 66, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOff, 111, 66, ledWidth, ledWidth, 0, 0, 80, 80);
    }
    else if (waveLed1 == 3)
    {
        g.drawImage(ledOff, 31, 66, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOff, 71, 66, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOn, 111, 66, ledWidth, ledWidth, 0, 0, 80, 80);
    }
    
    if (waveLed2 == 1)
    {
        g.drawImage(ledOn, 31, 136, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOff, 71, 136, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOff, 111, 136, ledWidth, ledWidth, 0, 0, 80, 80);
    }
    else if (waveLed2 == 2)
    {
        g.drawImage(ledOff, 31, 136, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOn, 71, 136, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOff, 111, 136, ledWidth, ledWidth, 0, 0, 80, 80);
    }
    else if (waveLed2 == 3)
    {
        g.drawImage(ledOff, 31, 136, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOff, 71, 136, ledWidth, ledWidth, 0, 0, 80, 80);
        g.drawImage(ledOn, 111, 136, ledWidth, ledWidth, 0, 0, 80, 80);
    }
    /* //delay BPM sync, future feature
    if (delaySyncLed)
    {
        g.drawImage(ledOn, 33, 280, ledSWidth, ledSWidth, 0, 0, 80, 80);
    }
    else
    {
        g.drawImage(ledOff, 33, 280, ledSWidth, ledSWidth, 0, 0, 80, 80);
    }
     */
}

void SubstractiveSynthAudioProcessorEditor::resized()
{
    int keyboardHeight = 60;
    int keyboardWidth = getWidth();
    
    int sliderWidth = 30;
    int sliderHeight = 120;
    int knobWidth = 70;
    int knobHeight = 70;
    
    int buttonWidth = 40;
    int buttonHeight = 44;
    
    int sliderSWidth = 23;
    int sliderSHeight = 91;
    
    int buttonSWidth = 30;
    int buttonSHeight = 34;
    
    //Keyboard
    keyboardComponent.setBounds(0, getHeight()-keyboardHeight, keyboardWidth, keyboardHeight);
    
    //VCF
    cutKnob.setBounds(175, 265,knobWidth,knobHeight);
    resKnob.setBounds(175+knobWidth, 265,knobWidth,knobHeight);
    envKnob.setBounds(175+knobWidth*2, 265,knobWidth,knobHeight);
    cutLabel.setBounds(175, 245, knobWidth, 20);
    resLabel.setBounds(175+knobWidth, 245, knobWidth, 20);
    envLabel.setBounds(175+knobWidth*2, 245, knobWidth, 20);
    
    //Envelope amp
    carrierASlider.setBounds(420+sliderWidth*2, 70, sliderWidth, sliderHeight);
    carrierDSlider.setBounds(420+sliderWidth*3, 70, sliderWidth, sliderHeight);
    carrierSSlider.setBounds(420+sliderWidth*4, 70, sliderWidth, sliderHeight);
    carrierRSlider.setBounds(420+sliderWidth*5, 70, sliderWidth, sliderHeight);
    carrierALabel.setBounds(420+sliderWidth*2, 50, 20, 20);
    carrierDLabel.setBounds(420+sliderWidth*3, 50, 20, 20);
    carrierSLabel.setBounds(420+sliderWidth*4, 50, 20, 20);
    carrierRLabel.setBounds(420+sliderWidth*5, 50, 20, 20);
    
    //Envelope filter
    filterASlider.setBounds(420+sliderWidth*2, 240, sliderWidth, sliderHeight);
    filterDSlider.setBounds(420+sliderWidth*3, 240, sliderWidth, sliderHeight);
    filterSSlider.setBounds(420+sliderWidth*4, 240, sliderWidth, sliderHeight);
    filterRSlider.setBounds(420+sliderWidth*5, 240, sliderWidth, sliderHeight);
    filterALabel.setBounds(420+sliderWidth*2, 220, 20, 20);
    filterDLabel.setBounds(420+sliderWidth*3, 220, 20, 20);
    filterSLabel.setBounds(420+sliderWidth*4, 220, 20, 20);
    filterRLabel.setBounds(420+sliderWidth*5, 220, 20, 20);
    
    //Waveform buttons
    pulseButton1.setBounds(20, 84, buttonWidth, buttonHeight);
    sawButton1.setBounds(20+buttonWidth, 84, buttonWidth, buttonHeight);
    sinButton1.setBounds(20+buttonWidth*2, 84, buttonWidth, buttonHeight);
    pulseButton2.setBounds(20, 154, buttonWidth, buttonHeight);
    sawButton2.setBounds(20+buttonWidth, 154, buttonWidth, buttonHeight);
    sinButton2.setBounds(20+buttonWidth*2, 154, buttonWidth, buttonHeight);
    waveLabel.setBounds(20+buttonWidth, 44, buttonWidth, 20);
    
    //Mix slider
    mixSlider.setBounds(420, 70, sliderWidth, sliderHeight);
    mixLabel1.setBounds(420, 50, sliderWidth, 20);
    mixLabel2.setBounds(420, 70+sliderHeight, sliderWidth, 20);
    
    //pitch knobs
    octKnob1.setBounds(175, 60, knobWidth, knobWidth);
    semiKnob1.setBounds(175 + knobWidth, 60, knobWidth, knobWidth);
    fineKnob1.setBounds(175 + knobWidth*2, 60, knobWidth, knobWidth);
    octKnob2.setBounds(175, 130, knobWidth, knobWidth);
    semiKnob2.setBounds(175 + knobWidth, 130, knobWidth, knobWidth);
    fineKnob2.setBounds(175 + knobWidth*2, 130, knobWidth, knobWidth);
    octLabel.setBounds(175, 40, knobWidth, 20);
    semiLabel.setBounds(175 + knobWidth, 40, knobWidth, 20);
    fineLabel.setBounds(175 + knobWidth*2, 40, knobWidth, 20);
    
    //lfo sliders
    lfoAmpSlider.setBounds(450, 70, sliderWidth, sliderHeight);
    lfoAmpLabel.setBounds(450, 50, sliderWidth, 20);
    lfoFreqSlider.setBounds(420, 240, sliderWidth, sliderHeight);
    lfoFreqLabel.setBounds(420, 220, sliderWidth, 20);
    lfoFilterSlider.setBounds(420+sliderWidth, 240, sliderWidth, sliderHeight);
    lfoFilterLabel.setBounds(420+sliderWidth, 220, sliderWidth, 20);
    
    //delay sliders, (button and led)
    delayTimeSlider.setBounds(45, 244, sliderSWidth, sliderSHeight);
    delayFeedSlider.setBounds(68, 244, sliderSWidth, sliderSHeight);
    delayDrySlider.setBounds(91, 244, sliderSWidth, sliderSHeight);
    delayTimeLabel.setBounds(45, 224, sliderSWidth, 20);
    delayFeedLabel.setBounds(68, 224, sliderSWidth, 20);
    delayDryLabel.setBounds(91, 224, sliderSWidth+2, 20);
    /* //delay BPM sync, future feature
    delaySyncButton.setBounds(25, 294, buttonSWidth, buttonSHeight);
    delaySyncLabel.setBounds(25, 264, buttonSWidth, 20);
     */
}

void SubstractiveSynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &carrierASlider)
    {
        audioProcessor.setAttack(carrierASlider.getValue());
    }
    else if(slider == &carrierDSlider)
    {
        audioProcessor.setDecay(carrierDSlider.getValue());
    }
    else if(slider == &carrierSSlider)
    {
        audioProcessor.setSustain(carrierSSlider.getValue());
    }
    else if(slider == &carrierRSlider)
    {
        audioProcessor.setRelease(carrierRSlider.getValue());
    }
    else if(slider == &cutKnob)
    {
        audioProcessor.setCut(cutKnob.getValue());
    }
    else if(slider == &resKnob)
    {
        audioProcessor.setRes(resKnob.getValue());
    }
    else if(slider == &filterASlider)
    {
        audioProcessor.setFilterAttack(filterASlider.getValue());
    }
    else if(slider == &filterDSlider)
    {
        audioProcessor.setFilterDecay(filterDSlider.getValue());
    }
    else if(slider == &filterSSlider)
    {
        audioProcessor.setFilterSustain(filterSSlider.getValue());
    }
    else if(slider == &filterRSlider)
    {
        audioProcessor.setFilterRelease(filterRSlider.getValue());
    }
    else if(slider == &envKnob)
    {
        audioProcessor.setFilterEnv(envKnob.getValue());
    }
    else if(slider == &mixSlider)
    {
        audioProcessor.setMix(mixSlider.getValue());
    }
    else if(slider == &octKnob1)
    {
        audioProcessor.setOct(octKnob1.getValue(),0);
    }
    else if(slider == &octKnob2)
    {
        audioProcessor.setOct(octKnob2.getValue(),1);
    }
    else if(slider == &semiKnob1)
    {
        audioProcessor.setSemi(semiKnob1.getValue(),0);
    }
    else if(slider == &semiKnob2)
    {
        audioProcessor.setSemi(semiKnob2.getValue(),1);
    }
    else if(slider == &fineKnob1)
    {
        audioProcessor.setFine(fineKnob1.getValue(),0);
    }
    else if(slider == &fineKnob2)
    {
        audioProcessor.setFine(fineKnob2.getValue(),1);
    }
    else if(slider == &lfoFreqSlider)
    {
        audioProcessor.setLfoFreq(lfoFreqSlider.getValue());
    }
    else if(slider == &lfoAmpSlider)
    {
        audioProcessor.setLfoAmp(lfoAmpSlider.getValue());
    }
    else if(slider == &lfoFilterSlider)
    {
        audioProcessor.setLfoFilt(lfoFilterSlider.getValue());
    }
    else if(slider == &delayTimeSlider)
    {
        audioProcessor.setDelayTime(delayTimeSlider.getValue());
    }
    else if(slider == &delayFeedSlider)
    {
        audioProcessor.setDelayFeed(delayFeedSlider.getValue());
    }
    else if(slider == &delayDrySlider)
    {
        audioProcessor.setDelayDry(delayDrySlider.getValue());
    }
    
}

void SubstractiveSynthAudioProcessorEditor::handleNoteOn (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    //printf("\nNOTE PRESSED\n");
    //printf("Received note %d\n",midiNoteNumber);
    // frequency calculation from midi note data
    double nota = 0.0;
    nota = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    //printf("Calculated frequency %f\n",nota);
    audioProcessor.setVelocity(1.f);
    audioProcessor.addVoice(nota);
}
void SubstractiveSynthAudioProcessorEditor::handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    auto nota = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    //printf("NOTE RELEASED\n");
    audioProcessor.deactivateVoice(nota);
}

void SubstractiveSynthAudioProcessorEditor::updateToggleState (juce::Button* button, juce::String name)
{
    if (name == "Pulse1")
    {
        audioProcessor.setWaveform1(1);
        waveLed1 = 1;
        repaint();
    }
    else if (name == "Saw1")
    {
        audioProcessor.setWaveform1(2);
        waveLed1 = 2;
        repaint();
    }
    else if (name == "Sine1")
    {
        audioProcessor.setWaveform1(3);
        waveLed1 = 3;
        repaint();
    }
    else if (name == "Pulse2")
    {
        audioProcessor.setWaveform2(1);
        waveLed2 = 1;
        repaint();
    }
    else if (name == "Saw2")
    {
        audioProcessor.setWaveform2(2);
        waveLed2 = 2;
        repaint();
    }
    else if (name == "Sine2")
    {
        audioProcessor.setWaveform2(3);
        waveLed2 = 3;
        repaint();
    }
    /* //delay BPM sync, future feature
    else if (name == "Sync")
    {
        audioProcessor.switchSync();
        delaySyncLed = !delaySyncLed;
        repaint();
    }
     */
}
