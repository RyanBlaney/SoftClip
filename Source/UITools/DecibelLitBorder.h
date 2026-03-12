/*
  ==============================================================================

    DecibelLitBorder.h
    Created: 21 Jan 2024 4:51:33pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DecibelLitBorder : public juce::Component
{
public:
	/** This is a border element that changes color depending on the 
	 *  decibel volume playing.
	 */
	DecibelLitBorder();

    //==============================================================================
    /** Processes a number of samples displaying them on the scope.

        The resolution will depend on how zoomed in you are.

        @see setHorizontalZoom.
    */
    void processBlock(const float* inputChannelData,
        int numSamples);

    /** Clears the internal buffers.

        It is a good idea to call this when an audio device is started or stopped
        to avoid clicks on the scope.
    */
    void clear();

    juce::Colour getCurrentColour() const;

    /** Sets the clipping colour of the border. */
    void setClippingColour(const juce::Colour& newClippingColour) { clippingColour = newClippingColour; }

    /** Sets the medium gain colour of the border. */
    void setMediumLevelColour(const juce::Colour& newMediumLevelColour) { mediumLevelColour = newMediumLevelColour; }

    /** Sets the normal colour of the border. */
    void setNormalColour(const juce::Colour& newNormalColour) { normalColour = newNormalColour; }

    void setRectangleRounding(float cornerSize) { rectangleRounding = cornerSize; }

    void setBorderSize(float borderSize) { lineThickness = borderSize; }

    void setClippingThreshold(float newClippingThreshold) { clippingThreshold = newClippingThreshold; }


    //==============================================================================
    /** @internal Used to add a sample to the internal buffer. */
    void addSample(const float sample);

    /** @internal Used to set the blended colour based on the dynamics */
    void setCurrentColour(const float& sample);

    //==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void paint(juce::Graphics& g) override;


private:
    juce::Colour clippingColour;
    juce::Colour mediumLevelColour;
    juce::Colour normalColour;
    juce::Colour currentColour;

    float rectangleRounding;
    float lineThickness;
    float clippingThreshold;

    int bufferSizeMask;
    float currentMax, currentMin;
    std::atomic<int> bufferPos, lastBufferPos, bufferSize, numSamplesIn;
    float bufferLastMax, bufferLastMin;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DecibelLitBorder)
};