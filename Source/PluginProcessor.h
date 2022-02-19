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
        voice->setWaveform1(waveform1);
        voice->setWaveform2(waveform2);
        float freq2a = frequency * pow(2,octave[0] + semitone[0]/12.f + fine[0]/1200.f);
        float freq2b = frequency * pow(2,octave[1] + semitone[1]/12.f  + fine[1]/1200.f);
        voice->setFrequency(freq2a, freq2b);
        
        // Amp Envelope
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
        
        //Mix
        voice->setMix(mix);
        
        //LFO
        voice->setFrequencyLFO(lfoFreq);
        voice->setLfoAmp(lfoAmp);
        
        //velocity
        voice->setVelocity(velocity);
        
        voices.add(voice);
    };
    void deactivateVoice(float freq)
    {
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            float freq2 = freq * pow(2,octave[0] + semitone[0]/12.f + fine[0]/1200.f);
            if (voices[voiceIndex]->getFrequency() == freq2)
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
    void setMix(float m)
    {
        mix = m;
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            voices[voiceIndex]->setMix(m);
        }
    };
    float getMix()
    {
        return mix;
    };
    void setOct(float o, int osc)
    {
        octave[osc] = o;
        resetVoices();
    };
    float getOct(int osc)
    {
        return octave[osc];
    };
    void setSemi(float s, int osc)
    {
        semitone[osc] = s;
        resetVoices();
    };
    float getSemi(int osc)
    {
        return semitone[osc];
    };
    void setFine(float f, int osc)
    {
        fine[osc] = f;
        resetVoices();
    };
    float getFine(int osc)
    {
        return fine[osc];
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
        for (auto voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex)
        {
            voices[voiceIndex]->setLfoAmp(a);
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
    void setVelocity(float vel)
    {
        velocity = vel;
    };

        
private:
    float currentSampleRate = 0.0;
    juce::OwnedArray<Voice> voices; //voices array
    
    // GUI parameters
    float attack, decay, sustain, release; //amp adsr
    float modFreq, modAmp;
    float cut, res;
    int waveform1;
    int waveform2;
    float envAmount;
    float attackF, decayF, sustainF, releaseF; //filter adsr
    float mix;
    float octave[2], semitone[2], fine[2];
    float lfoFreq, lfoAmp, lfoFilt;
    float velocity;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SubstractiveSynthAudioProcessor)
};
