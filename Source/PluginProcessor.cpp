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
    // Initializations
    
    currentSampleRate = sampleRate;
    
    attack = 0.001f;
    decay = 0.0001f;
    sustain = 1.f;
    release = 0.09f;
    
    attackF = 0.001f;
    decayF = 0.0001f;
    sustainF = 1.f;
    releaseF = 0.09f;
    envAmount = 0.f;
    filterEnvAmount = 0.f;
    
    waveform1 = 1;
    waveform2 = 1;
    
    mix = 0.5f;
    
    octave[0] = 0;
    octave[1] = 0;
    semitone[0] = 0;
    semitone[1] = 0;
    fine[0] = 0;
    fine[1] = 0;
    
    lfoFreq = 1.f;
    lfoAmp = 0.f;
    lfoFiltAmp = 0.f;
    
    //delay
    delay.initDelay(currentSampleRate, 300.f, 20.f, 0.f);
    
    voicesSize = NUM_VOICES;
    writeVoicePos = 0;
    
    cut = 20000.f;
    res = 0.1f;
    
    for (int i=0; i<voicesSize; i++)
    {
        voices[i].setSampleRate(currentSampleRate);
        voices[i].setWaveform1(waveform1);
        voices[i].setWaveform2(waveform2);
        voices[i].setFrequency(440.f, octave, semitone, fine);
        
        // Amp Envelope
        voices[i].setAttack(attack);
        voices[i].setDecay(decay);
        voices[i].setSustain(sustain);
        voices[i].setRelease(release);
        
        //filter
        voices[i].initFilter(cut, res);
        
        // Filter Envelope
        voices[i].setFilterAttack(attackF);
        voices[i].setFilterDecay(decayF);
        voices[i].setFilterSustain(sustainF);
        voices[i].setFilterRelease(releaseF);
        voices[i].initFilterEnv(cut, 0.f);
        
        //Mix
        //voices[i].setMix(mix);
        
        //LFO
        //voices[i].setFrequencyLFO(lfoFreq);
        //voices[i].setLfoAmp(lfoAmp);
        //voices[i].setWaveformLFO(3);
        
        //velocity
        voices[i].setVelocity(0.f);
        voices[i].setNoteOn(false);
    }
    
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
    numSamples = buffer.getNumSamples();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);
    
    // MIDI HANDLING -----------------------------------------------------------------------------------------
    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage currentMessage;
    int samplePos;
    
    while (it.getNextEvent(currentMessage, samplePos))
    {
        if (currentMessage.isNoteOn()) // note pressed
        {
            velocity = currentMessage.getVelocity() / 127.f;
            float nota = currentMessage.getNoteNumber();
            float notaHz = juce::MidiMessage::getMidiNoteInHertz(currentMessage.getNoteNumber());
            addVoice(juce::MidiMessage::getMidiNoteInHertz(currentMessage.getNoteNumber()));
        }
        else if (currentMessage.isNoteOff()) // note released
        {
            deactivateVoice(juce::MidiMessage::getMidiNoteInHertz(currentMessage.getNoteNumber()));
        }
    }
    
    // AUDIO PROCESSING --------------------------------------------------------------------------------------
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
    //int channel =0;
        auto* channelData = buffer.getWritePointer(channel);
        //oscillators + amplitude envelope + filter
        processOsc(channelData, channel);
        //delay
        delay.processDelay(channelData, channel, numSamples);
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
