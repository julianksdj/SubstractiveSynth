//
//  Filter.h
//  substractiveSynth
//
//  Created by Keepin' Soul on 16/12/2021.
//

#ifndef Filter_h
#define Filter_h

class Filter{

public:
    //==============================================================================
    //Setters
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
        // Resonator filter formulas from the book (chapter 6.3.1)
        frec = juce::MathConstants<float>::twoPi * cut / currentSampleRate;
        bw = cut / res;
        b2L = exp(- juce::MathConstants<float>::twoPi * bw / currentSampleRate);
        b2R = b2L;
        b1L = (-4 * b2L / (1+b2L)) * cos(frec);
        b1R = b1L;
        a0L = (1-b2L) * sqrt(1 - b1L * b1L / (4 * b2L));
        a0R = a0L;
    };
    
    //getters (for gui)
    float getRes(){
        return res;
    };
    float getCut(){
        return cut;
    };
    
    float processSample(float xn, int channel, float env){
        //cut = env;
        cut = env;
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
            // READ: Delay samples y(n-1), y(n-2)
            float yn_2 = z2R;
            float yn_1 = z1R;
            // Difference Equation
            float yn = a0R * xn - b1R * yn_1 - b2R * yn_2;
            // WRITE: Delay with current y(n)
            z2R = z1R;
            z1R = yn;
            return yn;
        }
    };
    void initFilter(float c, float r){
        setCut(c);
        setRes(r);
        //z1L = 0.0;
        //z2L = 0.0;
        //z1R = 0.0;
        //z1L = 0.0;
    };
    
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
    };
    
private:
    float b2L, b2R, b1L, b1R, a0L, a0R; //filter coefficients
    float z1L, z1R, z2L, z2R; //delays
    float cut, res; //cutoff frequency, resonance (Q)
    float frec; //normalized cutoff frequency
    float bw; //bandwith
    float currentSampleRate;
    float envAmount;
};

#endif /* Filter_h */
