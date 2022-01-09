/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SubstractiveSynthAudioProcessor::SubstractiveSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SubstractiveSynthAudioProcessor::~SubstractiveSynthAudioProcessor()
{
}

//==============================================================================
const juce::String SubstractiveSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SubstractiveSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SubstractiveSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SubstractiveSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SubstractiveSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SubstractiveSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SubstractiveSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SubstractiveSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SubstractiveSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void SubstractiveSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SubstractiveSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    currentSampleRate = sampleRate;
    
    attack = 0.0001;
    decay = 0.0001;
    sustain = 0.125;
    release = 0.0001;
    
    setCut(20000.0);
    setRes(0.5);
    
    attackF = 0.0001;
    decayF = 0.0001;
    sustainF = 20000.0;
    releaseF = 0.0001;
    envAmount = 0.0;
    
    waveform = 1;
}

void SubstractiveSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SubstractiveSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SubstractiveSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // MIDI HANDLING -----------------------------------------------------------------------------------------
    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage currentMessage;
    int samplePos;
    
    while (it.getNextEvent(currentMessage, samplePos))
    {
        if (currentMessage.isNoteOn())
        {
            //printf("\nNOTE PRESSED\n");
            //printf("Received note %d\n",currentMessage.getNoteNumber());
            addVoice(juce::MidiMessage::getMidiNoteInHertz(currentMessage.getNoteNumber()));
        }
        else if (currentMessage.isNoteOff())
        {
            //printf("NOTE RELEASED\n");
            deactivateVoice(juce::MidiMessage::getMidiNoteInHertz(currentMessage.getNoteNumber()));
        }
    }
    // -------------------------------------------------------------------------------------------------------
    
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float sumOsc = 0.0;
            // sum of voices
            for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
            {
                auto* voice = voices.getUnchecked(voiceIndex);
                auto ampEnv = voice->getEnvelope();
                auto filtEnv = voice->getFilterEnvelope();
                if(voice->isActive())
                {
                    auto currentSample = voice->getNextSample(channel)*ampEnv;
                    float filteredSample = voice->processSample(currentSample, channel, filtEnv);// filter
                    sumOsc += filteredSample;
                    //sumOsc += currentSample;
                }
                else
                    voices.remove(voiceIndex);
            }
            channelData[sample] += sumOsc;
        }
    }
}

//==============================================================================
bool SubstractiveSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SubstractiveSynthAudioProcessor::createEditor()
{
    return new SubstractiveSynthAudioProcessorEditor (*this);
}

//==============================================================================
void SubstractiveSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SubstractiveSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SubstractiveSynthAudioProcessor();
}
