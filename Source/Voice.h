//
//  Voice.h
//  fmsynth
//
//  Created by Keepin' Soul on 28/11/2021.
//

#ifndef Voice_h
#define Voice_h

#include "Envelope.h"
#include "Oscillator.h"
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
        osc1.setFrequency(f1);
        osc2.setFrequency(f2);
    };
    float getNextSample(int channel) //noexcept
    {
        float sample1 = 0.f;
        float sample2 = 0.f;
        sample1 = osc1.getNextSample(channel);
        sample2 = osc2.getNextSample(channel);
        
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

    float getNextFilterSample(float xn, int channel, float env, float lfoA){
        float lfoAmount = (1.f - lfoF.getNextSample(channel)*lfoA);
        float y = filter.processSample(xn, channel, env, lfoAmount);
        return y;
    };
    void initFilter(float cut, float res){
        filter.initFilter(cut, res);
    };
    void setWaveform1(int w)
    {
        waveform1 = w;
        osc1.setWaveform(w);
    };
    void setWaveform2(int w)
    {
        waveform2 = w;
        osc2.setWaveform(w);
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
    void setWaveformLFO(int w)
    {
        lfo.setWaveform(w);
        lfoF.setWaveform(w);
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
        osc1.setSampleRate(sr);
        osc2.setSampleRate(sr);
        env.setSampleRate(sr);
        filter.setSampleRate(sr);
        filterEnv.setSampleRate(sr);
        lfo.setSampleRate(sr);
        lfoF.setSampleRate(sr);
    };
    

private:

    Oscillator osc1;
    Oscillator osc2;
    Filter filter;
    int waveform1, waveform2;
    Envelope env;
    FilterEnvelope filterEnv;
    float mix;
    Oscillator lfo;
    float lfoAmp;
    float velocity;
    Oscillator lfoF; //lfo del filtro
    float frequency0;
};

#endif /* Voice_h */
