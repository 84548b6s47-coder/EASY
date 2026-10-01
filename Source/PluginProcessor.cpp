#include "PluginProcessor.h"
#include "PluginEditor.h"

EASYAudioProcessor::EASYAudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

void EASYAudioProcessor::prepareToPlay (double, int)
{
}

void EASYAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EASYAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& mainOutput = layouts.getMainOutputChannelSet();
    const auto& mainInput  = layouts.getMainInputChannelSet();

    if (mainOutput != juce::AudioChannelSet::mono()
        && mainOutput != juce::AudioChannelSet::stereo())
        return false;

    return mainInput == mainOutput;
}
#endif

void EASYAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                       juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    for (auto channel = getTotalNumInputChannels();
         channel < getTotalNumOutputChannels();
         ++channel)
        buffer.clear (channel, 0, buffer.getNumSamples());

    // v0.1: intentional transparent pass-through.
    // The resonance detector will be introduced as a separate DSP module next.
}

bool EASYAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* EASYAudioProcessor::createEditor()
{
    return new EASYAudioProcessorEditor (*this);
}

void EASYAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    const auto state = apvts.copyState();
    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void EASYAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessorValueTreeState::ParameterLayout EASYAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        "amount", "Amount",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        50.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat>(
        "maxCut", "Max Cut",
        juce::NormalisableRange<float> (0.0f, 18.0f, 0.1f),
        6.0f));

    layout.add (std::make_unique<juce::AudioParameterBool>(
        "delta", "Delta", false));

    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EASYAudioProcessor();
}
