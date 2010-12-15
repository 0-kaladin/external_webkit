/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config.h"
#include "AndroidAnimation.h"

#if USE(ACCELERATED_COMPOSITING)

#include "Animation.h"
#include "GraphicsLayerAndroid.h"

#include "Timer.h"
#include "TimingFunction.h"
#include "TranslateTransformOperation.h"
#include "UnitBezier.h"

#include <wtf/CurrentTime.h>

#ifdef DEBUG

#include <cutils/log.h>
#include <wtf/text/CString.h>

#undef XLOG
#define XLOG(...) android_printLog(ANDROID_LOG_DEBUG, "AndroidAnimation", __VA_ARGS__)

#else

#undef XLOG
#define XLOG(...)

#endif // DEBUG


namespace WebCore {

static long gDebugAndroidAnimationInstances;

long AndroidAnimation::instancesCount()
{
    return gDebugAndroidAnimationInstances;
}

AndroidAnimation::AndroidAnimation(const Animation* animation,
                                   double beginTime)
    : m_beginTime(beginTime)
    , m_duration(animation->duration())
    , m_iterationCount(animation->iterationCount())
    , m_currentIteration(0)
    , m_direction(animation->direction())
    , m_currentDirection(false)
    , m_timingFunction(animation->timingFunction())
{
    ASSERT(m_timingFunction);

    if (!static_cast<int>(beginTime)) // time not set
        m_beginTime = WTF::currentTime();

    gDebugAndroidAnimationInstances++;
}

AndroidAnimation::AndroidAnimation(AndroidAnimation* anim)
    : m_beginTime(anim->m_beginTime)
    , m_duration(anim->m_duration)
    , m_iterationCount(anim->m_iterationCount)
    , m_currentIteration(0)
    , m_direction(anim->m_direction)
    , m_currentDirection(false)
    , m_timingFunction(anim->m_timingFunction)
{
    gDebugAndroidAnimationInstances++;
}

AndroidAnimation::~AndroidAnimation()
{
    gDebugAndroidAnimationInstances--;
}

float AndroidAnimation::currentProgress(double time)
{
    if (m_beginTime <= 0.000001) // overflow or not correctly set
        m_beginTime = time;

    m_elapsedTime = time - m_beginTime;

    if (m_duration <= 0)
      m_duration = 0.000001;

    if (m_elapsedTime < 0) // animation not yet started.
        return 0;

    return m_elapsedTime / m_duration;
}

bool AndroidAnimation::checkIterationsAndProgress(double time, float* finalProgress)
{
    float progress = currentProgress(time);

    int currentIteration = static_cast<int>(progress);
    if (currentIteration != m_currentIteration)
        if (m_direction == Animation::AnimationDirectionAlternate)
            swapDirection();

    m_currentIteration = currentIteration;
    progress -= m_currentIteration;

    if ((m_currentIteration >= m_iterationCount)
          && (m_iterationCount != Animation::IterationCountInfinite))
        return false;

    if (m_timingFunction->isCubicBezierTimingFunction()) {
        CubicBezierTimingFunction* bezierFunction = static_cast<CubicBezierTimingFunction*>(m_timingFunction.get());
        UnitBezier bezier(bezierFunction->x1(),
                          bezierFunction->y1(),
                          bezierFunction->x2(),
                          bezierFunction->y2());
        if (m_duration > 0)
            progress = bezier.solve(progress, 1.0f / (200.0f * m_duration));
    }

    *finalProgress = progress;
    return true;
}

PassRefPtr<AndroidOpacityAnimation> AndroidOpacityAnimation::create(
                                                float fromValue,
                                                float toValue,
                                                const Animation* animation,
                                                double beginTime)
{
    return adoptRef(new AndroidOpacityAnimation(fromValue, toValue,
                                                animation, beginTime));
}

AndroidOpacityAnimation::AndroidOpacityAnimation(float fromValue, float toValue,
                                                 const Animation* animation,
                                                 double beginTime)
    : AndroidAnimation(animation, beginTime)
    , m_fromValue(fromValue)
    , m_toValue(toValue)
{
}

AndroidOpacityAnimation::AndroidOpacityAnimation(AndroidOpacityAnimation* anim)
    : AndroidAnimation(anim)
    , m_fromValue(anim->m_fromValue)
    , m_toValue(anim->m_toValue)
{
}

PassRefPtr<AndroidAnimation> AndroidOpacityAnimation::copy()
{
    return adoptRef(new AndroidOpacityAnimation(this));
}

bool AndroidOpacityAnimation::evaluate(LayerAndroid* layer, double time)
{
    float progress;
    if (!checkIterationsAndProgress(time, &progress))
        return false;

    if (progress < 0) // we still want to be evaluated until we get progress > 0
        return true;

    float value = m_fromValue + ((m_toValue - m_fromValue) * progress);
    layer->setOpacity(value);
    return true;
}

PassRefPtr<AndroidTransformAnimation> AndroidTransformAnimation::create(
                                                     const Animation* animation,
                                                     KeyframeValueList* operations,
                                                     double beginTime)
{
    return adoptRef(new AndroidTransformAnimation(animation, operations, beginTime));
}

AndroidTransformAnimation::AndroidTransformAnimation(const Animation* animation,
                                                     KeyframeValueList* operations,
                                                     double beginTime)
    : AndroidAnimation(animation, beginTime)
    , m_operations(operations)
{
}

AndroidTransformAnimation::AndroidTransformAnimation(AndroidTransformAnimation* anim)
    : AndroidAnimation(anim)
    , m_operations(anim->m_operations)
{
}

PassRefPtr<AndroidAnimation> AndroidTransformAnimation::copy()
{
    return adoptRef(new AndroidTransformAnimation(this));
}

bool AndroidTransformAnimation::evaluate(LayerAndroid* layer, double time)
{
    float progress;
    if (!checkIterationsAndProgress(time, &progress))
        return false;

    if (progress < 0) // we still want to be evaluated until we get progress > 0
        return true;

    IntSize size(layer->getSize().width(), layer->getSize().height());
    TransformationMatrix matrix;
    XLOG("Evaluate transforms animations, %d operations, progress %.2f for layer %d (%d, %d)"
         , m_operations->size(), progress, layer->uniqueId(), size.width(), size.height());

    if (!m_operations->size())
        return true;

    // First, we need to get the from and to values

    TransformAnimationValue* fromValue = 0;
    TransformAnimationValue* toValue = 0;

    float distance = 0;
    unsigned int foundAt = 0;
    for (unsigned int i = 0; i < m_operations->size(); i++) {
        TransformAnimationValue* value = (TransformAnimationValue*) m_operations->at(i);
        TransformOperations* values = (TransformOperations*) value->value();
        float key = value->keyTime();
        float d = fabs(progress - key);
        XLOG("[%d] Key %.2f, %d values", i, key, values->size());
        if (!fromValue || (d < distance && i + 1 < m_operations->size())) {
            fromValue = value;
            distance = d;
            foundAt = i;
        }
    }

    if (foundAt + 1 < m_operations->size())
        toValue = (TransformAnimationValue*) m_operations->at(foundAt + 1);
    else
        toValue = fromValue;

    XLOG("[layer %d] fromValue %x, key %.2f, toValue %x, key %.2f for progress %.2f",
         layer->uniqueId(),
         fromValue, fromValue->keyTime(),
         toValue, toValue->keyTime(), progress);

    // We now have the correct two values to work with, let's compute the
    // progress value

    float delta = toValue->keyTime() - fromValue->keyTime();
    float rprogress = (progress - fromValue->keyTime()) / delta;
    XLOG("We picked keys %.2f to %.2f for progress %.2f, real progress %.2f",
         fromValue->keyTime(), toValue->keyTime(), progress, rprogress);
    progress = rprogress;


    // With both values and the progress, we also need to check out that
    // the operations are compatible (i.e. we are animating the same number
    // of values; if not we do a matrix blend)

    TransformationMatrix transformMatrix;
    bool valid = true;
    unsigned int fromSize = fromValue->value()->size();
    if (fromSize) {
        if (toValue->value()->size() != fromSize)
            valid = false;
        else {
            for (unsigned int j = 0; j < fromSize && valid; j++) {
                if (!fromValue->value()->operations()[j]->isSameType(
                    *toValue->value()->operations()[j]))
                    valid = false;
            }
        }
    }

    if (valid) {
        for (size_t i = 0; i < toValue->value()->size(); ++i)
            toValue->value()->operations()[i]->blend(fromValue->value()->at(i),
                                                     progress)->apply(transformMatrix, size);
    } else {
        TransformationMatrix source;

        fromValue->value()->apply(size, source);
        toValue->value()->apply(size, transformMatrix);

        transformMatrix.blend(source, progress);
    }

    // Set the final transform on the layer
    layer->setTransform(transformMatrix);

    return true;
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
