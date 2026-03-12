/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginGlobals.h"
#include <memory>

//==============================================================================
SoftClipAudioProcessor::SoftClipAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()

              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      valueTreeState(*this, nullptr, "Parameters", createParameterLayout()),
      presetManager(valueTreeState)
#endif
{
  valueTreeState.addParameterListener(PARAM_DRIVE_ID, this);
  valueTreeState.addParameterListener(PARAM_OUTPUT_ID, this);

  valueTreeState.addParameterListener(PARAM_CURVE_ID, this);

  valueTreeState.addParameterListener(PARAM_EQ_BASS_ID, this);
  valueTreeState.addParameterListener(PARAM_EQ_MID_ID, this);
  valueTreeState.addParameterListener(PARAM_EQ_TREBLE_ID, this);
  valueTreeState.addParameterListener(PARAM_EQ_PRESENCE_ID, this);

  valueTreeState.addParameterListener(PARAM_MIX_ID, this);

  // valueTreeState.undoManager =
}

SoftClipAudioProcessor::~SoftClipAudioProcessor() {
  valueTreeState.removeParameterListener(PARAM_DRIVE_ID, this);
  valueTreeState.removeParameterListener(PARAM_OUTPUT_ID, this);

  valueTreeState.removeParameterListener(PARAM_CURVE_ID, this);

  valueTreeState.removeParameterListener(PARAM_EQ_BASS_ID, this);
  valueTreeState.removeParameterListener(PARAM_EQ_MID_ID, this);
  valueTreeState.removeParameterListener(PARAM_EQ_TREBLE_ID, this);
  valueTreeState.removeParameterListener(PARAM_EQ_PRESENCE_ID, this);

  valueTreeState.removeParameterListener(PARAM_MIX_ID, this);
}

//==============================================================================
const juce::String SoftClipAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool SoftClipAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool SoftClipAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool SoftClipAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double SoftClipAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int SoftClipAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int SoftClipAudioProcessor::getCurrentProgram() { return 0; }

void SoftClipAudioProcessor::setCurrentProgram(int index) {}

const juce::String SoftClipAudioProcessor::getProgramName(int index) {
  return {};
}

void SoftClipAudioProcessor::changeProgramName(
    int index, const juce::String &newName) {}

//==============================================================================
void SoftClipAudioProcessor::prepareToPlay(double sampleRate,
                                                 int samplesPerBlock) {
  processSpec.sampleRate = sampleRate;
  processSpec.maximumBlockSize = samplesPerBlock;
  processSpec.numChannels = getTotalNumOutputChannels();

  float drive =
      valueTreeState.getRawParameterValue(PARAM_DRIVE_ID)->load();
  float outputGain =
      valueTreeState.getRawParameterValue(PARAM_OUTPUT_ID)->load();

  softClip.prepare(processSpec);
}

void SoftClipAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoftClipAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void SoftClipAudioProcessor::processBlock(
    juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  juce::dsp::AudioBlock<float> audioBlock{buffer};
  juce::dsp::ProcessContextReplacing<float> context =
      juce::dsp::ProcessContextReplacing<float>(audioBlock);

  softClip.process(context);
  oscilloscope.processBlock(buffer.getReadPointer(0), buffer.getNumSamples());
  oscilloscopeBorder.processBlock(buffer.getReadPointer(0),
                                  buffer.getNumSamples());

  /*for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
          buffer.clear(i, 0, buffer.getNumSamples());


  for (int channel = 0; channel < getNumInputChannels(); ++channel) {
          float* channelData = buffer.getWritePointer(channel);

          for (int i = 0; i < buffer.getNumSamples(); ++i) {
                  channelData[i] = tubeDistortion.processSample(channelData[i]);
          }
  }*/

  // tubeDistortion.process(juce::dsp::ProcessContextReplacing<float>
  // (audioBlock));
}

//==============================================================================
bool SoftClipAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *SoftClipAudioProcessor::createEditor() {
  return new SoftClipAudioProcessorEditor(*this);
  // return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void SoftClipAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.

  auto state = valueTreeState.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void SoftClipAudioProcessor::setStateInformation(const void *data,
                                                       int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.

  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));

  if (xmlState.get() != nullptr)
    if (xmlState->hasTagName(valueTreeState.state.getType()))
      valueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new SoftClipAudioProcessor();
}

//==============================================================================

// Value Tree States
juce::AudioProcessorValueTreeState::ParameterLayout
SoftClipAudioProcessor::createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout layout;

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID(PARAM_DRIVE_ID, 1), PARAM_DRIVE_NAME,
      juce::NormalisableRange<float>(0.1, GAIN_MAX / 2.f, 0.025f,
                                     DRIVE_SKEW_FACTOR, false),
      0.1f));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID(PARAM_OUTPUT_ID, 1), PARAM_OUTPUT_NAME,
      juce::NormalisableRange<float>(-GAIN_MAX, GAIN_MAX, 0.025f,
                                     GAIN_SKEW_FACTOR, true),
      0.f));

  juce::StringArray harmonicOrderChoice = {"1", "2", "3", "4", "5",
                                           "6", "7", "8", "9"};
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      juce::ParameterID(PARAM_CURVE_ID, 1), PARAM_CURVE_NAME, harmonicOrderChoice,
      1));

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID(PARAM_EQ_BASS_ID, 1), PARAM_EQ_BASS_NAME,
      juce::NormalisableRange<float>(-GAIN_MAX, GAIN_MAX, 0.1f, 1.f, false),
      0.f));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID(PARAM_EQ_MID_ID, 1), PARAM_EQ_MID_NAME,
      juce::NormalisableRange<float>(-GAIN_MAX, GAIN_MAX, 0.1f, 1.f, false),
      0.f));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID(PARAM_EQ_TREBLE_ID, 1), PARAM_EQ_TREBLE_NAME,
      juce::NormalisableRange<float>(-GAIN_MAX, GAIN_MAX, 0.1f, 1.f, false),
      0.f));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID(PARAM_EQ_PRESENCE_ID, 1), PARAM_EQ_PRESENCE_NAME,
      juce::NormalisableRange<float>(-GAIN_MAX, GAIN_MAX, 0.1f, 1.f, false),
      0.f));
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID(PARAM_MIX_ID, 1), PARAM_MIX_NAME,
      juce::NormalisableRange<float>(0.f, 1.f, 0.05f, 1.f, false), 1.f));

  return layout;
}

// Value Tree Listener
void SoftClipAudioProcessor::parameterChanged(
    const juce::String &parameterID, float newValue) {
  if (parameterID.equalsIgnoreCase(PARAM_DRIVE_ID))
    softClip.setDrive(newValue);
  if (parameterID.equalsIgnoreCase(PARAM_OUTPUT_ID))
    softClip.setOutput(newValue);
  if (parameterID.equalsIgnoreCase(PARAM_MIX_ID))
    softClip.setMix(newValue);
  if (parameterID.equalsIgnoreCase(PARAM_CURVE_ID))
    softClip.setCurve(newValue + 1);
  if (parameterID.equalsIgnoreCase(PARAM_EQ_BASS_ID))
    softClip.setBassLevel(newValue);
  if (parameterID.equalsIgnoreCase(PARAM_EQ_MID_ID))
    softClip.setMidLevel(newValue);
  if (parameterID.equalsIgnoreCase(PARAM_EQ_TREBLE_ID))
    softClip.setTrebleLevel(newValue);
  if (parameterID.equalsIgnoreCase(PARAM_EQ_PRESENCE_ID))
    softClip.setPresenceLevel(newValue);
}
