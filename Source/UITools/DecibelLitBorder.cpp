/*
  ==============================================================================

    DecibelLitBorder.cpp
    Created: 21 Jan 2024 4:51:33pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#include "DecibelLitBorder.h"

DecibelLitBorder::DecibelLitBorder() :
    clippingColour(juce::Colours::red),
    mediumLevelColour(juce::Colours::yellow),
    normalColour(juce::Colours::blue),
    currentColour(normalColour),
	rectangleRounding(12.f),
	lineThickness(5.f),
	clippingThreshold(.7f),

    bufferSizeMask(2047),
    currentMax(-1.0f),
    currentMin(1.0f),
    bufferPos(0),
    lastBufferPos(0),
    bufferSize(2048), // Needs to be a power of 2 and larger than the width of your scope!
    numSamplesIn(0),
    bufferLastMax(-1.0f),
    bufferLastMin(1.0f)
{

    setOpaque(false);
    clear();

    resized(); //Initialises the image
}

//==============================================================================
void DecibelLitBorder::processBlock(const float* inputChannelData,
    int numSamples)
{
    if (inputChannelData != nullptr)
        for (int i = 0; i < numSamples; ++i)
            addSample(inputChannelData[i]);
}

void DecibelLitBorder::clear()
{
    
}

//==============================================================================
void DecibelLitBorder::resized()
{
    
}

void DecibelLitBorder::paint(juce::Graphics& g)
{
    g.setColour(getCurrentColour());
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), rectangleRounding, lineThickness);

    lastBufferPos = bufferPos.load();

    repaint();
}

void DecibelLitBorder::addSample(const float sample)
{
    if (sample > currentMax)
        currentMax = sample;
    if (sample < currentMin)
        currentMin = sample;

    constexpr int samplesToAverage = 100;

    if (++numSamplesIn > samplesToAverage)
    {
        bufferPos = bufferPos & bufferSizeMask;
        bufferPos++;

        numSamplesIn = 0;
        currentMin = 1.0e6f;
        currentMax = -currentMin;
    }

    setCurrentColour(currentMax);
}

void DecibelLitBorder::setCurrentColour(const float& sample)
{
    if (sample > clippingThreshold)
        currentColour = mediumLevelColour.interpolatedWith(clippingColour, (sample - clippingThreshold) / clippingThreshold);
    else 
		currentColour = normalColour.interpolatedWith(mediumLevelColour, sample / clippingThreshold);
}

juce::Colour DecibelLitBorder::getCurrentColour() const
{
    return currentColour;

}

