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

void Chorus::setChorusParameters(chorusParams params)
{
    
    chorus.setRate(params.rate);
    chorus.setDepth(params.depth);
    chorus.setCentreDelay(params.centreDelay);
    chorus.setFeedback(params.feedback);
    chorus.setMix(params.mix);
}

void Chorus::prepare(juce::dsp::ProcessSpec spec)
{
    chorusParams params;
    chorus.prepare(spec);
    chorus.reset();
    setChorusParameters(params);
}

void Chorus::process(juce::AudioBuffer<float>& inBuffer, chorusParams params)
{
//    setChorusParameters(params);
//    juce::dsp::AudioBlock<float> sampleBlock (inBuffer);
//    chorus.process (juce::dsp::ProcessContextReplacing<float> (sampleBlock));
//
    setChorusParameters(params);
    juce::dsp::AudioBlock<float> block(inBuffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    chorus.process(context);
}

