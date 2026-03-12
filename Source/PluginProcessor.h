/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Saturators/SoftClip.h"
#include "UITools/AudioOscilloscope.h"
#include "UITools/DecibelLitBorder.h"
#include "UITools/PresetManager.h"

//==============================================================================
/**
 */
class SoftClipAudioProcessor
    : public juce::AudioProcessor,
      public juce::AudioProcessorValueTreeState::Listener {
public:
  //==============================================================================
  SoftClipAudioProcessor();
  ~SoftClipAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  PresetManager &getPresetManager() { return presetManager; }

  //==============================================================================
  // PLUGIN PARAMETERS

  juce::AudioProcessorValueTreeState valueTreeState{
      *this, nullptr, "Parameters", createParameterLayout()};

  // DSP

  SoftClip softClip;

  AudioOscilloscope oscilloscope;
  DecibelLitBorder oscilloscopeBorder;

private:
  juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
  void parameterChanged(const juce::String &parameterID,
                        float newValue) override;

  juce::dsp::ProcessSpec processSpec;

  PresetManager presetManager;

  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoftClipAudioProcessor)
};
