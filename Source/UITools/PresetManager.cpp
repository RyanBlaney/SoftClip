/*
  ==============================================================================

    PresetManager.cpp
    Created: 15 Jan 2024 10:03:41pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#include "PresetManager.h"
#include "JuceHeader.h"



PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts) :
	valueTreeState(apvts),
    defaultDirectory(getVst3PresetsPath())
{
	// Ensure it exists, but DO NOT ASSERT FALSE if it fails permission-wise.
    if (!defaultDirectory.exists()) {
        const auto result = defaultDirectory.createDirectory();
        if (result.failed()) {
            DBG("Warning: Could not create presets directory '" + defaultDirectory.getFullPathName() 
                + "': " + result.getErrorMessage());
             // Optional: Try falling back to userDocumentsDirectory if VST3 path fails?
        }
    }
    
    currentPreset = ""; 
}

/*
  Helper to resolve the correct preset folder based on OS.
  Prioritizes ~vst3 structure on Linux/Mac/Windows for plugin compatibility.
*/
juce::File PresetManager::getVst3PresetsPath() const {
    juce::String company = ProjectInfo::companyName;
    juce::String project = ProjectInfo::projectName;
    
#ifdef JUCE_LINUX
    // Use home directory path for Linux VST3 presets (User specific) to avoid permission errors.
    return juce::File().getSpecialLocation(juce::File::userHomeDirectory).getChildFile(".vst3/presets/" + company + "/" + project);
        
#elif defined(JUCE_MAC) || defined(JUCE_WINDOWS)
    // Use JUCE defaults for others, or stick to App Data
    return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile(project + "/Presets");

#else
    #error "Unknown Platform"
#endif
}

void PresetManager::addPreset(const juce::String& presetName)
{
	if (presetName.isEmpty()) return;

	const auto xml = valueTreeState.copyState().createXml();
	const auto presetFile = defaultDirectory.getChildFile(presetName  + "." + extension);

	if (!xml->writeTo(presetFile))
	{
		DBG("Could not create preset file: " + presetFile.getFullPathName());
	}

	currentPreset = presetName;
}

void PresetManager::deletePreset(const juce::String& presetName)
{
	if (presetName.isEmpty()) return;

	const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
	if (!presetFile.existsAsFile())
	{
		DBG("The preset file: " + presetFile.getFullPathName() + " does not exist.");
		return;
	}

	if (!presetFile.deleteFile())
	{
		DBG("Could not delete preset file: " + presetFile.getFullPathName());
		return;
	}

	currentPreset = "";
}

void PresetManager::loadPreset(const juce::String& presetName)
{
	if (presetName.isEmpty()) return;

	const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
	if (!presetFile.existsAsFile())
	{
		DBG("The preset file: " + presetFile.getFullPathName() + " does not exist.");
		return;
	}

	juce::XmlDocument xmlDoc{ presetFile };
	const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDoc.getDocumentElement());

	valueTreeState.replaceState(valueTreeToLoad);
	currentPreset = presetName;
}

void PresetManager::loadNextPreset()
{
	const auto allPresets = getAllPresets();
	if (allPresets.isEmpty()) return;

	int index = allPresets.indexOf(currentPreset);

	int nextIndex;
	if (index >= allPresets.size() - 1)
		nextIndex = 0;
	else nextIndex = index + 1;

	loadPreset(allPresets.getReference(nextIndex));
}

void PresetManager::loadPreviousPreset()
{
	const auto allPresets = getAllPresets();
	if (allPresets.isEmpty()) return;

	int index = allPresets.indexOf(currentPreset);

	int prevIndex;
	if (index <= 0)
		prevIndex = allPresets.size() - 1;
	else prevIndex = index - 1;

	loadPreset(allPresets.getReference(prevIndex));
}

juce::StringArray PresetManager::getAllPresets() const
{
	juce::StringArray presets;
	const auto fileArray = defaultDirectory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);
	for (const auto& file : fileArray)
		presets.add(file.getFileNameWithoutExtension());
	return presets;
}

juce::String PresetManager::getCurrentPreset() const
{
	return currentPreset;
}
