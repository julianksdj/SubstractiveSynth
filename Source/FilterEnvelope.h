//
//  FilterEnvelope.h
//  SubstractiveSynth
//
//  Created by Keepin' Soul on 26/12/2021.
//

#ifndef FilterEnvelope_h
#define FilterEnvelope_h

class FilterEnvelope
{

public:
    FilterEnvelope()
    {
        envCount[0] = 0;
        envCount[1] = 0;
    };
    float getEnvelope(int channel)
    {
        //Calculate envelope
        if(noteOn) //pressed note on the keyboard
        {
            if (envCount[channel] < attackSamples)
            {
                currentFreq[channel] = currentFreq[channel] + ((maxFreq-cut)/attackSamples); //the amplitude raises until max amplitude is reached
            }
            else if (envCount[channel] < daSamples)
            {
                currentFreq[channel] = currentFreq[channel] - (maxFreq-sustainFreq)/decaySamples; //the amplitude decreases until sustain level is reached
            }
            releaseFreq[channel] = currentFreq[channel];
        }
        else //released note on the keyboard
        {
            if(currentFreq[channel]> (20.f+(releaseFreq[channel]-20.f)/releaseSamples))
            {
                currentFreq[channel] = currentFreq[channel] - envAmount*(releaseFreq[channel] - 20.f)/releaseSamples;
            }
        }
        envCount[channel]++;
        return currentFreq[channel];
    };
    
    void setAttack(float a)
    {
        attackTime = a;
        attackSamples = currentSampleRate * attackTime;
    };
    void setDecay(float d)
    {
        decayTime = d;
        decaySamples = currentSampleRate * decayTime;
        daSamples = decaySamples + attackSamples;
    };
    void setSustain(float s)
    {
        sustain = s;
    };
    void setRelease(float r)
    {
        releaseTime = r;
        releaseSamples = currentSampleRate * releaseTime;
    };
    void setCut(float cutoff){
        cut = cutoff;
        currentFreq[0] = cutoff;
        currentFreq[1] = cutoff;
        maxEnvAmount = envAmount * (20000.0 - cut);
        maxFreq = cut + maxEnvAmount;
        sustainFreq = cut + maxEnvAmount * sustain;
    };
    void setFilterEnv(float env){
        envAmount = env;
        maxEnvAmount = envAmount * (20000.0 - cut);
        maxFreq = cut + maxEnvAmount;
        sustainFreq = cut + maxEnvAmount * sustain;
        envCount[0] = 0;
        envCount[1] = 0;
        currentFreq[0] = cut;
        currentFreq[1] = cut;
    };
    void initFilterEnv(float cutoff, float env){
        cut = cutoff;
        currentFreq[0] = cutoff;
        currentFreq[1] = cutoff;
        envAmount = env;
        maxEnvAmount = envAmount * (20000.0 - cut);
        maxFreq = cut + maxEnvAmount;
        sustainFreq = cut + maxEnvAmount * sustain;
        envCount[0] = 0;
        envCount[1] = 0;
    };
    void setNoteOn(bool n){
        noteOn = n;
    };
    void resetEnvCount(){
        envCount[0] = 0;
        envCount[1] = 0;
    };
    void setSampleRate(float sr){
        currentSampleRate = sr;
    };
    
private:
    float currentSampleRate;
    float currentFreq[2];
    float attackTime, decayTime, sustainFreq, releaseTime;
    int attackSamples, decaySamples, releaseSamples, daSamples;
    int envCount[2];
    bool noteOn;
    float releaseFreq[2];
    float cut;
    float maxFreq;
    float envAmount;
    float maxEnvAmount;
    float sustain;
};


#endif /* FilterEnvelope_h */
