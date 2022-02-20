//
//  SineOscillator.h
//  fmsynth
//
//  Created by Keepin' Soul on 21/11/2021.
//

#ifndef SineOscillator_h
#define SineOscillator_h

class SineOscillator {
public:
    SineOscillator()
    {
    };
    void setFrequency (float frequency)
    {
        carrFreq0 = frequency;
        carrFreq = frequency;
        auto cyclesPerSample = carrFreq / currentSampleRate;
        angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    float getNextSample(int channel)
    {
        float sample = std::sin(currentAngle[channel]);
        currentAngle[channel] += angleDelta;
        if (currentAngle[channel]>=juce::MathConstants<double>::twoPi)
            currentAngle[channel] -= juce::MathConstants<double>::twoPi;
        return sample;
    };
    float getFrequency()
    {
        return carrFreq0;
    };
//    void setCarrFreq(float m)
//    {
//        carrFreq = m;
//    };
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
    };
    
private:
    float currentAngle[2], angleDelta;
    float currentSampleRate;
    float carrFreq, carrFreq0;
};

#endif /* SineOscillator_h */
