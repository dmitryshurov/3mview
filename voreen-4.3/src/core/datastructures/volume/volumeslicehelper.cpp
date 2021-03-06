/***********************************************************************************
 *                                                                                 *
 * Voreen - The Volume Rendering Engine                                            *
 *                                                                                 *
 * Copyright (C) 2005-2013 University of Muenster, Germany.                        *
 * Visualization and Computer Graphics Group <http://viscg.uni-muenster.de>        *
 * For a list of authors please refer to the file "CREDITS.txt".                   *
 *                                                                                 *
 * This file is part of the Voreen software package. Voreen is free software:      *
 * you can redistribute it and/or modify it under the terms of the GNU General     *
 * Public License version 2 as published by the Free Software Foundation.          *
 *                                                                                 *
 * Voreen is distributed in the hope that it will be useful, but WITHOUT ANY       *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR   *
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.      *
 *                                                                                 *
 * You should have received a copy of the GNU General Public License in the file   *
 * "LICENSE.txt" along with this file. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                                 *
 * For non-commercial academic use see the license exception specified in the file *
 * "LICENSE-academic.txt". To get information about commercial licensing please    *
 * contact the authors.                                                            *
 *                                                                                 *
 ***********************************************************************************/

#include "voreen/core/datastructures/volume/volumeslicehelper.h"
#include "voreen/core/datastructures/roi/roibase.h"
#include "voreen/core/datastructures/geometry/trianglemeshgeometry.h"
#include "tgt/logmanager.h"

#include "voreen/core/datastructures/volume/volumeram.h"
#include "voreen/core/datastructures/volume/volumedisk.h"
#ifdef VRN_MODULE_STAGING
#include "modules/staging/octree/datastructures/volumeoctreebase.h"
#endif

namespace voreen {

using tgt::vec2;
using tgt::ivec2;
using tgt::vec3;
using tgt::mat4;

TriangleMeshGeometryVec3* getSliceGeometry(const VolumeBase* vh, SliceAlignment alignment, float sliceIndex, bool applyTransformation, const std::vector<const VolumeBase*> secondaryVolumes) {
    TriangleMeshGeometryVec3* slice = new TriangleMeshGeometryVec3();
    vec3 urb = vh->getURB();
    vec3 llf = vh->getLLF();
    vec3 sp = vh->getSpacing();
    tgt::Bounds b(llf, urb);

    mat4 wToP = vh->getWorldToPhysicalMatrix();
    for(size_t i=0; i<secondaryVolumes.size(); i++) {
        tgt::Bounds sb(secondaryVolumes[i]->getLLF(), secondaryVolumes[i]->getURB());
        tgt::Bounds sbTf = sb.transform(wToP * secondaryVolumes[i]->getPhysicalToWorldMatrix());

        b.addPoint(sbTf.getLLF());
        b.addPoint(sbTf.getURB());
    }

    vec3 bb_urb = b.getURB();
    vec3 bb_llf = b.getLLF();

    switch(alignment) {
        case YZ_PLANE: {
                           float x = sliceIndex;
                           float xcoord = llf.x + (x+0.5f) * sp.x; // We want our slice to be in the center of voxels

                           slice->addQuad(
                           VertexVec3(tgt::vec3(xcoord, bb_urb.y, bb_urb.z), tgt::vec3(xcoord, bb_urb.y, bb_urb.z)),
                           VertexVec3(tgt::vec3(xcoord, bb_urb.y, bb_llf.z), tgt::vec3(xcoord, bb_urb.y, bb_llf.z)),
                           VertexVec3(tgt::vec3(xcoord, bb_llf.y, bb_llf.z), tgt::vec3(xcoord, bb_llf.y, bb_llf.z)),
                           VertexVec3(tgt::vec3(xcoord, bb_llf.y, bb_urb.z), tgt::vec3(xcoord, bb_llf.y, bb_urb.z)));
                       }
                       break;
        case XZ_PLANE: {
                           float y = sliceIndex;
                           float ycoord = llf.y + (y+0.5f) * sp.y; // We want our slice to be in the center of voxels

                           slice->addQuad(
                           VertexVec3(tgt::vec3(bb_urb.x, ycoord, bb_urb.z), tgt::vec3(bb_urb.x, ycoord, bb_urb.z)),
                           VertexVec3(tgt::vec3(bb_urb.x, ycoord, bb_llf.z), tgt::vec3(bb_urb.x, ycoord, bb_llf.z)),
                           VertexVec3(tgt::vec3(bb_llf.x, ycoord, bb_llf.z), tgt::vec3(bb_llf.x, ycoord, bb_llf.z)),
                           VertexVec3(tgt::vec3(bb_llf.x, ycoord, bb_urb.z), tgt::vec3(bb_llf.x, ycoord, bb_urb.z)));
                       }
                       break;
        case XY_PLANE: {
                           float z = sliceIndex;
                           float zcoord = llf.z + (z+0.5f) * sp.z; // We want our slice to be in the center of voxels

                           slice->addQuad(
                           VertexVec3(tgt::vec3(bb_urb.x, bb_urb.y, zcoord), tgt::vec3(bb_urb.x, bb_urb.y, zcoord)),
                           VertexVec3(tgt::vec3(bb_urb.x, bb_llf.y, zcoord), tgt::vec3(bb_urb.x, bb_llf.y, zcoord)),
                           VertexVec3(tgt::vec3(bb_llf.x, bb_llf.y, zcoord), tgt::vec3(bb_llf.x, bb_llf.y, zcoord)),
                           VertexVec3(tgt::vec3(bb_llf.x, bb_urb.y, zcoord), tgt::vec3(bb_llf.x, bb_urb.y, zcoord)));
                       }
                       break;
        default: tgtAssert(false, "should not get here!");
    }
    tgt::mat4 m = vh->getPhysicalToTextureMatrix();

    // set coords to texture coordinates:
    for(size_t j=0; j<slice->getNumTriangles(); ++j) {
        Triangle<VertexVec3> t = slice->getTriangle(j);
        t.v_[0].pos_ = m * t.v_[0].pos_;
        t.v_[1].pos_ = m * t.v_[1].pos_;
        t.v_[2].pos_ = m * t.v_[2].pos_;
        t.v_[0].attr1_ = t.v_[0].pos_;
        t.v_[1].attr1_ = t.v_[1].pos_;
        t.v_[2].attr1_ = t.v_[2].pos_;
        slice->setTriangle(t, j);
    }

    slice->setTransformationMatrix(vh->getTextureToWorldMatrix());
    return slice;
}

Slice::Slice(tgt::vec3 origin, tgt::vec3 xVec, tgt::vec3 yVec, tgt::Texture* tex, RealWorldMapping rwm) : origin_(origin), xVec_(xVec), yVec_(yVec), tex_(tex), rwm_(rwm)
{
}

Slice::~Slice()
{
    delete tex_;
}

tgt::mat4 Slice::getTextureToWorldMatrix() const {
    vec3 zVec = normalize(cross(xVec_, yVec_));

    mat4 m(xVec_.x, yVec_.x, zVec.x, origin_.x,
           xVec_.y, yVec_.y, zVec.y, origin_.y,
           xVec_.z, yVec_.z, zVec.z, origin_.z,
           0.0f,    0.0f,    0.0f,   1.0f);
    return m;
}

tgt::mat4 Slice::getWorldToTextureMatrix() const {
    mat4 m = getTextureToWorldMatrix();
    mat4 inv;
    m.invert(inv);
    return inv;
}

//-------------------------------------------------------------------------------------------------

Slice* getVolumeSlice(const VolumeBase* vh, SliceAlignment alignment, int sliceIndex, int channel /*= 0*/) {
    vec3 urb = vh->getURB();
    vec3 llf = vh->getLLF();
    vec3 sp = vh->getSpacing();
    tgt::Bounds b(llf, urb);
    tgt::svec3 dims = vh->getDimensions();

    vec3 bb_urb = b.getURB();
    vec3 bb_llf = b.getLLF();

    vec3 origin(0.0f);
    vec3 xVec(0.0f);
    vec3 yVec(0.0f);
    tgt::Texture* tex = 0;

    switch(alignment) {
        case YZ_PLANE: {
            // representation preference:
            // 1) use RAM volume, if present
            // 2) else: use octree, if present
            // 3) else: try to create RAM representation
            // note: disk representation is not usable for this alignment
            const VolumeRAM* ramVolume3D = 0;     //< RAM representation
            const VolumeRAM* ramVolumeSlice = 0;  //< slice retrieved from octree (has to be deleted afterwards)
            if (vh->hasRepresentation<VolumeRAM>()) {
                ramVolume3D = vh->getRepresentation<VolumeRAM>();
            }
#ifdef VRN_MODULE_STAGING
            else if (vh->hasRepresentation<VolumeOctreeBase>()) {
                try {
                    const VolumeOctreeBase* octree = vh->getRepresentation<VolumeOctreeBase>();
                    tgtAssert(octree, "no octree");
                    ramVolumeSlice = octree->createSlice(YZ_PLANE, sliceIndex, 0, channel);
                    tgtAssert(ramVolumeSlice, "null pointer returned (exception expected)");
                }
                catch (tgt::Exception& e) {
                    LERRORC("voreen.VolumeSliceHelper", "Failed to create YZ slice from octree: " << e.what());
                    return 0;
                }
            }
#endif
            else {
                ramVolume3D = vh->getRepresentation<VolumeRAM>();
                if (!ramVolume3D) {
                    LERRORC("voreen.VolumeSliceHelper",
                        "Failed to create YZ slice: neither RAM nor octree representation available");
                    return 0;
                }
            }

            float x = static_cast<float>(sliceIndex);
            float xcoord = llf.x + (x+0.5f) * sp.x; // We want our slice to be in the center of voxels

            origin = vec3(xcoord, bb_llf.y, bb_llf.z);
            xVec = vec3(xcoord, bb_urb.y, bb_llf.z);
            yVec = vec3(xcoord, bb_llf.y, bb_urb.z);

            tex = new tgt::Texture(tgt::ivec3(dims.yz(), 1), GL_ALPHA, GL_ALPHA32F_ARB, GL_FLOAT, tgt::Texture::LINEAR); //TODO: make dependent on input, add support for multiple channels

            tgtAssert(ramVolume3D || ramVolumeSlice, "no volume");
            for (int y=0; y<static_cast<int>(dims.y); y++) {
                for(int z=0; z<static_cast<int>(dims.z); z++) {
                    //float value = volumeRam->getVoxelNormalized(sliceIndex, y, z);
                    //tex->texel< float >(y, z) = value;
                    float value = ramVolume3D ?
                        ramVolume3D->getVoxelNormalized(sliceIndex, y, z, channel) :
                        ramVolumeSlice->getVoxelNormalized(0, y, z);
                    tex->texel< float >(y, z) = value;
                }
            }

            delete ramVolumeSlice;
            ramVolumeSlice = 0;

            tex->uploadTexture();
        }
        break;

        case XZ_PLANE: {
            // representation preference:
            // 1) use RAM volume, if present
            // 2) else: use octree, if present
            // 3) else: try to create RAM representation
            // note: disk representation is not usable for this alignment
            const VolumeRAM* ramVolume3D = 0;     //< RAM representation
            const VolumeRAM* ramVolumeSlice = 0;  //< slice retrieved from octree (has to be deleted afterwards)
            if (vh->hasRepresentation<VolumeRAM>()) {
                ramVolume3D = vh->getRepresentation<VolumeRAM>();
            }
#ifdef VRN_MODULE_STAGING
            else if (vh->hasRepresentation<VolumeOctreeBase>()) {
                try {
                    const VolumeOctreeBase* octree = vh->getRepresentation<VolumeOctreeBase>();
                    tgtAssert(octree, "no octree");
                    ramVolumeSlice = octree->createSlice(XZ_PLANE, sliceIndex, 0, channel);
                    tgtAssert(ramVolumeSlice, "null pointer returned (exception expected)");
                }
                catch (tgt::Exception& e) {
                    LERRORC("voreen.VolumeSliceHelper", "Failed to create XZ slice from octree: " << e.what());
                    return 0;
                }
            }
#endif
            else {
                ramVolume3D = vh->getRepresentation<VolumeRAM>();
                if (!ramVolume3D) {
                    LERRORC("voreen.VolumeSliceHelper",
                        "Failed to create XZ slice: neither RAM nor octree representation available");
                    return 0;
                }
            }

            float y = static_cast<float>(sliceIndex);
            float ycoord = llf.y + (y+0.5f) * sp.y; // We want our slice to be in the center of voxels

            origin = vec3(bb_llf.x, ycoord, bb_llf.z);
            xVec = vec3(bb_urb.x, ycoord, bb_llf.z);
            yVec = vec3(bb_llf.x, ycoord, bb_urb.z);

            tgtAssert(ramVolume3D || ramVolumeSlice, "no volume");
            tex = new tgt::Texture(tgt::ivec3(static_cast<int>(dims.x), static_cast<int>(dims.z), 1), GL_ALPHA, GL_ALPHA32F_ARB, GL_FLOAT, tgt::Texture::LINEAR); //TODO: make dependent on input, add support for multiple channels
            for(int x=0; x<static_cast<int>(dims.x); x++) {
                for(int z=0; z<static_cast<int>(dims.z); z++) {
                    /*float value = volumeRam->getVoxelNormalized(x, sliceIndex, z);
                    tex->texel< float >(x, z) = value; */
                    float value = ramVolume3D ?
                        ramVolume3D->getVoxelNormalized(x, sliceIndex, z, channel) :
                        ramVolumeSlice->getVoxelNormalized(x, 0, z);
                    tex->texel< float >(x, z) = value;
                }
            }

            delete ramVolumeSlice;
            ramVolumeSlice = 0;

            tex->uploadTexture();
        }
        break;

        case XY_PLANE: {
            // representation preference:
            // 1) use RAM volume, if present
            // 2) else: use octree, if present
            // 3) else: use disk volume, if present
            const VolumeRAM* ramVolume3D = 0;     //< RAM representation
            const VolumeRAM* ramVolumeSlice = 0;  //< slice retrieved from octree or disk volume (has to be deleted)
            if (vh->hasRepresentation<VolumeRAM>()) {
                ramVolume3D = vh->getRepresentation<VolumeRAM>();
            }
#ifdef VRN_MODULE_STAGING
            else if (vh->hasRepresentation<VolumeOctreeBase>()) {
                try {
                    const VolumeOctreeBase* octree = vh->getRepresentation<VolumeOctreeBase>();
                    tgtAssert(octree, "no octree");
                    ramVolumeSlice = octree->createSlice(XY_PLANE, sliceIndex, 0, channel);
                    tgtAssert(ramVolumeSlice, "null pointer returned (exception expected)");
                }
                catch (tgt::Exception& e) {
                    LERRORC("voreen.VolumeSliceHelper", "Failed to create XY slice from octeee: " << e.what());
                    return 0;
                }
            }
#endif
            else {
                ramVolume3D = vh->getRepresentation<VolumeRAM>();
                if (!ramVolume3D) {
                    LERRORC("voreen.VolumeSliceHelper",
                        "Failed to create XY slice: neither RAM nor octree representation available");
                    return 0;
                }
            }
            /*else if (vh->hasRepresentation<VolumeDisk>()) {
                try {
                    ramVolumeSlice = vh->getRepresentation<VolumeDisk>()->loadSlices(sliceIndex, sliceIndex);
                    tgtAssert(ramVolumeSlice, "null pointer returned (exception expected)");
                }
                catch (tgt::Exception& e) {
                    LERRORC("voreen.VolumeSliceHelper", "Failed to load XY slice from disk volume: " << e.what());
                    return 0;
                }
            }
            else {
                LERRORC("voreen.VolumeSliceHelper",
                    "Failed to create XY slice: neither RAM, nor octree, nor disk volume representation available");
                return 0;
            } */

            float z = static_cast<float>(sliceIndex);
            float zcoord = llf.z + (z+0.5f) * sp.z; // We want our slice to be in the center of voxels

            origin = vec3(bb_llf.x, bb_llf.y, zcoord);
            xVec = vec3(bb_urb.x, bb_llf.y, zcoord);
            yVec = vec3(bb_llf.x, bb_urb.y, zcoord);

            tgtAssert(ramVolume3D || ramVolumeSlice, "no representation");
            tex = new tgt::Texture(tgt::ivec3(dims.xy(), 1), GL_ALPHA, GL_ALPHA32F_ARB, GL_FLOAT, tgt::Texture::LINEAR); //TODO: make dependent on input, add support for multiple channels
            for(int y=0; y<static_cast<int>(dims.y); y++) {
                for(int x=0; x<static_cast<int>(dims.x); x++) {
                    float value = ramVolume3D ?
                        ramVolume3D->getVoxelNormalized(x, y, sliceIndex, channel) :
                        ramVolumeSlice->getVoxelNormalized(x, y, 0);
                    tex->texel< float >(x, y) = value;
                }
            }

            delete ramVolumeSlice;
            ramVolumeSlice = 0;

            tex->uploadTexture();
        }
        break;

        default: tgtAssert(false, "should not get here!");
    }

    origin = vh->getPhysicalToWorldMatrix() * origin;
    xVec = vh->getPhysicalToWorldMatrix() * xVec;
    yVec = vh->getPhysicalToWorldMatrix() * yVec;
    xVec = xVec - origin;
    yVec = yVec - origin;

    return new Slice(origin, xVec, yVec, tex, vh->getRealWorldMapping());
}

//-------------------------------------------------------------------------------------------------

Slice* getVolumeSlice(const VolumeBase* vh, tgt::plane pl, float samplingRate) {
    vec3 urb = vh->getURB();
    vec3 llf = vh->getLLF();
    vec3 center = (urb + llf) * 0.5f;

    vec3 xMax = center;
    xMax.x = urb.x;
    vec3 yMax = center;
    yMax.y = urb.y;
    vec3 zMax = center;
    zMax.z = urb.z;

    // check whether the plane normal matches one of the main directions of the volume:
    tgt::plane plVoxel = pl.transform(vh->getWorldToVoxelMatrix());
    if(fabs(fabs(dot(vec3(1.0f, 0.0f, 0.0f), plVoxel.n)) - 1.0f) < 0.01f) {
        float sliceNumber = -plVoxel.d * plVoxel.n.x;
        sliceNumber -= 0.5f;

        float integral = tgt::round(sliceNumber);
        if(fabs(sliceNumber - integral) < 0.1f)
            return getVolumeSlice(vh, YZ_PLANE, static_cast<int>(sliceNumber));
        //else TODO
    }
    else if(fabs(fabs(dot(vec3(0.0f, 1.0f, 0.0f), plVoxel.n)) - 1.0f) < 0.01f) {
        float sliceNumber = -plVoxel.d * plVoxel.n.y;
        sliceNumber -= 0.5f;

        float integral = tgt::round(sliceNumber);
        if(fabs(sliceNumber - integral) < 0.1f)
            return getVolumeSlice(vh, XZ_PLANE, static_cast<int>(sliceNumber));
        //else TODO
    }
    else if(fabs(fabs(dot(vec3(0.0f, 0.0f, 1.0f), plVoxel.n)) - 1.0f) < 0.01f) {
        float sliceNumber = -plVoxel.d * plVoxel.n.z;
        sliceNumber -= 0.5f;

        float integral = tgt::round(sliceNumber);
        if(fabs(sliceNumber - integral) < 0.1f)
            return getVolumeSlice(vh, XY_PLANE, static_cast<int>(sliceNumber));
        //else TODO
    }

    const VolumeRAM* vol = vh->getRepresentation<VolumeRAM>();
    if(!vol)
        return 0;

    // transform to world coordinates:
    mat4 pToW = vh->getPhysicalToWorldMatrix();
    center = pToW * center;
    xMax = pToW * xMax;
    yMax = pToW * yMax;
    zMax = pToW * zMax;

    // project to plane:
    float d = pl.distance(center);
    center = center - (pl.n * d);
    d = pl.distance(xMax);
    xMax = xMax - (pl.n * d);
    d = pl.distance(yMax);
    yMax = yMax - (pl.n * d);
    d = pl.distance(zMax);
    zMax = zMax - (pl.n * d);

    // find max axis in plane:
    vec3 maxVec = xMax - center;
    if(distance(yMax, center) > length(maxVec))
        maxVec = yMax - center;
    if(distance(zMax, center) > length(maxVec))
        maxVec = zMax - center;

    maxVec = normalize(maxVec);
    vec3 temp = normalize(cross(maxVec, pl.n));

    // construct transformation to temporary system:
    mat4 m(maxVec.x, temp.x, pl.n.x, center.x,
           maxVec.y, temp.y, pl.n.y, center.y,
           maxVec.z, temp.z, pl.n.z, center.z,
           0.0f,     0.0f,   0.0f,   1.0f);
    tgt::mat4 mInv = tgt::mat4::identity;
    m.invert(mInv);

    // transform bounds to temp system in order to construct new coordinate frame
    tgt::Bounds b(vh->getLLF(), vh->getURB());
    b = b.transform(mInv*pToW);

    // construct new coordinate frame:
    vec3 origin = center;
    origin += b.getLLF().x * maxVec;
    origin += b.getLLF().y * temp;

    vec2 sp(tgt::min(vh->getSpacing()) / samplingRate);
    ivec2 res(tgt::iceil(b.diagonal().x / sp.x), tgt::iceil(b.diagonal().y / sp.y));

    vec3 xVec = maxVec * (sp.x * res.x);
    vec3 yVec = temp * (sp.y * res.y);

    LGL_ERROR;
    tgt::Texture* tex = new tgt::Texture(tgt::ivec3(res, 1), GL_ALPHA, GL_ALPHA32F_ARB, GL_FLOAT, tgt::Texture::LINEAR); //TODO: make dependent on input, add support for multiple channels
    LGL_ERROR;
    vec3 fetchX = normalize(xVec) * sp.x;
    vec3 fetchY = normalize(yVec) * sp.y;
    vec3 fetchOrigin = origin + (0.5f * fetchX) + (0.5f * fetchY);
    mat4 wToV = vh->getWorldToVoxelMatrix();
    vec3 dims = vh->getDimensions();
    for(int x=0; x<res.x; x++) {
        for(int y=0; y<res.y; y++) {
            vec3 pos = fetchOrigin + ((float)x * fetchX) + ((float)y * fetchY);
            pos = wToV * pos;
            float valueFloat = 0.0f;
            if(hand(greaterThanEqual(pos, vec3(0.0f))) && hand(lessThanEqual(pos, dims)))
                valueFloat = vol->getVoxelNormalizedLinear(pos);
            //uint8_t value = tgt::iround(valueFloat * 255.0f);

            tex->texel<float>(x, y) = valueFloat;
            //tex->texel<tgt::col4>(x, y) = tgt::col4(value);
            //tex->texel<tgt::col4>(x, y) = tgt::col4(128);
            //tex->texel<tgt::col4>(x, y) = tgt::col4(x);
        }
    }
    LGL_ERROR;
    tex->uploadTexture();
    LGL_ERROR;
    return new Slice(origin, xVec, yVec, tex, vh->getRealWorldMapping());
}

} // namespace voreen
