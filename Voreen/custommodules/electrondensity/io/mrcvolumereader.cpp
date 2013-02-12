/***********************************************************************************
 *                                                                                 *
 * Voreen - The Volume Rendering Engine                                            *
 *                                                                                 *
 * Copyright (C) 2005-2012 University of Muenster, Germany.                        *
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

#include "mrcvolumereader.h"

#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/datastructures/volume/volumedecorator.h"
#include "voreen/core/io/textfilereader.h"
#include "voreen/core/io/progressbar.h"
#include "voreen/core/utils/stringutils.h"

#include <fstream>
#include <iostream>
#include <assert.h>

#include "tgt/exception.h"
#include "tgt/vector.h"
#include "tgt/texturemanager.h"

using std::string;
using tgt::vec3;
using tgt::ivec3;
using tgt::Texture;

namespace voreen {

const std::string MRCVolumeReader::loggerCat_ = "voreen.electrondensity.MRCVolumeReader";

MRCVolumeReader::MRCVolumeReader(ProgressBar* progress) : VolumeReader(progress)
{
    extensions_.push_back("mrc");
    extensions_.push_back("ccp4");
}

VolumeCollection* MRCVolumeReader::read(const std::string &url)
    throw (tgt::FileException, tgt::IOException, std::bad_alloc)
{
    VolumeCollection* volumeCollection = new VolumeCollection();

    VolumeURL origin(url);
    std::string fileName = origin.getPath();
    LINFO(fileName);

    
    
    std::ifstream mrc;
    mrc.open(fileName.c_str(), std::ios::binary);
    if (!mrc.is_open()) {
        LWARNING("Can't open stream");
    }
    else {
        int dimensions[3];
        mrc.read((char*)(&dimensions), sizeof(dimensions));
        std::cout << "X: " << dimensions[0] << std::endl; // number of columns (fastest changing in map)
        std::cout << "Y: " << dimensions[1] << std::endl; // number of rows
        std::cout << "Z: " << dimensions[2] << std::endl; // number of sections (slowest changing in map)
        
        int numVoxels = dimensions[0] * dimensions[1] * dimensions[2];
        std::cout << "numVoxels: " << numVoxels << std::endl;
        
        int dataType;
        mrc.read((char*)(&dataType), sizeof(dataType));
        std::cout << "dataType: " << dataType << std::endl;
        
        int dataSize = 0;
             if (dataType == 0) dataSize = 1; // signed 8-bit bytes range -128 to 127
        else if (dataType == 1) dataSize = 2; // 16-bit halfwords
        else if (dataType == 2) dataSize = 4; // 32-bit reals
        else if (dataType == 6) dataSize = 2; // unsigned 16-bit range 0 to 65535
        
        tgtAssert(dataSize, "Datasize is 0 at MRCVolumeReader::read()");
        
        int totalDataSize = dataSize * numVoxels;

        int gridSize[3]; // cell dimensions in angstroms
        
        mrc.seekg(4*7, std::ios::beg);
        mrc.read((char*)(&gridSize), sizeof(gridSize));
        std::cout << "gridSizeX: " << gridSize[0] << std::endl; // number of columns (fastest changing in map)
        std::cout << "gridSizeY: " << gridSize[1] << std::endl; // number of rows
        std::cout << "gridSizeZ: " << gridSize[2] << std::endl; // number of sections (slowest changing in map)
     
        float cellDimensions[3]; // cell dimensions in angstroms
        
        mrc.read((char*)(&cellDimensions), sizeof(cellDimensions));
        std::cout << "cellX: " << cellDimensions[0] << std::endl; // number of columns (fastest changing in map)
        std::cout << "cellY: " << cellDimensions[1] << std::endl; // number of rows
        std::cout << "cellZ: " << cellDimensions[2] << std::endl; // number of sections (slowest changing in map)
        
        float sclX, sclY, sclZ;
        sclX = cellDimensions[0] / gridSize[0];
        sclY = cellDimensions[1] / gridSize[1];
        sclZ = cellDimensions[2] / gridSize[2];
        std::cout << "pixelSpacingX: " << sclX << std::endl; // number of columns (fastest changing in map)
        std::cout << "pixelSpacingY: " << sclY << std::endl; // number of rows
        std::cout << "pixelSpacingZ: " << sclZ << std::endl; // number of sections (slowest changing in map)
        
        float cellAngles[3]; // cell angles in degrees

        mrc.read((char*)(&cellAngles), sizeof(cellAngles));
        std::cout << "cellAngleX: " << cellAngles[0] << std::endl; // number of columns (fastest changing in map)
        std::cout << "cellAngleY: " << cellAngles[1] << std::endl; // number of rows
        std::cout << "cellAngleZ: " << cellAngles[2] << std::endl; // number of sections (slowest changing in map)
        
        int axes[3]; // cell dimensions in angstroms
        
        mrc.read((char*)(&axes), sizeof(axes));
        std::cout << "axesX: " << axes[0] << std::endl; // number of columns (fastest changing in map)
        std::cout << "axesY: " << axes[1] << std::endl; // number of rows
        std::cout << "axesZ: " << axes[2] << std::endl; // number of sections (slowest changing in map)
        
        float origin[3]; // cell angles in degrees
        mrc.seekg(4*49, std::ios::beg);
        mrc.read((char*)(&origin), sizeof(origin));
        std::cout << "originX: " << origin[0] << std::endl; // number of columns (fastest changing in map)
        std::cout << "originY: " << origin[1] << std::endl; // number of rows
        std::cout << "originZ: " << origin[2] << std::endl; // number of sections (slowest changing in map)
        
        void* data = malloc(totalDataSize);
        mrc.seekg(1024, std::ios::beg);
        mrc.read((char*)data, totalDataSize);
        mrc.close();
        
        std::vector<VolumeRAM*> targetDataset;
        targetDataset.push_back(new VolumeRAM_Float(ivec3(dimensions[0], dimensions[1], dimensions[2])));
        
        std::vector<float*> scalars32;
        
        if (dataType == 2) 
            scalars32.push_back(reinterpret_cast<float*>(((VolumeRAM_Float*)targetDataset[0])->voxel()));
            
        memcpy(scalars32[0], data, numVoxels);
        
        Volume* volumeHandle = new Volume(targetDataset[0], vec3(1.0f), vec3(0.0f));
        tgt::Matrix4<float> transform(
        sclX, 0.0f, 0.0f, 1.0f,
        0.0f, sclY, 0.0f, 1.0f,
        0.0f, 0.0f, sclZ, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
        VolumeBase* outVol = new VolumeDecoratorReplaceTransformation(volumeHandle, transform);
        volumeCollection->add(outVol);
    }
    
    if (!volumeCollection->empty())
        volumeCollection->first()->setOrigin(VolumeURL(fileName));
        
    return volumeCollection;
}

VolumeReader* MRCVolumeReader::create(ProgressBar* progress) const {
    return new MRCVolumeReader(progress);
}

} // namespace voreen
