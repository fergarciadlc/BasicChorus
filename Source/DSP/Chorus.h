/*
  ==============================================================================

    Chorus.h
    Created: 14 Jul 2022 9:31:40pm
    Author:  Fernando Garcia

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Chorus
{
public:
    Chorus();
    ~Chorus();
    
    void setChorusParameters(int rate = 1, // must be lower than 100 Hz.
                             float depth= 0.25f, //Sets the volume of the LFO modulating the chorus delay line (between 0 and 1)
                             int centreDelay = 20,   //Sets the centre delay in milliseconds of the chorus delay line modul. This delay must be between 1 and 100 ms.
                             float feedback = 0.1f, // Sets the feedback volume (between -1 and 1) of the chorus delay line
                             float mix = 0.5f); // Sets the amount of dry and wet signal in the output of the chorus (between 0 for full dry and 1 for full wet).
    
    void prepare(juce::dsp::ProcessSpec spec);
    void process(juce::AudioBuffer<float>& inBuffer);
    
private:
    juce::dsp::Chorus<float> chorus;
};


//struct chorusParams
//{
//    int rate = 1;
//    float depth = 0.25f;
//    int centreDelay = 20;
//    float feedback = 0.1f;
//    float mix = 0.5f;
//};
