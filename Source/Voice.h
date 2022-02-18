//
//  Voice.h
//  fmsynth
//
//  Created by Keepin' Soul on 28/11/2021.
//

#ifndef Voice_h
#define Voice_h

#include "Envelope.h"
#include "SineOscillator.h"
#include "SquareOscillator.h"
#include "SawOscillator.h"
#include "Filter.h"
#include "FilterEnvelope.h"

class Voice
{

public:
    
    // voice features functions
    bool isActive ()
    {
        if (!env.getFin())
            return true;
        else
            return false;
    };
    
    // Oscillator functions
    void setFrequency (float f1, float f2)
    {
        oscSquare1.setFrequency(f1);
        oscSine1.setFrequency(f1);
        oscSaw1.setFrequency(f1);
        oscSquare2.setFrequency(f2);
        oscSine2.setFrequency(f2);
        oscSaw2.setFrequency(f2);
    };

    float getNextSample(int channel) //noexcept
    {
        float sample1 = 0.f;
        float sample2 = 0.f;
        if (waveform1 == 1)
            sample1 = oscSquare1.getNextSample(channel);
        else if (waveform1 == 2)
            sample1 = oscSaw1.getNextSample(channel);
        else if (waveform1 == 3)
            sample1 =oscSine1.getNextSample(channel);
        
        if (waveform2 == 1)
            sample2 = oscSquare2.getNextSample(channel);
        else if (waveform2 == 2)
            sample2 = oscSaw2.getNextSample(channel);
        else if (waveform2 == 3)
            sample2 = oscSine2.getNextSample(channel);
        
        //return sample1 * mix + sample2 * (1.f-mix); //mix of both oscillators
        float mixx = sample1 * mix + sample2 * (1.f-mix);
        //float lfoMix = mixx * lfo.getNextSample(channel);
        float lfoMix = mixx * (1 - lfo.getNextSample(channel)*lfoAmp);
        
        return lfoMix * velocity;
    };
    float getFrequency()
    {
        if (waveform1 == 1)
            return oscSquare1.getFrequency();
        else if (waveform1 == 2)
            return oscSaw1.getFrequency();
        else if (waveform1 == 3)
            return oscSine1.getFrequency();
        /*else if (waveform2 == 1)
            return oscSquare2.getFrequency();
        else if (waveform2 == 2)
            return oscSaw2.getFrequency();
        else if (waveform2 == 3)
            return oscSine2.getFrequency();*/
        return 0.0;
            
    };
    void setCarrFreq(float m)
    {
        oscSquare1.setCarrFreq(m);
        oscSine1.setCarrFreq(m);
        oscSaw1.setCarrFreq(m);
        oscSquare2.setCarrFreq(m);
        oscSine2.setCarrFreq(m);
        oscSaw2.setCarrFreq(m);
            
    };

    // Envelope functions
    float getEnvelope(int channel)
    {
        return env.getEnvelope(channel);
    };
    void setAttack(float a)
    {
        env.setAttack(a);
    };
    void setDecay(float d)
    {
        env.setDecay(d);
    };
    void setSustain(float s)
    {
        env.setSustain(s);
    };
    void setRelease(float r)
    {
        env.setRelease(r);
    };
    
//    void setCarrAmp(float a, int channel)
//    {
//        env.setCarrAmp(a, channel);
//    };
    void setNoteOn(bool n)
    {
        env.setNoteOn(n);
        filterEnv.setNoteOn(n);
    };
    void resetEnvCount()
    {
        env.resetEnvCount();
        filterEnv.resetEnvCount();
    };
    
    // Filter functions
    void setCut(float x){
        filter.setCut(x);
        //filterEnv.setCut(x);
    };
    void setRes(float x){
        filter.setRes(x);
    };
    void updateFilter(){
        filter.updateFilter();
    };
    float getRes(){
        return filter.getRes();
    };
    float getCut(){
        return filter.getCut();
    };
    float processSample(float xn, int channel, float env){
        return filter.processSample(xn, channel, env);
    };
    void initFilter(float cut, float res){
        filter.initFilter(cut, res);

    };
    void setSampleRate(float sr)
    {
        oscSquare1.setSampleRate(sr);
        oscSine1.setSampleRate(sr);
        oscSaw1.setSampleRate(sr);
        oscSquare2.setSampleRate(sr);
        oscSine2.setSampleRate(sr);
        oscSaw2.setSampleRate(sr);
        env.setSampleRate(sr);
        filter.setSampleRate(sr);
        filterEnv.setSampleRate(sr);
        lfo.setSampleRate(sr);
    };
    void setWaveform1(int w)
    {
        waveform1 = w;
    };
    void setWaveform2(int w)
    {
        waveform2 = w;
    };
    
    // Filter Envelope
    void setFilterAttack(float a)
    {
        filterEnv.setAttack(a);
    };
    void setFilterDecay(float d)
    {
        filterEnv.setDecay(d);
    };
    void setFilterSustain(float s)
    {
        filterEnv.setSustain(s);
    };
    void setFilterRelease(float r)
    {
        filterEnv.setRelease(r);
    };
    float getFilterEnvelope(int channel)
    {
        return filterEnv.getEnvelope(channel);
    };
    void setFilterEnv(float env){
        filterEnv.setFilterEnv(env);
    };
    void initFilterEnv(float cutoff, float env){
        filterEnv.initFilterEnv(cutoff,env);
    };
    void setMix(float m)
    {
        mix = m;
    };
    
    //LFO
    void setFrequencyLFO(float f)
    {
        lfo.setFrequency(f);
    };
    void setLfoAmp(float a)
    {
        lfoAmp = a;
    };
    
    void setVelocity(float vel)
    {
        velocity = vel;
    }
    
private:
    SineOscillator oscSine1;
    SquareOscillator oscSquare1;
    SawOscillator oscSaw1;
    SineOscillator oscSine2;
    SquareOscillator oscSquare2;
    SawOscillator oscSaw2;
    Filter filter;
    int waveform1, waveform2;
    Envelope env;
    FilterEnvelope filterEnv;
    float mix;
    SineOscillator lfo;
    float lfoAmp;
    float velocity;
};

#endif /* Voice_h */
