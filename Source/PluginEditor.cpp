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
    setSize (315, 300);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::purple);
    
    //keyboard
    addAndMakeVisible (keyboardComponent);
    keyboardState.addListener (this);
    
    //attack slider
    carrierASlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierASlider);
    // min  attack time must be 0.0001 (100us) to avoid attackSamples from being 0
    // so the division Aa = 0.125 / attackSamples is never inf
    carrierASlider.setRange (0.0001, 10.0);
    carrierASlider.addListener (this);
    carrierASlider.setValue(audioProcessor.getAttack());
    addAndMakeVisible (carrierALabel);
    carrierALabel.setText ("A", juce::dontSendNotification);

    //decay slider
    carrierDSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierDSlider);
    carrierDSlider.setRange (0.0001, 10.0);
    carrierDSlider.addListener (this);
    carrierDSlider.setValue(audioProcessor.getDecay());
    addAndMakeVisible (carrierDLabel);
    carrierDLabel.setText ("D", juce::dontSendNotification);
    
    //sustain slider
    carrierSSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierSSlider);
    carrierSSlider.setRange (0.0, 0.125);
    carrierSSlider.addListener (this);
    carrierSSlider.setValue(audioProcessor.getSustain());
    addAndMakeVisible (carrierSLabel);
    carrierSLabel.setText ("S", juce::dontSendNotification);
    
    //release slider
    carrierRSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (carrierRSlider);
    carrierRSlider.setRange (0.0001, 10.0);
    carrierRSlider.addListener (this);
    carrierASlider.setValue(audioProcessor.getRelease());
    addAndMakeVisible (carrierRLabel);
    carrierRLabel.setText ("R", juce::dontSendNotification);
    
    //cutoff knob
    addAndMakeVisible (cutKnob);
    cutKnob.setRange (20.0, 20000.0);
    cutKnob.addListener (this);
    cutKnob.setValue(audioProcessor.getCut());
    cutKnob.setSkewFactorFromMidPoint(1000);
    cutLabel.setText ("Cut", juce::dontSendNotification);
    cutLabel.setJustificationType(juce::Justification::centred);
    cutLabel.setColour(juce::Label::textColourId, juce::Colours::purple);
    addAndMakeVisible (cutLabel);
    cutKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    cutKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 15);
    
    //resonance knob
    addAndMakeVisible (resKnob);
    resKnob.setRange (0.5, 10.0);
    resKnob.addListener (this);
    resKnob.setValue(audioProcessor.getRes());
    resLabel.setText ("Res", juce::dontSendNotification);
    resLabel.setJustificationType(juce::Justification::centred);
    resLabel.setColour(juce::Label::textColourId, juce::Colours::purple);
    addAndMakeVisible (resLabel);
    resKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    resKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 15);
    
    //combobox waveform
    waveLabel.setText("Waveform", juce::dontSendNotification);
    waveLabel.setColour(juce::Label::textColourId, juce::Colours::purple);
    addAndMakeVisible (waveLabel);
    addAndMakeVisible (waveMenu);
    waveMenu.addItem ("Square",  1);
    waveMenu.addItem ("Sine",   2);
    waveMenu.addItem ("Saw", 3);
    waveMenu.onChange = [this] { waveMenuChanged(); };
    waveMenu.setSelectedId(audioProcessor.getWaveform()+1);
    
    //env knob
    addAndMakeVisible (envKnob);
    envKnob.setRange (0.0, 1.0);
    envKnob.addListener (this);
    envKnob.setValue(audioProcessor.getFilterEnv());
    envLabel.setText ("Env", juce::dontSendNotification);
    envLabel.setJustificationType(juce::Justification::centred);
    envLabel.setColour(juce::Label::textColourId, juce::Colours::purple);
    addAndMakeVisible (envLabel);
    envKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    envKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 15);
    
    //filter attack slider
    filterASlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (filterASlider);
    // min  attack time must be 0.0001 (100us) to avoid attackSamples from being 0
    // so the division Aa = 0.125 / attackSamples is never inf
    filterASlider.setRange (0.0001, 10.0);
    filterASlider.addListener (this);
    filterASlider.setValue(audioProcessor.getAttack());
    addAndMakeVisible (filterALabel);
    filterALabel.setText ("A", juce::dontSendNotification);

    //filter decay slider
    filterDSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (filterDSlider);
    filterDSlider.setRange (0.0001, 5.0);
    filterDSlider.addListener (this);
    filterDSlider.setValue(audioProcessor.getDecay());
    addAndMakeVisible (filterDLabel);
    filterDLabel.setText ("D", juce::dontSendNotification);
    
    //filter sustain slider
    filterSSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (filterSSlider);
    filterSSlider.setRange (0.0, 1.0);
    filterSSlider.setValue(20000.0);
    filterSSlider.addListener (this);
    filterSSlider.setValue(audioProcessor.getSustain());
    addAndMakeVisible (filterSLabel);
    filterSLabel.setText ("S", juce::dontSendNotification);
    
    //filter release slider
    filterRSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    addAndMakeVisible (filterRSlider);
    filterRSlider.setRange (0.0001, 5.0);
    filterRSlider.addListener (this);
    carrierASlider.setValue(audioProcessor.getRelease());
    addAndMakeVisible (filterRLabel);
    filterRLabel.setText ("R", juce::dontSendNotification);
}

SubstractiveSynthAudioProcessorEditor::~SubstractiveSynthAudioProcessorEditor()
{
}

//==============================================================================
void SubstractiveSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour (178, 176, 255));
    g.setColour(juce::Colours::purple);
    g.setFont (15.0f);
}

void SubstractiveSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    keyboardComponent.setBounds(0, 240, getWidth(), 60);
    
    waveMenu.setBounds(10, 60, 80, 20);
    waveLabel.setBounds(10, 40, 100, 20);
    
    carrierASlider.setBounds(10, 140, 20, 80);
    carrierDSlider.setBounds(30, 140, 20, 80);
    carrierSSlider.setBounds(50, 140, 20, 80);
    carrierRSlider.setBounds(70, 140, 20, 80);
    carrierALabel.setBounds(10, 215, 20, 20);
    carrierDLabel.setBounds(30, 215, 20, 20);
    carrierSLabel.setBounds(50, 215, 20, 20);
    carrierRLabel.setBounds(70, 215, 20, 20);
    
    cutKnob.setBounds(120, 50, 60, 70);
    resKnob.setBounds(180, 50, 60, 70);
    cutLabel.setBounds(120, 35, 50, 20);
    resLabel.setBounds(180, 35, 50, 20);
    
    envKnob.setBounds(240, 50, 60, 70);
    envLabel.setBounds(240, 35, 50, 20);
    
    filterASlider.setBounds(170, 140, 20, 80);
    filterDSlider.setBounds(190, 140, 20, 80);
    filterSSlider.setBounds(210, 140, 20, 80);
    filterRSlider.setBounds(230, 140, 20, 80);
    filterALabel.setBounds(170, 215, 20, 20);
    filterDLabel.setBounds(190, 215, 20, 20);
    filterSLabel.setBounds(210, 215, 20, 20);
    filterRLabel.setBounds(230, 215, 20, 20);
    
}

void SubstractiveSynthAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if(slider == &carrierASlider)
    {
        audioProcessor.setAttack(carrierASlider.getValue());
    }
    if(slider == &carrierDSlider)
    {
        audioProcessor.setDecay(carrierDSlider.getValue());
    }
    if(slider == &carrierSSlider)
    {
        audioProcessor.setSustain(carrierSSlider.getValue());
    }
    if(slider == &carrierRSlider)
    {
        audioProcessor.setRelease(carrierRSlider.getValue());
    }
    if(slider == &cutKnob)
    {
        audioProcessor.setCut(cutKnob.getValue());
    }
    if(slider == &resKnob)
    {
        audioProcessor.setRes(resKnob.getValue());
    }
    if(slider == &filterASlider)
    {
        audioProcessor.setFilterAttack(filterASlider.getValue());
    }
    if(slider == &filterDSlider)
    {
        audioProcessor.setFilterDecay(filterDSlider.getValue());
    }
    if(slider == &filterSSlider)
    {
        audioProcessor.setFilterSustain(filterSSlider.getValue());
    }
    if(slider == &filterRSlider)
    {
        audioProcessor.setFilterRelease(filterRSlider.getValue());
    }
    if(slider == &envKnob)
    {
        audioProcessor.setFilterEnv(envKnob.getValue());
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
    audioProcessor.addVoice(nota);
}
void SubstractiveSynthAudioProcessorEditor::handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    auto nota = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    //printf("NOTE RELEASED\n");
    audioProcessor.deactivateVoice(nota);
}

void SubstractiveSynthAudioProcessorEditor::waveMenuChanged()
{
    switch (waveMenu.getSelectedId())
    {
        case 1:
            audioProcessor.setWaveform(0);
            break;
        case 2:
            audioProcessor.setWaveform(1);
            break;
        case 3:
            audioProcessor.setWaveform(2);
            break;
        default: break;
    }
}
