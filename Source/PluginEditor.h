/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StyleSheet.h"
#include "UITools/PresetBrowser.h"

//==============================================================================
/**
*/
class SoftClipAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SoftClipAudioProcessorEditor (SoftClipAudioProcessor&);
    ~SoftClipAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void resizeParams();

private:
    SoftClipAudioProcessor& audioProcessor;

    // Sliders
    CustomLNF sliderLNF;

    CustomSliderWithLabels
		driveKnob,
		outputKnob,
		mixKnob,
		curveSlider,
		bassEQKnob,
		midEQKnob,
		trebleEQKnob,
		presenceEQKnob;

    juce::Label
        driveLabel,
        outputLabel,
        mixLabel,
        bassEQLabel,
        midEQLabel,
        trebleEQLabel,
        presenceEQLabel,
        curveLabel;

    PresetPanel presetManager;

    juce::AudioProcessorValueTreeState::SliderAttachment
        driveAttach,
        outputAttach,
        mixAttach,
        curveAttach,
        bassEQAttach,
        midEQAttach,
        trebleEQAttach,
        presenceEQAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftClipAudioProcessorEditor)
};
