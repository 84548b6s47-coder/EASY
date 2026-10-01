#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class EASYAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit EASYAudioProcessorEditor (EASYAudioProcessor&);
    ~EASYAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    EASYAudioProcessor& processor;

    juce::Slider amount;
    juce::Slider maxCut;
    juce::ToggleButton delta { "DELTA" };

    juce::Label amountLabel;
    juce::Label maxCutLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> amountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> maxCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> deltaAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EASYAudioProcessorEditor)
};
