//
//  SawOscillator.h
//  fmsynth
//
//  Created by Keepin' Soul on 17/12/2021.
//

#ifndef SawOscillator_h
#define SawOscillator_h

class SawOscillator {
public:
    void setFrequency (float frequency)
    {
        freq = frequency;
        auto cyclesPerSample = freq / currentSampleRate;
        angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    float getNextSample(int channel)
    {
        currentAngle[channel] += angleDelta;
        float sample = 2.f * currentAngle[channel] / juce::MathConstants<double>::twoPi - 1.f;
        if(currentAngle[channel] >= juce::MathConstants<double>::twoPi)
        {
            currentAngle[channel] -= juce::MathConstants<double>::twoPi;
            sample = - 1.f;
        }
        return sample;
    };
    float getFrequency()
    {
        return freq;
    };
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
    };
    
private:
    float currentAngle[2], angleDelta;
    float currentSampleRate;
    float freq;
};

#endif /* SawOscillator_h */
