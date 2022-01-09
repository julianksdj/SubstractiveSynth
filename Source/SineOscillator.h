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
        modFreq = 500;
        modAmp = 0.0000001;
    };
    void setFrequency (float frequency)
    {
        carrFreq = frequency;
        auto cyclesPerSample = carrFreq / currentSampleRate;
        angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    void setFrequencyFM (float frequency)
    {
        modFreq=frequency;
        auto cyclesPerSample = modFreq / currentSampleRate;
        angleDeltaFM = cyclesPerSample * juce::MathConstants<double>::twoPi;
    };
    float getNextSample(int channel)
    {
        float sample = std::sin(currentAngle[channel] + modAmp * std::cos(currentAngleFM[channel]));
        currentAngle[channel] += angleDelta;
        currentAngleFM[channel] += angleDeltaFM;
        if (currentAngle[channel]>=juce::MathConstants<double>::twoPi)
            currentAngle[channel] -= juce::MathConstants<double>::twoPi;
        if (currentAngleFM[channel]>=juce::MathConstants<double>::twoPi)
            currentAngleFM[channel] -= juce::MathConstants<double>::twoPi;
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
    void setModFreq(float m)
    {
        modFreq = m;
    };
    void setModAmp(float m)
    {
        modAmp = m;
    };
    void setSampleRate(float sr)
    {
        currentSampleRate = sr;
    };
    
private:
    float currentAngle[2], currentAngleFM[2], angleDelta, angleDeltaFM;
    float currentSampleRate;
    float carrFreq;
    float modFreq;
    float modAmp;
    float modAmpSmoothed;
};

#endif /* SineOscillator_h */
