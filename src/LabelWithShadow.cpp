//--------------------------------------------------------------------------------------
//
//  File:       LabelWithShadow.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for a text label with a shadow.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms,
//              with or without modification, are permitted provided that the following
//              conditions are met:
//                * Redistributions of source code must retain the above copyright
//                  notice, this list of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright
//                  notice, this list of conditions and the following disclaimer in the
//                  documentation and/or other materials provided with the
//                  distribution.
//                * Neither the name of the copyright holders nor the names of its
//                  contributors may be used to endorse or promote products derived
//                  from this software without specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
//              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created:    2014-05-12
//
//--------------------------------------------------------------------------------------

#include "LabelWithShadow.h"
#include "Utilities.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a text label with a shadow. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The default width to be used for the labels. */
static const int kOurDefaultLabelWidth = 20;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

float LabelWithShadow::shadowWidth(3);

ofColor LabelWithShadow::shadowColor(128);

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

LabelWithShadow::LabelWithShadow(IconlessPanel * parent) :
            inherited(), _parent(parent)
{
	_thisShadowColor = shadowColor;
    _thisShadowWidth = shadowWidth;
} // LabelWithShadow::LabelWithShadow

LabelWithShadow::LabelWithShadow(IconlessPanel *       parent,
                                 ofParameter<string> label,
                                 const float         width,
                                 const float         height) :
            inherited(label, width, height), _parent(parent)
{
	_thisShadowColor = shadowColor;
    _thisShadowWidth = shadowWidth;
} // LabelWithShadow::LabelWithShadow

LabelWithShadow::~LabelWithShadow(void)
{
} // LabelWithShadow::~LabelWithShadow

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

float LabelWithShadow::calculateTextWidth(void)
{
    string name;
    
    if (! getName().empty())
    {
        name = getName() + ": ";
    }
    name += static_cast<string>(label);
    ofRectangle bbox = getTextBoundingBox(name, 0, 0);
    
    return ((0 <= bbox.width) ? (bbox.width + (2 * textPadding)) : 0);
} // LabelWithShadow::calculateTextWidth

void LabelWithShadow::generateDraw(void)
{
    float newWidth = calculateTextWidth();
    float parentWidth = _parent->getWidth();
    
    if ((newWidth > getWidth()) || (newWidth > _parent->getWidth()))
    {
        b.width = newWidth;
        _parent->setWidth(newWidth);
    }
    else if (getWidth() < parentWidth)
    {
        b.width = parentWidth;
    }
    _shadow.clear();
    _shadow.setFillColor(_thisShadowColor);
    _shadow.setFilled(true);
    _shadow.rectangle(b.x + _thisShadowWidth, b.y + _thisShadowWidth, b.width, b.height);
    inherited::generateDraw();
} // LabelWithShadow::generateDraw

void LabelWithShadow::render(void)
{
    _shadow.draw();
    inherited::render();
} // LabelWithShadow::render

LabelWithShadow * LabelWithShadow::setup(ofParameter<string> label,
                                         const float         width,
                                         const float         height)
{
    LabelWithShadow * result = static_cast<LabelWithShadow *>(inherited::setup(label, width, height));

    b.width = kOurDefaultLabelWidth;
    return result;
} // LabelWithShadow::setup

LabelWithShadow * LabelWithShadow::setup(string      labelName,
                                         string      label,
                                         const float width,
                                         const float height)
{
    LabelWithShadow * result = static_cast<LabelWithShadow *>(inherited::setup(labelName, label, width, height));
    
    b.width = kOurDefaultLabelWidth;
    return result;
} // LabelWithShadow::setup

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)