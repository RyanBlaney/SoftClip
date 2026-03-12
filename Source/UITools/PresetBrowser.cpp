/*
  ==============================================================================

    PresetBrowser.cpp
    Created: 15 Jan 2024 8:14:13pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#include "PresetBrowser.h"

PresetPanel::PresetPanel(PresetManager& pm) :
	presetManager(pm)
{
    configureButton(saveButton, "SAVE");
    configureButton(deleteButton, "DELETE");
    configureButton(seekButton, "<");
    configureButton(skipButton, ">");

    presetList.setEditableText(false);
    presetList.setTextWhenNothingSelected("");
    presetList.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    presetList.addListener(this);

    loadPresetsInComboBox();

    addAndMakeVisible(presetList);
}

PresetPanel::~PresetPanel()
{
    saveButton.removeListener(this);
    deleteButton.removeListener(this);
    seekButton.removeListener(this);
    skipButton.removeListener(this);
    presetList.removeListener(this);
}

void PresetPanel::resized()
{
    const auto padding = getLocalBounds().getWidth() / 64;
    const auto container = getLocalBounds();
    auto bounds = container;


    saveButton.setBounds(bounds.removeFromLeft(proportionOfWidth(0.2f)).reduced(padding));
    seekButton.setBounds(bounds.removeFromLeft(proportionOfWidth(0.1f)).reduced(padding));
    deleteButton.setBounds(bounds.removeFromRight(proportionOfWidth(0.2f)).reduced(padding));
    skipButton.setBounds(bounds.removeFromRight(proportionOfWidth(0.1f)).reduced(padding));
    presetList.setBounds(bounds.reduced(padding));
}


void PresetPanel::configureButton(juce::TextButton& button, const juce::String& buttonName)
{
    button.setButtonText(buttonName);
    button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    button.addListener(this);
    addAndMakeVisible(button);
}

void PresetPanel::buttonClicked(juce::Button* button)
{
	if (button == &saveButton)
	{
        fileChooser = std::make_unique<juce::FileChooser>(
								            "Enter the name of the preset to save.",
								            presetManager.getPresetsDirectory(),
								            "*." + presetManager.getExtension(),
								            true);
        fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser)
	        {
        		const auto result = chooser.getResult();
                presetManager.addPreset(result.getFileNameWithoutExtension());
                loadPresetsInComboBox();
	        });
	}

    if (button == &deleteButton)
    {
        presetManager.deletePreset(presetManager.getCurrentPreset());
        loadPresetsInComboBox();
    }

    if (button == &seekButton)
    {
        presetManager.loadPreviousPreset();
    }

    if (button == &skipButton)
    {
        presetManager.loadNextPreset();
    }

    presetList.setText(presetManager.getCurrentPreset());
}

void PresetPanel::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == &presetList)
	{
        presetManager.loadPreset(presetList.getItemText(presetList.getSelectedItemIndex()));
	}
}

void PresetPanel::loadPresetsInComboBox()
{
    presetList.clear();
    const auto allPresets = presetManager.getAllPresets();
    const auto currentPreset = presetManager.getCurrentPreset();

    presetList.addItemList(allPresets, 1);
    presetList.setSelectedItemIndex(allPresets.indexOf(currentPreset), juce::dontSendNotification);
}

