/*
  ==============================================================================

    SoftClip.h
    Created: 17 Nov 2023 3:54:33pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#pragma once

#include "juce_dsp/juce_dsp.h"
#include <JuceHeader.h>
#include <cmath>

class SoftClip : juce::dsp::ProcessorBase {
public:
  SoftClip() {
    drive = 1.0f;
    curve = 1.0f;
    mix = 1.0f;
    output = 0.0f;
    bassLevel = 0.0f;
    midLevel = 0.0f;
    trebleLevel = 0.0f;
    presenceLevel = 0.0f;
  }

  ~SoftClip() override = default;

  //====================================================================================================================

  void prepare(const juce::dsp::ProcessSpec &spec) override {
    sampleRate = spec.sampleRate;
    bassEQ.prepare(spec);
    midEQ.prepare(spec);
    trebleEQ.prepare(spec);
    presenceEQ.prepare(spec);
  }

  void process(const juce::dsp::ProcessContextReplacing<float> &context) override {
    auto &inBlock = context.getInputBlock();
    auto &outBlock = context.getOutputBlock();

    for (auto channel = 0; channel < inBlock.getNumChannels(); channel++) {
      float bassGain = juce::Decibels::decibelsToGain(bassLevel);
      float midGain = juce::Decibels::decibelsToGain(midLevel);
      float trebleGain = juce::Decibels::decibelsToGain(trebleLevel);
      float presenceGain = juce::Decibels::decibelsToGain(presenceLevel);

      if (bassLevel != 0.0f) {
        *(bassEQ.state) = juce::dsp::IIR::ArrayCoefficients<float>::makeLowShelf(
            sampleRate, 250.0f, 0.66f, bassGain);
      }
      if (midLevel != 0.0f) {
        *(midEQ.state) = juce::dsp::IIR::ArrayCoefficients<float>::makePeakFilter(
            sampleRate, 500.0f, 0.66f, midGain);
      }
      if (trebleLevel != 0.0f) {
        *(trebleEQ.state) = juce::dsp::IIR::ArrayCoefficients<float>::makePeakFilter(
            sampleRate, 2400.0f, 0.66f, trebleGain);
      }
      if (presenceLevel != 0.0f) {
        *(presenceEQ.state) = juce::dsp::IIR::ArrayCoefficients<float>::makeHighShelf(
            sampleRate, 5000.0f, 0.66f, presenceGain);
      }

      // Process samples
      for (auto s = 0; s < inBlock.getNumSamples(); s++) {
        float sample = inBlock.getSample(channel, s);
        float outputSample = processSample(sample);
        outBlock.setSample(channel, s, outputSample);
      }
    }
  }

  //====================================================================================================================

  float processSample(float sample) {
    float driven = sample * drive;

    float clipped = std::tanh(driven * curve);

    float wet = clipped;
    float dry = sample;
    float mixed = (wet * mix) + (dry * (1.0f - mix));
  
    float postGain = juce::Decibels::decibelsToGain(output);

    return std::clamp(mixed * postGain, -1.0f, 1.0f);
  }

  //====================================================================================================================
  void reset() override {
    bassEQ.reset();
    midEQ.reset();
    trebleEQ.reset();
    presenceEQ.reset();
  }

  //====================================================================================================================
  // Parameter Setters
  void setDrive(float value) { drive = juce::jlimit(0.0f, 10.0f, value); }
  void setCurve(float value) { curve = juce::jlimit(1.0f, 6.0f, value); }
  void setMix(float value) { mix = juce::jlimit(0.0f, 1.0f, value); }
  void setOutput(float value) { output = juce::jlimit(-24.0f, 12.0f, value); }
  void setBassLevel(float value) { bassLevel = value; }
  void setMidLevel(float value) { midLevel = value; }
  void setTrebleLevel(float value) { trebleLevel = value; }
  void setPresenceLevel(float value) { presenceLevel = value; }

  //====================================================================================================================
  // Parameter Getters
  float getDrive() const { return drive; }
  float getCurve() const { return curve; }
  float getMix() const { return mix; }
  float getOutput() const { return output; }
  float getBassLevel() const { return bassLevel; }
  float getMidLevel() const { return midLevel; }
  float getTrebleLevel() const { return trebleLevel; }
  float getPresenceLevel() const { return presenceLevel; }

private:
  //====================================================================================================================
  // EQ Filters
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                 juce::dsp::IIR::Coefficients<float>> bassEQ;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                 juce::dsp::IIR::Coefficients<float>> midEQ;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                 juce::dsp::IIR::Coefficients<float>> trebleEQ;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                 juce::dsp::IIR::Coefficients<float>> presenceEQ;

  //====================================================================================================================
  // Parameters
  float drive = 1.0f;        // Input Drive
  float curve = 1.0f;        // Saturation Curve (1.0-6.0)
  float mix = 1.0f;          // Dry/Wet Mix (0.0-1.0)
  float output = 0.0f;       // Post-Gain dB

  //====================================================================================================================
  // EQ Levels (dB)
  float bassLevel = 0.0f;       // 250Hz Low Shelf
  float midLevel = 0.0f;        // 500Hz Peak
  float trebleLevel = 0.0f;     // 2400Hz Peak
  float presenceLevel = 0.0f;   // 5000Hz Peak
                                     
  // DSP
  double sampleRate;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoftClip);
};
