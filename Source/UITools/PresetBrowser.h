/*
  ==============================================================================

    PresetBrowser.h
    Created: 15 Jan 2024 5:13:51pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PresetManager.h"

class PresetPanel : public juce::Component, public juce::Button::Listener, public juce::ComboBox::Listener
{
public:
    PresetPanel(PresetManager& pm);

    ~PresetPanel() override;

    void resized() override;

    void setBackgroundColor(const juce::Colour& colour) { backgroundColour = colour; }

    void setTextColour(const juce::Colour& colour) { textColour = colour; }


private:

    void buttonClicked(juce::Button*) override;

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    void configureButton(juce::TextButton& button, const juce::String& buttonName);

    void loadPresetsInComboBox();

    PresetManager& presetManager;
    std::unique_ptr<juce::FileChooser> fileChooser;

    juce::TextButton saveButton, deleteButton, seekButton, skipButton;
    juce::ComboBox presetList;

    juce::Colour backgroundColour, textColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};
