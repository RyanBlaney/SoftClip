/*
  ==============================================================================

    SourceCode.h
    Created: 1 Dec 2023 6:45:54pm
    Author:  Ryan Blaney

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RamperLinear
{
public:

    RamperLinear() = default;
    ~RamperLinear() = default;

    void prepare(float _current, float _delta) {
        current = _current;
        target = current;
        //Ensure that delta is positive to allow for optimizations in getNext()
        delta = std::abs(_delta);
    }

    //Resetting the ramper is a useful way to snap to the target value without ramping.
    void reset() {
        current = target;
    }

    void updateTarget(float newTarget) {
        target = newTarget;
        //Calculate direction here to optimize getNext()
        direction = (target > current) ? 1.0f : -1.0f;
    };

    float getNext()
    {
        if (current != target) {
            if (juce::isWithin(target, current, delta)) {
                current = target;
            }
            else {
                current += delta * direction;
            }
        }
        return current;
    }

private:
    float current, target, direction, delta;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RamperLinear);
};
