/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#define NUM_VOICES 16

#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "Delay.h"

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
        
    //Voices
    int getVoicesSize()
    {
        //return voices.size();
        return voicesSize;
    };
    void addVoice(float frequency)
    {

        Voice voice;

        voice.setNoteOn(true);
        voice.setSampleRate(currentSampleRate);
        voice.setWaveform1(waveform1);
        voice.setWaveform2(waveform2);
        voice.setFrequency(frequency, octave, semitone, fine);
        
        // Amp Envelope
        voice.setAttack(attack);
        voice.setDecay(decay);
        voice.setSustain(sustain);
        voice.setRelease(release);
        
        voice.initFilter(cut, res);
        
        // Filter Envelope
        voice.setFilterAttack(attackF);
        voice.setFilterDecay(decayF);
        voice.setFilterSustain(sustainF);
        voice.setFilterRelease(releaseF);
        voice.initFilterEnv(cut, filterEnvAmount);
        
        //Mix
        voice.setMix(mix);
        
        //LFO
        voice.setFrequencyLFO(lfoFreq);
        voice.setLfoAmp(lfoAmp);
        voice.setWaveformLFO(3);
        
        //velocity
        voice.setVelocity(velocity);
        
        
        if (writeVoicePos >= voicesSize)
        {
            writeVoicePos = 0;
            voices[writeVoicePos] = voice;
        }
        else
        {
            voices[writeVoicePos] = voice;
            writeVoicePos++;
        }
        
        

        
    };
    void deactivateVoice(float freq) //release note
    {
        for (auto voiceIndex = 0; voiceIndex < voicesSize; ++voiceIndex)
        {
            if (voices[voiceIndex].getFrequency() == freq)
            {
                voices[voiceIndex].setNoteOn(false);
                voices[voiceIndex].resetEnvCount();
            }
        }
        writeVoicePos--;
        if(writeVoicePos<0)
            writeVoicePos=0;
    };
    void resetVoices()
    {
        for (auto voiceIndex = 0; voiceIndex < voicesSize; ++voiceIndex)
        {
            voices[voiceIndex].setNoteOn(false);
            voices[voiceIndex].resetEnvCount();
            //writeVoicePos = 0;
        }
    };
    
    //Envelope
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
    
    // Filter
    void setCut(float x)
    {
        cut = x;
        for (auto voiceIndex = 0; voiceIndex < voicesSize; ++voiceIndex)
        {
            voices[voiceIndex].setCut(x);
        }
    };
    void setRes(float x)
    {
        res = x;
        for (auto voiceIndex = 0; voiceIndex < voicesSize; ++voiceIndex)
        {
            voices[voiceIndex].setRes(x);
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
        filterEnvAmount = env;
        for (auto voiceIndex = 0; voiceIndex < voicesSize; ++voiceIndex)
        {
            voices[voiceIndex].setFilterEnv(env);
        }
    };
    float getFilterEnv()
    {
        return filterEnvAmount;
    };
    
    // waveforms
    void setWaveform1(int x)
    {
        waveform1 = x;
        resetVoices();
    };
    void setWaveform2(int x)
    {
        waveform2 = x;
        resetVoices();
    };
    int getWaveform1()
    {
        return waveform1;
    };
    int getWaveform2()
    {
        return waveform2;
    };
    
    //Mix
    void setMix(float m)
    {
        mix = m;
        for (auto voiceIndex = 0; voiceIndex < voicesSize; ++voiceIndex)
        {
            voices[voiceIndex].setMix(m);
        }
    };
    float getMix()
    {
        return mix;
    };
    
    //Oscillators
    void setOct(float o, int osc)
    {
        octave[osc] = o;
    };
    float getOct(int osc)
    {
        return octave[osc];
    };
    void setSemi(float s, int osc)
    {
        semitone[osc] = s;
    };
    float getSemi(int osc)
    {
        return semitone[osc];
    };
    void setFine(float f, int osc)
    {
        fine[osc] = f;
    };
    float getFine(int osc)
    {
        return fine[osc];
    };
    void processOsc(float *channelData, int channel)
    {
        for (auto sample = 0; sample < numSamples; ++sample)
        {
            float sumOsc = 0.0;
            // sum of voices
            for (auto voiceIndex = 0; voiceIndex < voicesSize; ++voiceIndex)
            {
                if(voices[voiceIndex].isActive())
                {
                    auto ampEnv = voices[voiceIndex].getEnvelope(channel);
                    auto filterEnv = voices[voiceIndex].getFilterEnvelope(channel);
                    auto currentSample = voices[voiceIndex].getNextSample(channel)*ampEnv*0.125f;
                    currentSample = voices[voiceIndex].getNextFilterSample(currentSample, channel, filterEnv, lfoFilt);;
                    sumOsc += currentSample;
                }
            }
            channelData[sample] += sumOsc;
        }
    };
    
    //LFO
    void setLfoFreq(float f)
    {
        lfoFreq = f;
    };
    float getLfoFreq()
    {
        return lfoFreq;
    };
    void setLfoAmp(float a)
    {
        lfoAmp = a;
        for (auto voiceIndex = 0; voiceIndex < voicesSize; ++voiceIndex)
        {
            voices[voiceIndex].setLfoAmp(a);
        }
    };
    void setLfoFilt(float f)
    {
        lfoFilt = f;
    };
    float getLfoAmp()
    {
        return lfoAmp;
    };
    
    //Velocity
    void setVelocity(float vel)
    {
        velocity = vel;
    };
    
    //Delay
    float getDelayTime(){
        return delay.getDelayTime();
    };
    float getDelayFeed(){
        return delay.getDelayFeed();
    };
    float getDelayDry(){
        return delay.getDelayDry();
    };
    void setDelayTime(float dt){
        delay.setDelayTime(dt);
    };
    void setDelayFeed(float fb){
        delay.setDelayFeed(fb);
    };
    void setDelayDry(float dw){
        delay.setDelayDry(dw);
    };

        
private:
    float currentSampleRate = 0.0;
    Voice voices[NUM_VOICES];
    int voicesSize;
    int writeVoicePos;
    
    // GUI parameters
    float attack, decay, sustain, release; //amp adsr
    float modFreq, modAmp;
    float cut, res;
    int waveform1;
    int waveform2;
    float envAmount;
    float filterEnvAmount;
    float attackF, decayF, sustainF, releaseF; //filter adsr
    float mix;
    float octave[2], semitone[2], fine[2];
    float lfoFreq, lfoAmp, lfoFilt;
    float velocity;
    Delay delay;
    int numSamples;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubstractiveSynthAudioProcessor)
};
