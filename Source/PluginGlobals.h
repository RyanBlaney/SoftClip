/*
  ==============================================================================

    PluginGlobals.h
    Created: 17 Nov 2023 12:34:40am
    Author:  Ryan Blaney

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/** === Plugin Theme === */
extern const juce::Colour THEMES_MAIN_BACKGROUND;
extern const juce::Colour THEMES_MAIN_TEXT;
extern const juce::Colour THEMES_MAIN_PARAM_FILL;
extern const juce::Colour THEMES_MAIN_PARAM_OUTLINE;
extern const juce::Colour THEMES_MAIN_BORDER;
extern const juce::Colour THEMES_MAIN_VISUALIZER_BG;


/** === Parameter IDs === */

extern const float DRIVE_SKEW_FACTOR;
extern const float GAIN_SKEW_FACTOR;
extern const float GAIN_MAX;

extern const juce::String PARAM_DRIVE_ID;
extern const juce::String PARAM_DRIVE_NAME;

extern const juce::String PARAM_OUTPUT_ID;
extern const juce::String PARAM_OUTPUT_NAME;

extern const juce::String PARAM_CURVE_ID;
extern const juce::String PARAM_CURVE_NAME;

extern const juce::String PARAM_EQ_BASS_ID;
extern const juce::String PARAM_EQ_BASS_NAME;

extern const juce::String PARAM_EQ_MID_ID;
extern const juce::String PARAM_EQ_MID_NAME;

extern const juce::String PARAM_EQ_TREBLE_ID;
extern const juce::String PARAM_EQ_TREBLE_NAME;

extern const juce::String PARAM_EQ_PRESENCE_ID;
extern const juce::String PARAM_EQ_PRESENCE_NAME;

extern const juce::String PARAM_MIX_ID;
extern const juce::String PARAM_MIX_NAME;
