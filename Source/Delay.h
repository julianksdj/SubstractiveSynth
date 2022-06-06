/*
  ==============================================================================

    Delay.h
    Created: 20 Feb 2022 3:58:26pm
    Author:  Keepin' Soul

  ==============================================================================
*/

#ifndef Delay_h
#define Delay_h

class Delay
{

public:
    Delay()
    {
    };
    float getDelayTime(){
        return delayTime;
    };
    float getDelayFeed(){
        return feedbackParam;
    };
    float getDelayDry(){
        return wetLevelParam;
    };
    void setDelayTime(float dt){
        delayTime = dt;
        cookVariables();
    };
    void setDelayFeed(float fb){
        feedbackParam = fb;
        cookVariables();
    };
    void setDelayDry(float dw){
        wetLevelParam = dw;
        cookVariables();
    };
    void cookVariables(){
        delaySamples = delayTime*(currentSampleRate/1000.0);
        feedback = feedbackParam/100.0;
        wetLevel = wetLevelParam/100.0;
        // subtract to make read index
        readIndex[0] = writeIndex[0] - (int)delaySamples; // cast as int!
        readIndex[1] = writeIndex[1] - (int)delaySamples;
        // check and wrap BACKWARDS if the index is negative
        if (readIndex[0] < 0)
            readIndex[0] += bufferSize;     // amount of wrap is Read + Length
        if (readIndex[1] < 0)
            readIndex[1] += bufferSize;
    };
    void resetDelay()
    {
        // flush buffer
        if(pBufferL)
            memset(pBufferL, 0, bufferSize*sizeof(float));
        if(pBufferR)
            memset(pBufferR, 0, bufferSize*sizeof(float));
        // init read/write indices
        writeIndex[0] = 0; // reset the Write index to top
        writeIndex[1] = 0;
        readIndex[0] = 0; // reset the Read index to top
        readIndex[1] = 0;
    };
    void switchSync()
    {
        delaySync = !delaySync;
    };
    bool getSync()
    {
        return delaySync;
    };
    void processDelay(float * channelData, int channel, int numSamples)
    {
        for (int sample=0; sample<numSamples; sample++)
        {
            // Do LEFT (MONO) Channel
            // Read the Input
            float xn = channelData[sample];
            // Read the output of the delay at readIndex
            float yn;
            if(channel == 0)
                yn = pBufferR[readIndex[channel]];
            else
                yn = pBufferL[readIndex[channel]];
            // if zero delay, just pass the input to output
            if(delaySamples == 0)
                yn = xn;
            // write the input to the delay
            if(channel == 0)
                pBufferL[writeIndex[channel]] = xn + feedback*yn;
            else
                pBufferR[writeIndex[channel]] = xn + feedback*yn;
            // create the wet/dry mix and write to the output buffer
            // dry = 1 - wet
            channelData[sample] = wetLevel * yn + (1.f - wetLevel)*xn;
            
            // incremnent the pointers and wrap if necessary
            writeIndex[channel]++;
            if(writeIndex[channel] >= bufferSize)
                 writeIndex[channel] = 0;
            readIndex[channel]++;
            if(readIndex[channel] >= bufferSize)
                 readIndex[channel] = 0;
        }
    };
    void initDelay(float sr, float dTime, float dFeed, float dWet)
    {
        currentSampleRate = sr;
        
        bufferSize = 2 * currentSampleRate;     // 2 seconds delay @ fs
        pBufferL = new float[bufferSize];
        pBufferR = new float[bufferSize];
        resetDelay();
        
        delayTime = dTime;
        feedbackParam = dFeed;
        wetLevelParam = dWet;
        cookVariables();
        //sync button enable/disable (future feature)
        //delaySync = false;
    };
    
private:
    float currentSampleRate;
    float delayTime;
    float delaySamples;
    float feedback, feedbackParam;
    float wetLevel, wetLevelParam;
    // circular buffer
    float* pBufferL;
    float* pBufferR;
    int readIndex[2];
    int writeIndex[2];
    int bufferSize;
    //sync button enable/disable (future feature)
    bool delaySync;
};


#endif /* Envelope_h */
