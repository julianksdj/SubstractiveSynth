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
    void setFrequency (float frequency)
    {
        freq = frequency;
        auto cyclesPerSample = freq / currentSampleRate;
        angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    float getNextSample(int channel)
    {
        float sample = 0.f;
        // 0 to pi
        if(currentAngle[channel] < juce::MathConstants<double>::pi)
        {
            sample = 1.f;
            currentAngle[channel] += angleDelta;
        }
        // pi to twoPi
        else
        {
            sample = -1.f;
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

#endif /* SquareOscillator_h */
