/*
-----------------------------------------------------------------------------
This source file is part of OGRE-Next
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef _OgreCompositorPassMipmap_H_
#define _OgreCompositorPassMipmap_H_

#include "Compositor/Pass/OgreCompositorPass.h"
#include "Compositor/Pass/PassMipmap/OgreCompositorPassMipmapDef.h"

#include "OgreHeaderPrefix.h"

namespace Ogre
{
    /** \addtogroup Core
     *  @{
     */
    /** \addtogroup Effects
     *  @{
     */

    /** Implementation of CompositorPass
        This implementation will generate mipmaps for the specified texture
    @author
        Matias N. Goldberg
    @version
        1.0
    */
    class _OgreExport CompositorPassMipmap : public CompositorPass
    {
    protected:
        TextureGpuVec mTextures;

        /// Compute
        TextureGpuVec               mTmpTextures;
        FastArray<HlmsComputeJob *> mJobs;

        bool mWarnedNoAutomipmapsAlready;

        void setupComputeShaders();
        void destroyComputeShaders();
        void setGaussianFilterParams( HlmsComputeJob *job, uint8 kernelRadius,
                                      float gaussianDeviationFactor );

        void analyzeBarriers( const bool bClearBarriers = true ) override;

    public:
        CompositorPassMipmap( const CompositorPassMipmapDef *definition,  //
                              const RenderTargetViewDef *rtv, CompositorNode *parentNode );
        ~CompositorPassMipmap() override;

        void execute( const Camera *lodCamera ) override;

        bool notifyRecreated( const TextureGpu *channel ) override;

    private:
        CompositorPassMipmapDef const *mDefinition;
    };

    /** @} */
    /** @} */
}  // namespace Ogre

#include "OgreHeaderSuffix.h"

#endif
