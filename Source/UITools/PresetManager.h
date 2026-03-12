/*
  ==============================================================================

    PresetManager.h
    Created: 15 Jan 2024 10:03:41pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PresetManager
{
public:

    inline static const juce::String extension = "lab";

    juce::File defaultDirectory;

    PresetManager(juce::AudioProcessorValueTreeState &);

    void addPreset(const juce::String& presetName);

    void deletePreset(const juce::String& presetName);

    void loadPreset(const juce::String& presetName);

    void loadNextPreset();

    void loadPreviousPreset();

    juce::StringArray getAllPresets() const;

    juce::String getCurrentPreset() const;

    const juce::File& getPresetsDirectory() const { return defaultDirectory; }

    const juce::String getExtension() const { return extension; }

    juce::File getVst3PresetsPath() const;

private:

    juce::AudioProcessorValueTreeState & valueTreeState;
    juce::String currentPreset;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetManager)
};
