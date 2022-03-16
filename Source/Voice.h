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
    
    // Oscillator functions
    void setFrequency (float frequency, float * octave, float * semitone, float * fine)
    {
        float f1 = frequency * pow(2,octave[0] + semitone[0]/12.f + fine[0]/1200.f);
        float f2 = frequency * pow(2,octave[1] + semitone[1]/12.f  + fine[1]/1200.f);
        frequency0 = frequency;
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
        
        float mixx = sample1 * mix + sample2 * (1.f-mix);
        float lfoMix = mixx * (1 - lfo.getNextSample(channel)*lfoAmp);
        
        return lfoMix * velocity;
    };
    float getFrequency()
    {
        return frequency0;
    };
    
    // Envelope functions
    bool isActive ()
    {
        if (!env.getFin())
            return true;
        else
            return false;
    };
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
    float processSample(float xn, int channel, float env, float lfo){
        
        float lfoAmount = (1 - lfoF.getNextSample(channel)*lfo);
        float y = filter.processSample(xn, channel, env, lfoAmount);
        return y;
    };
    void initFilter(float cut, float res){
        filter.initFilter(cut, res);
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
        lfoF.setFrequency(f);
        
    };
    void setLfoAmp(float a)
    {
        lfoAmp = a;
    };
    void setVelocity(float vel)
    {
        velocity = vel;
    };

    // sample rate
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
        lfoF.setSampleRate(sr);
    };

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
    SineOscillator lfoF; //lfo del filtro
    float frequency0;
};

#endif /* Voice_h */
