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
        envCount = 0;
    };

    float getEnvelope()
    {
        //Calculate envelope
        if(noteOn) //pressed note on the keyboard
        {
            //if (envCount<attackSamples && currentFreq<maxFreq)
            if (envCount < attackSamples)
            {
                //currentFreq = currentFreq + ((maxFreq-20.0)/attackSamples); //the amplitude raises until max amplitude is reached
                currentFreq = currentFreq + ((maxFreq-cut)/attackSamples); //the amplitude raises until max amplitude is reached
            }
            else if (envCount < daSamples)
            {
                currentFreq = currentFreq - (maxFreq-sustainFreq)/decaySamples; //the amplitude decreases until sustain level is reached
            }
            //releaseFreq = currentFreq * envAmount; // saves the amplitude state
            releaseFreq = currentFreq;
            //printf("sustain %f\n",sustainFreq);
            //printf("max freq %f\n",maxFreq);
            //printf("max env amount %f\n",maxEnvAmount);
            //printf("cut %f\n", cut);
            //printf("freq %f\n",currentFreq);
            //printf("sample %d\n", envCount);
            //printf("\n");
        }
        else //released note on the keyboard
        {
            //if(currentFreq > 20.0)
            if(currentFreq> (20.0+(releaseFreq-20.0)/releaseSamples))
            {
                //currentFreq = currentFreq - releaseFreq/releaseSamples; //amplitude decreases until silence
                //currentFreq = currentFreq - (releaseFreq - 20.0)/releaseSamples;
                currentFreq = currentFreq - envAmount*(releaseFreq - 20.0)/releaseSamples;
            }
        }
        envCount++;
        return currentFreq;
    };
    
    void setAttack(float a)
    {
        attackTime = a;
        attackSamples = currentSampleRate * attackTime;
        //printf("Attack time: %f\n", attackTime);
        //printf("Attack samples: %d\n", attackSamples);
    };
    void setDecay(float d)
    {
        decayTime = d;
        decaySamples = currentSampleRate * decayTime;
        //printf("decay time %f\n", decayTime);
        daSamples = decaySamples + attackSamples;
        //printf("decay Samples %d\n", decaySamples);
        //printf("da Samples %d\n", daSamples);
    };
    void setSustain(float s)
    {
        sustain = s;
    };
    void setRelease(float r)
    {
        releaseTime = r;
        releaseSamples = currentSampleRate * releaseTime;
        //printf("release level %f\n", releaseFreq);
        //printf("release time %f\n", releaseTime);
        //printf("release samples %d\n", releaseSamples);
    };
    void setCut(float cutoff){
        cut = cutoff;
        //currentFreq = 20.0;
        currentFreq = cut;
        maxEnvAmount = envAmount * (20000.0 - cut);
        maxFreq = cut + maxEnvAmount;
        sustainFreq = cut + maxEnvAmount * sustain;
    };
    void setFilterEnv(float env){
        envAmount = env;
        maxEnvAmount = envAmount * (20000.0 - cut);
        maxFreq = cut + maxEnvAmount;
        sustainFreq = cut + maxEnvAmount * sustain;
        envCount = 0;
        //currentFreq = 20.0;
        currentFreq = cut;
    };
    void initFilterEnv(float cutoff, float env){
        cut = cutoff;
        //currentFreq = 20.0;
        currentFreq = cut;
        envAmount = env;
        maxEnvAmount = envAmount * (20000.0 - cut);
        maxFreq = cut + maxEnvAmount;
        sustainFreq = cut + maxEnvAmount * sustain;
        envCount = 0;
    };
    void setNoteOn(bool n){
        noteOn = n;
    };
    void resetEnvCount(){
        envCount = 0;
    };
    void setSampleRate(float sr){
        currentSampleRate = sr;
    };
    
private:
    float currentSampleRate;
    float currentFreq;
    float attackTime, decayTime, sustainFreq, releaseTime;
    int attackSamples, decaySamples, releaseSamples, daSamples;
    int envCount;
    bool noteOn;
    float releaseFreq;
    float cut;
    float maxFreq;
    float envAmount;
    float maxEnvAmount;
    float sustain;
};


#endif /* FilterEnvelope_h */
