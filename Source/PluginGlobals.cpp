/*
  ==============================================================================

    PluginGlobals.cpp
    Created: 17 Nov 2023 12:34:40am
    Author:  Ryan Blaney

  ==============================================================================
*/

#include "PluginGlobals.h"

/** === Plugin Theme === */
extern const juce::Colour THEMES_MAIN_BACKGROUND = juce::Colour(0xff1a1a1a);  
extern const juce::Colour THEMES_MAIN_TEXT = juce::Colour(0xffe0e0e0);  
extern const juce::Colour THEMES_MAIN_PARAM_FILL = juce::Colour(0xff4ecdc4);  
extern const juce::Colour THEMES_MAIN_PARAM_OUTLINE = juce::Colour(0xff1c1c1c);  
extern const juce::Colour THEMES_MAIN_BORDER = juce::Colour(0xff1a1a1a);  
extern const juce::Colour THEMES_MAIN_VISUALIZER_BG = juce::Colour(0xff0f0f0f);


/** === Parameter IDs === */

extern const float DRIVE_SKEW_FACTOR = 0.666f;
extern const float GAIN_SKEW_FACTOR = 0.666f;
extern const float GAIN_MAX = 24.f;

extern const juce::String PARAM_DRIVE_ID = "Drive";
extern const juce::String PARAM_DRIVE_NAME = "Drive";

extern const juce::String PARAM_OUTPUT_ID = "Output";
extern const juce::String PARAM_OUTPUT_NAME = "Output";

extern const juce::String PARAM_CURVE_ID = "Curve";
extern const juce::String PARAM_CURVE_NAME = "Curve";

extern const juce::String PARAM_EQ_BASS_ID = "EQ_Bass";
extern const juce::String PARAM_EQ_BASS_NAME = "Bass";

extern const juce::String PARAM_EQ_MID_ID = "EQ_Mid";
extern const juce::String PARAM_EQ_MID_NAME = "Mid";

extern const juce::String PARAM_EQ_TREBLE_ID = "EQ_Treble";
extern const juce::String PARAM_EQ_TREBLE_NAME = "Treble";

extern const juce::String PARAM_EQ_PRESENCE_ID = "EQ_Presence";
extern const juce::String PARAM_EQ_PRESENCE_NAME = "Presence";

extern const juce::String PARAM_MIX_ID = "MIX";
extern const juce::String PARAM_MIX_NAME = "Mix";
