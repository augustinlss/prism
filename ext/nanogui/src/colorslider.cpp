/*
    nanogui/slider.cpp -- Fractional slider widget with mouse control

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/colorslider.h>
#include <iostream>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/serializer/core.h>

NAMESPACE_BEGIN(nanogui)

ColorSlider::ColorSlider(Widget *parent)
    : Widget(parent), mValue(0.0f), mRange(-50, 50),
      mHighlightedRange(0.f, 0.f) {
    mHighlightColor = Color(255, 80, 80, 70);
}

Vector2i ColorSlider::preferredSize(NVGcontext *) const {
    return Vector2i(70, 16);
}

bool ColorSlider::mouseDragEvent(const Vector2i &p, const Vector2i & /* rel */,
                            int /* button */, int /* modifiers */) {
    if (!mEnabled)
        return false;

    const float kr = (int) (mSize.y() * 0.4f), kshadow = 3;
    const float startX = kr + kshadow + mPos.x() - 1;
    const float widthX = mSize.x() - 2 * (kr + kshadow);

    float value = (p.x() - startX) / widthX;
    value = value * (mRange.second - mRange.first) + mRange.first;
    mValue = (int) std::min(std::max(value, mRange.first), mRange.second);

    std::cout << "Slider Value: " << mValue << std::endl;

    if (mCallback)
        mCallback(mValue);
    return true;
}

bool ColorSlider::mouseButtonEvent(const Vector2i &p, int /* button */, bool down, int /* modifiers */) {
    if (!mEnabled)
        return false;

    const float kr = (int) (mSize.y() * 0.4f), kshadow = 3;
    const float startX = kr + kshadow + mPos.x() - 1;
    const float widthX = mSize.x() - 2 * (kr + kshadow);

    float value = (p.x() - startX) / widthX;
    value = value * (mRange.second - mRange.first) + mRange.first;
    mValue = std::min(std::max(value, mRange.first), mRange.second);
    if (mCallback)
        mCallback(mValue);
    if (mFinalCallback && !down)
        mFinalCallback(mValue);
    return true;
}

void ColorSlider::draw(NVGcontext* ctx) {
    Vector2f center = mPos.cast<float>() + mSize.cast<float>() * 0.5f;
    float kr = (int) (mSize.y() * 0.4f), kshadow = 3;

    float startX = kr + kshadow + mPos.x();
    float widthX = mSize.x() - 2*(kr+kshadow);

    Vector2f knobPos(startX + (mValue - mRange.first) /
            (mRange.second - mRange.first) * widthX,
            center.y() + 0.5f);

    NVGcolor startColor = nvgRGBA(255, 0, 0, mEnabled ? 32 : 10);  // Start color (red)
    NVGcolor endColor = nvgRGBA(0, 255, 0, mEnabled ? 128 : 210);  // End color (green)

    NVGpaint bg = nvgBoxGradient(
        ctx, startX, center.y() - 3 + 1, widthX, 6, 3, 3,
        startColor, endColor);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, startX, center.y() - 3 + 1, widthX, 6, 2);
    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    if (mHighlightedRange.second != mHighlightedRange.first) {
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, startX + mHighlightedRange.first * mSize.x(),
                       center.y() - kshadow + 1,
                       widthX *
                           (mHighlightedRange.second - mHighlightedRange.first),
                       kshadow * 2, 2);
        nvgFillColor(ctx, mHighlightColor);
        nvgFill(ctx);
    }

    NVGpaint knobShadow =
        nvgRadialGradient(ctx, knobPos.x(), knobPos.y(), kr - kshadow,
                          kr + kshadow, Color(0, 64), mTheme->mTransparent);

    nvgBeginPath(ctx);
    nvgRect(ctx, knobPos.x() - kr - 5, knobPos.y() - kr - 5, kr * 2 + 10,
            kr * 2 + 10 + kshadow);
    nvgCircle(ctx, knobPos.x(), knobPos.y(), kr);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, knobShadow);
    nvgFill(ctx);

    NVGpaint knob = nvgLinearGradient(ctx,
        mPos.x(), center.y() - kr, mPos.x(), center.y() + kr,
        mTheme->mBorderLight, mTheme->mBorderMedium);
    NVGpaint knobReverse = nvgLinearGradient(ctx,
        mPos.x(), center.y() - kr, mPos.x(), center.y() + kr,
        mTheme->mBorderMedium,
        mTheme->mBorderLight);

    nvgBeginPath(ctx);
    nvgCircle(ctx, knobPos.x(), knobPos.y(), kr);
    nvgStrokeColor(ctx, mTheme->mBorderDark);
    nvgFillPaint(ctx, knob);
    nvgStroke(ctx);
    nvgFill(ctx);
    nvgBeginPath(ctx);
    nvgCircle(ctx, knobPos.x(), knobPos.y(), kr/2);
    nvgFillColor(ctx, Color(150, mEnabled ? 255 : 100));
    nvgStrokePaint(ctx, knobReverse);
    nvgStroke(ctx);
    nvgFill(ctx);

    std::string valueLabel = std::to_string((int)mValue);
    nvgFillColor(ctx, Color(255, 255, 255, 100)); // White color
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFontFace(ctx, "sans");
    nvgFontSize(ctx, 14.0f);
    nvgText(ctx, center.x() + 80, center.y(), valueLabel.c_str(), nullptr); // Adjusted position
}

void ColorSlider::save(Serializer &s) const {
    Widget::save(s);
    s.set("value", mValue);
    s.set("range", mRange);
    s.set("highlightedRange", mHighlightedRange);
    s.set("highlightColor", mHighlightColor);
}

bool ColorSlider::load(Serializer &s) {
    if (!Widget::load(s)) return false;
    if (!s.get("value", mValue)) return false;
    if (!s.get("range", mRange)) return false;
    if (!s.get("highlightedRange", mHighlightedRange)) return false;
    if (!s.get("highlightColor", mHighlightColor)) return false;
    return true;
}

NAMESPACE_END(nanogui)
