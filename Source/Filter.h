//
//  Filter.h
//  substractiveSynth
//
//  Created by Keepin' Soul on 16/12/2021.
//

#ifndef Filter_h
#define Filter_h
//#include "FilterEnvelope.h"

class Filter{

public:
    Filter(){
        z1L = 0.0;
        z2L = 0.0;
        z1R = 0.0;
        z1L = 0.0;
    };
    void setCut(float x){
        cut = x;
        this->updateFilter();
    };
    void setRes(float x){
        res = x;
        this->updateFilter();
    };
    void updateFilter(){
        // Resonator filter formulas from Will Pirkle's book (chapter 6.3.1)
        frec = juce::MathConstants<float>::twoPi * cut / currentSampleRate;
        bw = cut / res;
        b2L = exp(- juce::MathConstants<float>::twoPi * bw / currentSampleRate);
        b2R = b2L;
        b1L = (-4 * b2L / (1+b2L)) * cos(frec);
        b1R = b1L;
        a0L = (1-b2L) * sqrt(1 - b1L * b1L / (4 * b2L));
        a0R = a0L;
    };
    float getRes(){
        return res;
    };
    float getCut(){
        return cut;
    };
    //void processFilter(float *channelData, float channel, float numSamples, float lfoAmount){
    //    for (int i=0; i<numSamples; i++)
    //    {
    //        auto env = filterEnv.getEnvelope(channel);
    //        cut = env * lfoAmount;
    //        updateFilter();
    //        if (channel == 0)
    //        {
    //            // READ: Delay samples y(n-1), y(n-2)
    //            float yn_2 = z2L;
    //            float yn_1 = z1L;
    //            // Difference Equation
    //            float yn = a0L * channelData[i] - b1L * yn_1 - b2L * yn_2;
    //            // WRITE: Delay with current y(n)
    //            z2L = z1L;
    //            z1L = yn;
    //            channelData[i] = yn;
    //        }
    //        else //channel == 1
    //        {
    //            float yn_2 = z2R;
    //            float yn_1 = z1R;
    //            float yn = a0R * channelData[i] - b1R * yn_1 - b2R * yn_2;
    //            z2R = z1R;
    //            z1R = yn;
    //            channelData[i] = yn;
    //        }
    //    }
    //};
    float processSample(float xn, int channel, float env, float lfoAmount){
        cut = env * lfoAmount;
        updateFilter();
        if (channel == 0)
        {
            // READ: Delay samples y(n-1), y(n-2)
            float yn_2 = z2L;
            float yn_1 = z1L;
            // Difference Equation
            float yn = a0L * xn - b1L * yn_1 - b2L * yn_2;
            // WRITE: Delay with current y(n)
            z2L = z1L;
            z1L = yn;
            return yn;
        }
        else //channel == 1
        {
            float yn_2 = z2R;
            float yn_1 = z1R;
            float yn = a0R * xn - b1R * yn_1 - b2R * yn_2;
            z2R = z1R;
            z1R = yn;
            return yn;
        }
    };
        
    void initFilter(float c, float r){
        setCut(c);
        setRes(r);
    };
    
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
        //filterEnv.setSampleRate(sr);
    };
    
    //void setAttack(float a)
    //{
    //    filterEnv.setAttack(a);
    //};
    //void setDecay(float d)
    //{
    //    filterEnv.setDecay(d);
    //};
    //void setSustain(float s)
    //{
    //    filterEnv.setSustain(s);
    //};
    //void setRelease(float r)
    //{
    //    filterEnv.setRelease(r);
    //};
    //void setNoteOn(bool n)
    //{
    //    filterEnv.setNoteOn(n);
    //};
    //void resetEnvCount()
    //{
    //    filterEnv.resetEnvCount();
    //};
    //float getEnvelope(int channel)
    //{
    //    return filterEnv.getEnvelope(channel);
    //};
    //void setFilterEnv(float env){
    //    filterEnv.setFilterEnv(env);
    //};
    //void initFilterEnv(float cutoff, float env){
    //    filterEnv.initFilterEnv(cutoff,env);
    //};
private:
    float b2L, b2R, b1L, b1R, a0L, a0R; //filter coefficients
    float z1L, z1R, z2L, z2R; //delays
    float cut, res; //cutoff frequency, resonance (Q)
    float frec; //normalized cutoff frequency
    float bw; //bandwith
    float currentSampleRate;
    //float envAmount;
    //FilterEnvelope filterEnv;
};

#endif /* Filter_h */
