/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef CanvasLayerChromium_h
#define CanvasLayerChromium_h

#if USE(ACCELERATED_COMPOSITING)

#include "LayerChromium.h"

namespace WebCore {

class GraphicsContext3D;

// A Layer containing a WebGL or accelerated 2d canvas
class CanvasLayerChromium : public LayerChromium {
public:
    static PassRefPtr<CanvasLayerChromium> create(GraphicsLayerChromium* owner = 0);
    virtual bool drawsContent() { return m_context; }
    virtual void updateContents();
    virtual void draw();

    void setContext(const GraphicsContext3D* context);

    class SharedValues {
    public:
        SharedValues();
        ~SharedValues();

        unsigned canvasShaderProgram() const { return m_canvasShaderProgram; }
        int shaderSamplerLocation() const { return m_shaderSamplerLocation; }
        int shaderMatrixLocation() const { return m_shaderMatrixLocation; }
        int shaderAlphaLocation() const { return m_shaderAlphaLocation; }
        bool initialized() const { return m_initialized; }

    private:
        unsigned m_canvasShaderProgram;
        int m_shaderSamplerLocation;
        int m_shaderMatrixLocation;
        int m_shaderAlphaLocation;
        bool m_initialized;
    };

    class PrepareTextureCallback : public Noncopyable {
    public:
        virtual void willPrepareTexture() = 0;
    };
    void setPrepareTextureCallback(PassOwnPtr<PrepareTextureCallback> callback) { m_prepareTextureCallback = callback; }

private:
    explicit CanvasLayerChromium(GraphicsLayerChromium* owner);
    GraphicsContext3D* m_context;
    unsigned m_textureId;
    bool m_textureChanged;
    OwnPtr<PrepareTextureCallback> m_prepareTextureCallback;
};

}
#endif // USE(ACCELERATED_COMPOSITING)

#endif
