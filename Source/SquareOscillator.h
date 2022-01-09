//
//  SquareOscillator.h
//  fmsynth
//
//  Created by Keepin' Soul on 17/12/2021.
//

#ifndef SquareOscillator_h
#define SquareOscillator_h

class SquareOscillator {
public:
    SquareOscillator()
    {
        //modFreq = 500;
        modAmp = 0.0000001;
    };
    void setFrequency (float frequency)
    {
        carrFreq = frequency;
        auto cyclesPerSample = carrFreq / currentSampleRate;
        angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    float getNextSample(int channel)
    {
        float sample = std::sin(currentAngle[channel]);
        //currentAngle[channel] += angleDelta;
        // 0 to pi
        if(currentAngle[channel] < juce::MathConstants<double>::pi)
        {
            sample = 1.0;
            currentAngle[channel] += angleDelta;
        }
        // pi to twoPi
        else
        {
            sample = 0.0;
            currentAngle[channel] += angleDelta;
        }
        // twoPi
        if(currentAngle[channel] >= juce::MathConstants<double>::twoPi)
        {
            currentAngle[channel] -= juce::MathConstants<double>::twoPi;
        }
        return sample;
    };
    float getFrequency()
    {
        return carrFreq;
    };
    void setCarrFreq(float m)
    {
        carrFreq = m;
    };
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
    };
    
private:
    float currentAngle[2], angleDelta;
    float currentSampleRate;
    float carrFreq;
    float modAmp;
    float modAmpSmoothed;
};

#endif /* SquareOscillator_h */
