/*
  ==============================================================================

    StyleSheet.cpp
    Created: 3 Dec 2023 1:43:30pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#include "StyleSheet.h"
#include "PluginGlobals.h"

void CustomLNF::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, 
                                 float sliderPos, 
                                 float rotaryStartAngle, float rotaryEndAngle, 
                                 juce::Slider& slider)
{
    const float radius = juce::jmin((float)width * 0.5f, (float)height * 0.5f) - 2.0f;
    const float centreX = (float)x + (float)width * 0.5f;
    const float centreY = (float)y + (float)height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

    if (radius > 12.0f)
    {


        const float thickness = 0.7f;

        {
            g.setColour(THEMES_MAIN_VISUALIZER_BG);
            juce::Path unFilledArc;
            unFilledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
            g.fillPath(unFilledArc);


            g.setColour(THEMES_MAIN_PARAM_FILL.withAlpha(isMouseOver ? 1.0f : 0.7f));
            juce::Path filledArc;
            filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
            g.fillPath(filledArc);
        }

        // Center Rect
        {
            const float innerRadius = radius * 0.2f;
            juce::Path p;
            //p.addTriangle(-innerRadius, 0.0f,
            //    0.0f, -radius * thickness * 1.1f,
            //    innerRadius, 0.0f);
            p.addLineSegment(juce::Line<float>(0.0f, 0.0f, 0.0f, -radius * thickness * 0.9f), innerRadius * thickness);
            
            //float val JUCE_LIVE_CONSTANT(0.75f);
            p.addEllipse(-innerRadius * 0.5f, -innerRadius * 0.5f, innerRadius * 1.0f, innerRadius * 1.0f);

            g.fillPath(p, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        }

        if (slider.isEnabled())
            g.setColour(THEMES_MAIN_PARAM_OUTLINE);
        else
            g.setColour(juce::Colour(0x80808080));

        juce::Path outlineArc;
        outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
        outlineArc.closeSubPath();

        g.strokePath(outlineArc, juce::PathStrokeType(slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));

        // Gold Marker
        {
            const float innerRadius = radius * 0.2f;
            juce::Path p;
            p.addLineSegment(juce::Line<float>(0.0f, -radius * thickness, 0.0f, -radius * thickness - innerRadius * 1.5f), innerRadius * thickness);

            g.setColour(THEMES_MAIN_TEXT);
            g.fillPath(p, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
        }
    }
    else
    {
        if (slider.isEnabled())
            g.setColour(THEMES_MAIN_PARAM_FILL.withAlpha(isMouseOver ? 1.0f : 0.7f));
        else
            g.setColour(juce::Colour(0x80808080));

        juce::Path p;
        p.addEllipse(-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        juce::PathStrokeType(rw * 0.1f).createStrokedPath(p, p);

        p.addLineSegment(juce::Line<float>(0.0f, 0.0f, 0.0f, -radius), rw * 0.2f);

        g.fillPath(p, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    }
}

void CustomLNF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos,
    float minSliderPos,
    float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

    if (slider.isBar())
    {
        //g.setColour(slider.findColour(juce::Slider::trackColourId));
        g.setColour(THEMES_MAIN_PARAM_FILL);
    	g.fillRect(slider.isHorizontal() ? juce::Rectangle<float>(static_cast<float> (x), (float)y + 0.5f, sliderPos - (float)x, (float)height - 1.0f)
            : juce::Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f, (float)y + ((float)height - sliderPos)));

        drawLinearSliderOutline(g, x, y, width, height, style, slider);
    }
    else
    {
        auto isTwoVal = (style == juce::Slider::SliderStyle::TwoValueVertical || style == juce::Slider::SliderStyle::TwoValueHorizontal);
        auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == juce::Slider::SliderStyle::ThreeValueHorizontal);

        auto trackWidth = juce::jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.25f);



        juce::Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
                                      slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

        juce::Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
                                    slider.isHorizontal() ? startPoint.y : (float)y);

        juce::Path backgroundTrack;

        // Outline
        backgroundTrack.startNewSubPath(startPoint);
        backgroundTrack.lineTo(startPoint);
        g.setColour(THEMES_MAIN_BORDER.withAlpha(isMouseOver ? 1.0f : 0.7f));
        g.strokePath(backgroundTrack, { trackWidth * 1.1f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        // Background
        backgroundTrack.startNewSubPath(startPoint);
        backgroundTrack.lineTo(endPoint);
        g.setColour(THEMES_MAIN_VISUALIZER_BG);
    	g.strokePath(backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        juce::Path valueTrack;
        juce::Point<float> minPoint, maxPoint, thumbPoint;

        if (isTwoVal || isThreeVal)
        {
            minPoint = { slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
                         slider.isHorizontal() ? (float)height * 0.5f : minSliderPos };

            if (isThreeVal)
                thumbPoint = { slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
                               slider.isHorizontal() ? (float)height * 0.5f : sliderPos };

            maxPoint = { slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
                         slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos };
        }
        else
        {
            auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
            auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

            minPoint = startPoint;
            maxPoint = { kx, ky };
        }

        auto thumbWidth = getSliderThumbRadius(slider);

        valueTrack.startNewSubPath(minPoint);
        valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
        g.setColour(THEMES_MAIN_PARAM_FILL.withAlpha(isMouseOver ? 1.0f : 0.7f));
        g.strokePath(valueTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });

        if (!isTwoVal)
        {
            g.setColour(THEMES_MAIN_TEXT);
        	g.fillEllipse(juce::Rectangle<float>(static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre(isThreeVal ? thumbPoint : maxPoint));
        }

        if (isTwoVal || isThreeVal)
        {
            auto sr = juce::jmin(trackWidth, (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
            auto pointerColour = slider.findColour(juce::Slider::thumbColourId);

            if (slider.isHorizontal())
            {
                drawPointer(g, minSliderPos - sr,
                            juce::jmax(0.0f, (float)y + (float)height * 0.5f - trackWidth * 2.0f),
                    trackWidth * 2.0f, pointerColour, 2);

                drawPointer(g, maxSliderPos - trackWidth,
                            juce::jmin((float)(y + height) - trackWidth * 2.0f, (float)y + (float)height * 0.5f),
                    trackWidth * 2.0f, pointerColour, 4);
            }
            else
            {
                drawPointer(g, juce::jmax(0.0f, (float)x + (float)width * 0.5f - trackWidth * 2.0f),
                    minSliderPos - trackWidth,
                    trackWidth * 2.0f, pointerColour, 1);

                drawPointer(g, juce::jmin((float)(x + width) - trackWidth * 2.0f, (float)x + (float)width * 0.5f), maxSliderPos - sr,
                    trackWidth * 2.0f, pointerColour, 3);
            }
        }

        if (slider.isBar())
            drawLinearSliderOutline(g, x, y, width, height, style, slider);
    }
}


void CustomLNF::drawPropertyComponentLabel(juce::Graphics& g, int width, int height, juce::PropertyComponent& component)
{
    auto indent = getPropertyComponentIndent(component);

    g.setColour(THEMES_MAIN_TEXT);

    g.setFont((float)juce::jmin(height, 24) * 0.65f);

    auto r = getPropertyComponentContentPosition(component);

    g.drawFittedText(component.getName(),
        indent, r.getY(), r.getX() - 5, r.getHeight(),
        juce::Justification::centredLeft, 2);
}


int CustomLNF::getPropertyComponentIndent(juce::PropertyComponent& component)
{
    return juce::jmin(10, component.getWidth() / 10);
}

//============================================================================================
void CustomSliderWithLabels::paint(juce::Graphics& g)
{
    using namespace juce;

    auto startAng = degreesToRadians(180.f + 45);
    auto endAng = degreesToRadians(180.f - 45.f) + MathConstants<float>::twoPi;

    auto range = getRange();

    auto sliderBounds = getSliderBounds();

    getLookAndFeel().drawRotarySlider(g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(),
            range.getStart(),
            range.getEnd(),
            0.0, 1.0),
        startAng,
        endAng, *this);

    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getWidth() * 0.5f;
    float padding = sliderBounds.getWidth() / 4.f;

    g.setColour(THEMES_MAIN_TEXT);
    g.setFont(getTextHeight());

    auto numChoices = labels.size();
    for (int i = 0; i < numChoices; ++i)
    {
        auto pos = labels[i].pos;
        jassert(0.f <= pos);
        jassert(pos <= 1.f);

        auto ang = jmap(pos, 0.f, 1.f, startAng, endAng);

        auto c = center.getPointOnCircumference(radius + getTextHeight() + 2, ang);

        Rectangle<float> r;
        auto str = labels[i].label;
        r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
        r.setCentre(c);
        r.setY(r.getY() + getTextHeight());

        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 1);
    }


    // Display
    if (displayTextBox)
    {
        Rectangle<float> r;
        auto str = getDisplayString();
        g.setFont(juce::Font(g.getClipBounds().getHeight() / 7));
        r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
        auto c = juce::Point<float>(center.getX(), center.getY() + radius - getTextHeight());
    	r.setCentre(c);

        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centredBottom, 1);
    }

}

juce::Rectangle<int> CustomSliderWithLabels::getSliderBounds() const
{
    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= getTextHeight();

    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), 0);
    r.setY(2);

    return r;
}

int CustomSliderWithLabels::getTextHeight() const
{
    return 14;
}


juce::String CustomSliderWithLabels::getDisplayString() const
{
    const float paramValue = param->getValue();
    const juce::NormalisableRange<float> range = param->getNormalisableRange();
    const float newParamValue = ((range.end - range.start) * paramValue) + range.start;

    if (isPercentage)
    {
        const float valueAsPercent = (newParamValue - range.start) / (range.end - range.start);
        return juce::String((std::roundf(valueAsPercent * 100.f) / 100.f) * 100) + "%";
    } 
    if (suffix.isNotEmpty())
        return juce::String(juce::String(std::round(newParamValue * 100.f) / 100.f)) + suffix;
    return juce::String(newParamValue);
}



