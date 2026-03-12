/*
  ==============================================================================

    StyleSheet.h
    Created: 3 Dec 2023 1:43:30pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLNF : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider) override;
    void drawPropertyComponentLabel(juce::Graphics&, int width, int height, juce::PropertyComponent&) override;
    int getPropertyComponentIndent(juce::PropertyComponent& component);
};

struct CustomSliderWithLabels : juce::Slider
{
public:
    CustomSliderWithLabels(juce::RangedAudioParameter& raParam, const juce::String& unitSuffix, bool displayAsPercent, bool displayValue) :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
            juce::Slider::TextEntryBoxPosition::NoTextBox),
        param(&raParam),
        suffix(unitSuffix),
		isPercentage(displayAsPercent),
		displayTextBox(displayValue)
    {
        setLookAndFeel(&lnf);
    }

    ~CustomSliderWithLabels()
    {
        setLookAndFeel(nullptr);
    }

    struct LabelPos
    {
        float pos;
        juce::String label;
    };

    juce::Array<LabelPos> labels;
    juce::Label textBox;

    void paint(juce::Graphics& g) override;
    juce::Rectangle <int> getSliderBounds() const;
    int getTextHeight() const;
    juce::String getDisplayString() const;

private:
    CustomLNF lnf;

    juce::RangedAudioParameter* param;
    juce::String suffix;
    bool isPercentage;
    bool displayTextBox;
};


