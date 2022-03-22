//
//  Oscillator.h
//
//  substractiveSynth
//
//  Created by Keepin' Soul on 22/04/2022.
//
//  PolyBLEP Saw & Square Waves
//  Gordon Smith Sine Wave

#ifndef Oscillator_h
#define Oscillator_h

class Oscillator {
public:
    Oscillator(){

    };
    void cookFrequency()
    {
        inc = freq/currentSampleRate;
        modulo[0] = 0.5f;
        modulo[1] = 0.5f;
        
        // sinewave oscillator
        B = 4.f/juce::MathConstants<double>::pi;
        C = -4.f/(juce::MathConstants<double>::pi*juce::MathConstants<double>::pi);
        P = 0.225f;
        
        angleDelta = inc * juce::MathConstants<double>::twoPi;
    };
    
    // CQBLimitedOscillator (Wil Pirkle Synths Book 5.32)
    inline float getNextSawSample(int channel, float fModulo, float fInc)
    {
        float bipolar = 0.f;
        float blep_saw = 0.f;
        
        
        //unipolar to bipolar
        bipolar = 2.f * fModulo - 1.f;
        blep_saw = bipolar + doPolyBLEP(fModulo, abs(fInc), 1.f, false);
        
        //increment counter
        fModulo += fInc;
        
        return blep_saw;
    };
    
    // CQBLimitedOscillator (Wil Pirkle Synths Book 5.32)
    inline float getNextSquSample(int channel, float fModulo, float fInc)
    {
        //get 1st sawtooth
        float saw1 = getNextSawSample(channel,fModulo, fInc);
        //phase shift on second oscillator
        if(fInc > 0.f)
            //modulo[channel] += pulseWidth/100.f;
            fModulo += 0.5f; //50% pulse width
        else
            //modulo[channel] -= pulseWidth/100.f;
            fModulo -= 0.5f;
        // for positive freqs
        if(fInc > 0.f && fModulo >= 1.f)
            fModulo -= 1.f;
        // for negative freqs
        if(fInc < 0.f && fModulo <= 0.f)
            fModulo += 1.f;
        
        // get 2nd sawtooth
        float saw2 = getNextSawSample(channel, fModulo, fInc);
        // substract 180 out of phase
        float blep_squ = 0.5f * saw1 - 0.5f * saw2;
        // calculate DC correction
        //float corr = 1.f/pulseWidth;
        float corr = 2.f; // 1/0.5=2
        // apply correction
        blep_squ *= corr;
        
        return blep_squ;
    };
    
    // Sinusoidal OSC Parabolic approximation (Wil Pirkle Synths Book 5.22)
    inline float getNextSinSample(int channel, float fModulo, float fInc)
    {
        //
        float sample = B * currentAngle[channel] + C * currentAngle[channel] * abs(currentAngle[channel]);
        // high precission
        sample = P * (sample * abs(sample) - sample) + sample;
        
        currentAngle[channel] += angleDelta;
        if (currentAngle[channel]>=juce::MathConstants<double>::pi)
            currentAngle[channel] -= juce::MathConstants<double>::twoPi;
        
        return sample;
    };
    
    inline float getNextSample(int channel)
    {
        // wrap
        if (modulo[channel] >= 1.f)
            modulo[channel] -= 1.f;
        
        float sample = 0.f;
        if (waveform == 1)
            sample = getNextSawSample(channel, modulo[channel], inc);
        else if (waveform == 2)
            sample = getNextSquSample(channel, modulo[channel], inc);
        else if (waveform == 3)
            sample = getNextSinSample(channel, modulo[channel], inc);
        
        //increment counter
        modulo[channel] += inc;
        return sample;
    };
    
    // BLEP & polyBLEP algorithms (Wil Pirkle Synths Book 5.10)
    inline float doPolyBLEP(float fModulo, float fInc, float fHeight, bool bRisingEdge)
    {
        float fPolyBLEP = 0.f;
        
        // leftside of discontinuity
        // -1 < t < 0
        if(fModulo > 1.f - fInc)
        {
            // calculate distance
            float t = (fModulo-1.f)/fInc;
            // calculate residual
            fPolyBLEP = fHeight*(t*t + 2.f*t + 1.f);
        }
        // rightside of discontinuity
        // 0 <= t < 1
        else if(fModulo < fInc)
        {
            // calculate distance
            float t = fModulo/fInc;
            // calculate residual
            fPolyBLEP = fHeight*(2.f*t - t*t - 1.f);
        }
        
        // substract for falling, add for rising edge
        if(!bRisingEdge)
            fPolyBLEP *= -1.f;
        
        return fPolyBLEP;
    };
    
    // =============== setters ====================
    void setFrequency(float f)
    {
        freq = f;
        cookFrequency();
    };
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
    };
    void setWaveform(int w)
    {
        waveform = w;
        cookFrequency();
    };
    // =============================================
    // =============== getters ====================
    float getFrequency()
    {
        return freq;
    };
    int getWaveform()
    {
        return waveform;
    };
    // =============================================
    
private:

    float modulo[2];
    float inc;
    
    float currentSampleRate;
    float freq;
    
    float currentSample[2];
    
    int waveform;
    
    // sine oscillator
    float B;
    float C;
    float P;
    
    float currentAngle[2];
    float angleDelta;
};

#endif /* Oscillator_h */
