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
    SawOscillator()
    {
    };
    void setFrequency (float frequency)
    {
        carrFreq = frequency;
        carrFreq0 = frequency;
        auto cyclesPerSample = carrFreq / currentSampleRate;
        angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    float getNextSample(int channel)
    {
        // twoPi
        currentAngle[channel] += angleDelta;
        float sample = currentAngle[channel] / juce::MathConstants<double>::twoPi;
        if(currentAngle[channel] >= juce::MathConstants<double>::twoPi)
        {
            currentAngle[channel] -= juce::MathConstants<double>::twoPi;
            sample = 0.0;
        }
        return sample;
    };
    float getFrequency()
    {
        return carrFreq0;
    };
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
    };
    
private:
    float currentAngle[2], angleDelta;
    float currentSampleRate;
    float carrFreq, carrFreq0;
    float modAmp;
    float modAmpSmoothed;
    float amp;
};

#endif /* SawOscillator_h */
