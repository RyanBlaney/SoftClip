cmake_minimum_required(VERSION 3.22)
project(SoftClip LANGUAGES C CXX)

# Enable compile_commands.json export for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Import full JUCE framework — including CMake utils!
add_subdirectory(external/JUCE)

# Create the plugin target
juce_add_plugin(SoftClip
    VERSION 1.0.0
    PRODUCT_NAME "SoftClip"
    BUNDLE_ID com.ryanblaney.softclip
    COMPANY_NAME "Ryan Blaney"
    PLUGIN_MANUFACTURER_CODE Crtl
    PLUGIN_CODE TDis
    FORMATS AU VST3 Standalone
    DESCRIPTION "Soft Clip Effect by Ryan Blaney"
)

# Attach sources (your plugin code)
target_sources(SoftClip PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/StyleSheet.cpp
    Source/PluginGlobals.cpp
    Source/UITools/AudioOscilloscope.cpp
    Source/UITools/DecibelLitBorder.cpp
    Source/UITools/PresetBrowser.cpp
    Source/UITools/PresetManager.cpp
)

# Set required C++ standard
target_compile_features(SoftClip PRIVATE cxx_std_17)

# Optionally force generation of JuceLibraryCode/JuceHeader.h, AppConfig.h etc.
juce_generate_juce_header(SoftClip)
