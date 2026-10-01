#include "PluginEditor.h"

EASYAudioProcessorEditor::EASYAudioProcessorEditor (EASYAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setSize (760, 430);

    amount.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    amount.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 24);

    maxCut.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    maxCut.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 90, 24);
    maxCut.setTextValueSuffix (" dB");

    amountLabel.setText ("AMOUNT", juce::dontSendNotification);
    maxCutLabel.setText ("MAX CUT", juce::dontSendNotification);

    for (auto* label : { &amountLabel, &maxCutLabel })
    {
        label->setJustificationType (juce::Justification::centred);
        addAndMakeVisible (*label);
    }

    addAndMakeVisible (amount);
    addAndMakeVisible (maxCut);
    addAndMakeVisible (delta);

    amountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.apvts, "amount", amount);

    maxCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.apvts, "maxCut", maxCut);

    deltaAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processor.apvts, "delta", delta);
}

void EASYAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB (18, 18, 20));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (34.0f, juce::Font::bold));
    g.drawText ("EASY", 28, 20, getWidth() - 56, 48, juce::Justification::centredLeft);

    g.setColour (juce::Colours::grey);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("Intelligent Resonance Control", 30, 66, getWidth() - 60, 24,
                juce::Justification::centredLeft);

    const auto spectrumArea = juce::Rectangle<int> (28, 110, getWidth() - 56, 155);
    g.setColour (juce::Colour::fromRGB (28, 28, 32));
    g.fillRoundedRectangle (spectrumArea.toFloat(), 10.0f);
    g.setColour (juce::Colour::fromRGB (70, 70, 78));
    g.drawRoundedRectangle (spectrumArea.toFloat(), 10.0f, 1.0f);

    g.setColour (juce::Colours::darkgrey);
    g.setFont (juce::FontOptions (13.0f));
    g.drawText ("Spectrum / resonance confidence view — next milestone",
                spectrumArea, juce::Justification::centred);
}

void EASYAudioProcessorEditor::resized()
{
    amountLabel.setBounds (165, 295, 140, 24);
    amount.setBounds      (165, 316, 140, 100);

    maxCutLabel.setBounds (325, 295, 140, 24);
    maxCut.setBounds      (325, 316, 140, 100);

    delta.setBounds       (505, 338, 100, 32);
}
