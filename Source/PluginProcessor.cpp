/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicChorusAudioProcessor::BasicChorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "parameters", createParameters())
#endif
{
}

BasicChorusAudioProcessor::~BasicChorusAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout BasicChorusAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameters;

    parameters.add(std::make_unique<juce::AudioParameterInt>("rate",
                                                             "rate",
                                                             0,
                                                             5,
                                                             1));

    parameters.add(std::make_unique<juce::AudioParameterFloat>("depth",
                                                               "depth",
                                                               juce::NormalisableRange<float>(0.0f, 1.0f),
                                                               0.25f));
    
    parameters.add(std::make_unique<juce::AudioParameterInt>("centreDelay",
                                                             "centreDelay",
                                                             10,
                                                             100,
                                                             1));
    
    parameters.add(std::make_unique<juce::AudioParameterFloat>("feedback",
                                                               "feedback",
                                                               juce::NormalisableRange<float>(-1.0f, 1.0f),
                                                               0.0f));
    
    parameters.add(std::make_unique<juce::AudioParameterFloat>("mix",
                                                               "mix",
                                                               juce::NormalisableRange<float>(0.0f, 1.0f),
                                                               0.25f));

    return parameters;
}

//==============================================================================
const juce::String BasicChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicChorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicChorusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicChorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    
    chorus.prepare (spec);
}

void BasicChorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BasicChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    
    chorusParams.rate = apvts.getRawParameterValue("rate")->load();
    chorusParams.depth = apvts.getRawParameterValue("depth")->load();
    chorusParams.centreDelay = apvts.getRawParameterValue("centreDelay")->load();
    chorusParams.feedback = apvts.getRawParameterValue("feedback")->load();
    chorusParams.mix = apvts.getRawParameterValue("mix")->load();


    chorus.process(buffer, chorusParams);
}

//==============================================================================
bool BasicChorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicChorusAudioProcessor::createEditor()
{
    //return new BasicChorusAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void BasicChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BasicChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicChorusAudioProcessor();
}
