//
//  Envelope.h
//  fmsynth
//
//  Created by Keepin' Soul on 28/11/2021.
//

#ifndef Envelope_h
#define Envelope_h

class Envelope
{

public:
    Envelope()
    {
        envCount[0] = 0;
        envCount[1] = 0;
    };

    float getEnvelope(int channel)
    {
        //Calculate envelope
        if(noteOn==true) //pressed note on the keyboard
        {
            if (envCount[channel] < attackSamples)
            {
                carrAmp[channel]+=(0.125f/attackSamples); //the amplitude raises until max amplitude is reached
            }
            else if (envCount[channel] < daSamples)
            {
                carrAmp[channel]-=((0.125f-sustainLevel)/decaySamples); //the amplitude decreases until sustain level is reached
            }
            releaseLevel[channel] = carrAmp[channel]; // saves the amplitude state
        }
        else //released note on the keyboard
        {
            if(carrAmp[channel]>0.0000001f)
                carrAmp[channel]-=(releaseLevel[channel]/releaseSamples); //amplitude decreases until silence
        }
        envCount[channel]++;
        return carrAmp[channel];
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
        sustainLevel = s;
    };
    void setRelease(float r)
    {
        releaseTime = r;
        releaseSamples = currentSampleRate * releaseTime;
    };
    float getCarrAmp(int channel){
        return carrAmp[channel];
    };
    void setCarrAmp(float a, int channel){
        carrAmp[channel]=a;
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
    bool isActive(){
        if (carrAmp[0]<0.0000001)
            return false;
        else
            return true;
    };
    
private:
    float currentSampleRate;
    float carrAmp[2];
    float attackTime, decayTime, sustainLevel, releaseTime;
    int attackSamples, decaySamples, releaseSamples, daSamples;
    int envCount[2];
    bool noteOn;
    float releaseLevel[2];
};


#endif /* Envelope_h */
