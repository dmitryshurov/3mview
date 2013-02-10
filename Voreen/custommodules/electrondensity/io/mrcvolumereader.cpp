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
        
        void* data = malloc(totalDataSize);
        mrc.read((char*)data, totalDataSize);
        mrc.close();
        
        std::vector<VolumeRAM*> targetDataset;
        targetDataset.push_back(new VolumeRAM_Float(ivec3(dimensions[0], dimensions[1], dimensions[2])));
        
        std::vector<float*> scalars32;
        
        if (dataType == 2) 
            scalars32.push_back(reinterpret_cast<float*>(((VolumeRAM_Float*)targetDataset[0])->voxel()));
            
        memcpy(scalars32[0], data, numVoxels);
        
                Volume* volumeHandle = new Volume(targetDataset[0], vec3(1.0f), vec3(0.0f));
        
        //oldVolumePosition(volumeHandle);
        volumeCollection->add(volumeHandle);
    }

    
/*
    TIFF* tif = TIFFOpen(fileName.c_str(), "r");
    if (tif) {
        int dircount = 0;
        do {
            dircount++;
        } while (TIFFReadDirectory(tif));
        LDEBUG(dircount << " directories found");
        dimensions.z = dircount;
        TIFFClose(tif);
    }
    else {
        LERROR("Failed to open tiffstack");
        throw tgt::IOException("Failed to open TIFF stack", fileName);
    }
    if (dimensions.z == 1)
        throw tgt::CorruptedFileException("TIFF file contains only a single image, but TIFF stack expected", fileName);

    uint16 depth, bps;
    tif = TIFFOpen(fileName.c_str(), "r");

    if (!tif) {
        LERROR("Failed to open tiffstack");
        throw tgt::IOException("Failed to open TIFF stack", fileName);
    }
    else {
        //TIFFReadDirectory(tif));
        uint32 width, height;

        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &depth);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
        dimensions.x = width;
        dimensions.y = height;

        uint16 count;
        void *data;
        int slices;
        if (TIFFGetField(tif, 33471, &count, &data)) {
            std::istringstream stream(static_cast<char*>(data));
            TextFileReader reader(&stream);
            reader.setSeparators("=");
            LDEBUG(static_cast<char*>(data));
            string type;
            std::istringstream args;
            while (reader.getNextLine(type, args, false)) {
                LDEBUG(type << ": " << args.str());
                if (type == "Band") {
                    args >> band;
                    LINFO("Band: " << band);
                }
                else if (type == "Z") {
                    args >> slices;
                    LINFO("Slices: " << slices);
                    dimensions.z = slices;
                }
                else {
                    // Parse lines of type <type> <value> and log results
                    // Later on these data should be filled into the metadata structure of a volume
                    int value;
                    int pos = static_cast<int>(type.size()) - 1;
                    while (isdigit(type[pos]) && pos > 0)
                        --pos;
                    type = type.substr(0, pos+1);
                    std::stringstream valueStr(type.substr(pos+1, type.size()-1));
                    valueStr >> value;
                    LDEBUG("Type: " << type << " with value: " << value);
                }
            }
        }
        else
            LERROR("Error");
        TIFFClose(tif);
        LINFO("depth: " << depth << " bps: " << bps);
    }

    LINFO("stacking " << dimensions.z*band << " images with dimensions (" << dimensions.x
          << ", " << dimensions.y << ") into " << band << " datasets.");
    std::vector<VolumeRAM*> targetDataset;
    std::vector<uint8_t*> scalars8;
    std::vector<uint16_t*> scalars16;
    bool use8BitDataset;
    if (bps == 8) {
        use8BitDataset = true;
    }

    else {
        if (bps != 16)
            LWARNING("Couldn't recognize bit depth. Trying 16 bit...");
        use8BitDataset = false;
    }

    if (use8BitDataset) {
        for (int i=0; i<band; ++i) {
            targetDataset.push_back(new VolumeRAM_UInt8(dimensions));
            scalars8.push_back(reinterpret_cast<uint8_t*>(((VolumeRAM_UInt8*)targetDataset[i])->voxel()));
        }
    }
    else {
        for (int i=0; i<band; ++i) {
            targetDataset.push_back(new VolumeRAM_UInt16(dimensions));
            scalars16.push_back(reinterpret_cast<uint16_t*>(((VolumeRAM_UInt16*)targetDataset[i])->voxel()));
        }
    }

    tif = TIFFOpen(fileName.c_str(), "r");
    if (!tif) {
        LERROR("Failed to open TIFF stack");
        throw tgt::IOException("Failed to open TIFF stack", fileName);
    }
    else {
        int* minValue = new int[band];
        int* maxValue = new int[band];
        for (int i=0; i<band; ++i) {
            minValue[i] = 65536;
            maxValue[i] = 0;
        }
        //(TIFFReadDirectory(tif));
        uint32 width, height;
        uint16 depth_, bps_;

        for (int i=0; i < dimensions.z*band; i++) {
            TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
            TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &depth_);
            TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps_);

            // if size or type of current image do not match skip the image..
            if ((dimensions.x != static_cast<int>(width)) || (dimensions.y != static_cast<int>(height)) || (bps != bps_) || (depth_ != depth)) {
                LWARNING("Images dimensions of " << i << ". image do not match!");
            }
            else {
                int currentBand = i % band;
                // Read in the possibly multiple strips
                int stripMax, stripCount;
                tsize_t stripSize;
                unsigned long imageOffset, result;
                unsigned long bufferSize;
                stripSize = TIFFStripSize (tif);
                stripMax = TIFFNumberOfStrips (tif);

                imageOffset = 0;

                bufferSize = stripMax * stripSize;
                uint8_t* buffer = new uint8_t[bufferSize];

                for (stripCount = 0; stripCount < stripMax; stripCount++) {
                    if (static_cast<int>(result = TIFFReadEncodedStrip (tif, stripCount,
                                    buffer + imageOffset,
                                    stripSize)) == -1){
                        LERROR("Read error on input strip number " << stripCount);
                        delete[] buffer;
                        delete[] minValue;
                        delete[] maxValue;
                        for (int i=0; i<band; ++i)
                            delete targetDataset[i];
                        TIFFClose(tif);
                        throw tgt::CorruptedFileException("Read error on input strip number " + itos(stripCount));
                    }
                    imageOffset += result;
                }
                for (int j=0; j<dimensions.x*dimensions.y; ++j) {
                    int value;
                    if (use8BitDataset)
                        value = buffer[j];
                    else
                        value = buffer[j*2] + 256*buffer[j*2+1];
                    if (minValue[currentBand] > value)
                        minValue[currentBand] = value;
                    if (maxValue[currentBand] < value)
                        maxValue[currentBand] = value;
                }
                if (use8BitDataset) {
                    memcpy(scalars8[currentBand], buffer, dimensions.x * dimensions.y);
                    scalars8[currentBand] += dimensions.x * dimensions.y;
                }
                else {
                    memcpy(scalars16[currentBand], buffer, dimensions.x * dimensions.y * 2);
                    scalars16[currentBand] += dimensions.x * dimensions.y;
                }
                delete[] buffer;
            }
            TIFFReadDirectory(tif);

            if (getProgressBar())
                getProgressBar()->setProgress(static_cast<float>(i) / static_cast<float>(dimensions.z*band));
        }

        TIFFClose(tif);
        for (int i=0; i<band; ++i) {
            LINFO("Band " << i << ": min/max value: " << minValue[i] << "/" << maxValue[i]);
        }
        delete[] minValue;
        delete[] maxValue;
    }
*/
    
    

    
    if (!volumeCollection->empty())
        volumeCollection->first()->setOrigin(VolumeURL(fileName));
        
    return volumeCollection;
}

VolumeReader* MRCVolumeReader::create(ProgressBar* progress) const {
    return new MRCVolumeReader(progress);
}

} // namespace voreen
