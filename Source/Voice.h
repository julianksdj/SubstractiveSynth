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
        if (env.getCarrAmp()<=0.0000001)
            return false;
        else
            return true;
    };
    
    // Oscillator functions
    void setFrequency (float f)
    {
        oscSquare.setFrequency(f);
        oscSine.setFrequency(f);
        oscSaw.setFrequency(f);
    };
    /*void setFrequencyFM (float f)
    {
        osc.setFrequencyFM(f);
    };*/
    float getNextSample(int channel) //noexcept
    {
        if (waveform == 0)
            return oscSquare.getNextSample(channel);
        else if (waveform == 1)
            return oscSine.getNextSample(channel);
        else if (waveform == 2)
            return oscSaw.getNextSample(channel);
        return 0.0;
    };
    float getFrequency()
    {
        if (waveform == 0)
            return oscSquare.getFrequency();
        else if (waveform == 1)
            return oscSine.getFrequency();
        else if (waveform == 2)
            return oscSaw.getFrequency();
        return 0.0;
            
    };
    void setCarrFreq(float m)
    {
        oscSquare.setCarrFreq(m);
        oscSine.setCarrFreq(m);
        oscSaw.setCarrFreq(m);
            
    };
    /*void setModFreq(float m)
    {
        osc.setModFreq(m);
    };
    void setModAmp(float m)
    {
        osc.setModAmp(m);
    };*/

    // Envelope functions
    float getEnvelope()
    {
        return env.getEnvelope();
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
    void setSampleRate(int sr)
    {
        env.setSampleRate(sr);
    };
    void setCarrAmp(float a)
    {
        env.setCarrAmp(a);
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
        oscSquare.setSampleRate(sr);
        oscSine.setSampleRate(sr);
        oscSaw.setSampleRate(sr);
        env.setSampleRate(sr);
        filter.setSampleRate(sr);
        filterEnv.setSampleRate(sr);
    };
    void setWaveform(int w)
    {
        waveform = w;
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
    float getFilterEnvelope()
    {
        return filterEnv.getEnvelope();
    };
    void setFilterEnv(float env){
        filterEnv.setFilterEnv(env);
    };
    void initFilterEnv(float cutoff, float env){
        filterEnv.initFilterEnv(cutoff,env);
    };
    
private:
    Envelope env;
    SineOscillator oscSine;
    SquareOscillator oscSquare;
    SawOscillator oscSaw;
    Filter filter;
    int waveform;
    FilterEnvelope filterEnv;
    
};

#endif /* Voice_h */
