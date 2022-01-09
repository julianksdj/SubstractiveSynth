/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Voice.h"

//==============================================================================
/**
*/
class SubstractiveSynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SubstractiveSynthAudioProcessor();
    ~SubstractiveSynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
        
    int getVoicesSize()
    {
        return voices.size();
    };
    void addVoice(float frequency)
    {
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            if (voices.getUnchecked(voiceIndex)->getFrequency()==frequency)
            {
                voices.remove(voiceIndex);
            }
        }
        Voice *voice = new Voice();
        voice->setNoteOn(true);
        voice->setSampleRate(currentSampleRate);
        //voice->setOctave(octave);
        voice->setWaveform(waveform);
        voice->setFrequency(frequency);
        /*voice->setModFreq(modFreq);
        voice->setModAmp(modAmp);
        voice->setFrequencyFM(modFreq);*/
        voice->setAttack(attack);
        voice->setDecay(decay);
        voice->setSustain(sustain);
        voice->setRelease(release);
        voice->initFilter(cut, res);
        
        // Filter Envelope
        voice->setFilterAttack(attackF);
        voice->setFilterDecay(decayF);
        voice->setFilterSustain(sustainF);
        voice->setFilterRelease(releaseF);
        voice->initFilterEnv(cut, envAmount);
        
        voices.add(voice);
    };
    void deactivateVoice(float freq)
    {
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            if (voices[voiceIndex]->getFrequency() == freq)
            {
                voices[voiceIndex]->setNoteOn(false);
                voices[voiceIndex]->resetEnvCount();
            }
        }
    };
    void removeVoice(float freq)
    {
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            if (voices[voiceIndex]->getFrequency() == freq)
                voices.remove(voiceIndex);
        }
    };
    //provisional
    void resetVoices()
    {
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            voices[voiceIndex]->setNoteOn(false);
            voices[voiceIndex]->resetEnvCount();
        }
    };
    void setAttack(float a)
    {
        attack = a;
    };
    void setDecay(float d)
    {
        decay = d;
    };
    void setSustain(float s)
    {
        sustain = s;
    };
    void setRelease(float r)
    {
        release = r;
    };
    float getAttack()
    {
        return attack;
    };
    float getDecay()
    {
        return decay;
    };
    float getSustain()
    {
        return sustain;
    };
    float getRelease()
    {
        return release;
    };
    /*void setModFreq(float mod)
    {
        modFreq = mod;
    };
    void setModAmp(float mod)
    {
        modAmp = mod;
    };
    float getModFreq()
    {
        return modFreq;
    };
    float getModAmp()
    {
        return modAmp;
    };*/
    void setCut(float x)
    {
        cut = x;
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            voices[voiceIndex]->setCut(x);
        }
    };
    void setRes(float x)
    {
        res = x;
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            voices[voiceIndex]->setRes(x);
        }
    };
    float getCut()
    {
        return cut;
    };
    float getRes()
    {
        return res;
    };
    
    // waveforms
    void setWaveform(int x)
    {
        waveform = x;
        resetVoices();
    };
    int getWaveform()
    {
        return waveform;
    };
    
    // Filter envelope
    void setFilterAttack(float a)
    {
        attackF = a;
    };
    void setFilterDecay(float d)
    {
        decayF = d;
    };
    void setFilterSustain(float s)
    {
        sustainF = s;
    };
    void setFilterRelease(float r)
    {
        releaseF = r;
    };
    float getFilterAttack()
    {
        return attackF;
    };
    float getFilterDecay()
    {
        return decayF;
    };
    float getFilterSustain()
    {
        return sustainF;
    };
    float getFilterRelease()
    {
        return releaseF;
    };
    void setFilterEnv(float env)
    {
        envAmount = env;
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            voices[voiceIndex]->setFilterEnv(env);
        }
    };
    float getFilterEnv()
    {
        return envAmount;
    };
        
private:
    float currentSampleRate = 0.0;
    juce::OwnedArray<Voice> voices; //voices array
    
    // GUI parameters
    float attack, decay, sustain, release; //amp adsr
    float modFreq, modAmp;
    float cut, res;
    int waveform;
    float envAmount;
    float attackF, decayF, sustainF, releaseF; //filter adsr
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubstractiveSynthAudioProcessor)
};
