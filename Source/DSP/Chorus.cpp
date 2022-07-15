/*
  ==============================================================================

    Chorus.cpp
    Created: 14 Jul 2022 9:31:40pm
    Author:  Fernando Garcia

  ==============================================================================
*/

#include "Chorus.h"
Chorus::Chorus() {}
Chorus::~Chorus() {}

void Chorus::setChorusParameters(int rate, float depth, int centreDelay, float feedback, float mix)
{
    chorus.setRate(rate);
    chorus.setDepth(depth);
    chorus.setCentreDelay(centreDelay);
    chorus.setFeedback(feedback);
    chorus.setMix(mix);
    
}

void Chorus::prepare(juce::dsp::ProcessSpec spec)
{
    
    chorus.prepare(spec);
    chorus.reset();
    setChorusParameters();
}

void Chorus::process(juce::AudioBuffer<float>& inBuffer)
{
    
    
    juce::dsp::AudioBlock<float> sampleBlock (inBuffer);
    chorus.process (juce::dsp::ProcessContextReplacing<float> (sampleBlock));
}

