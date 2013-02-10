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

#ifndef VRN_MRCVOLUMEREADER_H
#define VRN_MRCVOLUMEREADER_H

#include "voreen/core/io/volumereader.h"

namespace voreen {

class IOProgress;

/**
 * Reads a multi-image TIFF file into a volume dataset.
 */
class MRCVolumeReader : public VolumeReader {
public:
    MRCVolumeReader(ProgressBar* progress = 0);
    ~MRCVolumeReader() {}
    virtual VolumeReader* create(ProgressBar* progress = 0) const;

    virtual std::string getClassName() const   { return "MRCVolumeReader"; }
    virtual std::string getFormatDescription() const { return "MRC/CCP4 electron density map format"; }

    virtual VolumeCollection* read(const std::string& url)
        throw (tgt::FileException, tgt::IOException, std::bad_alloc);

private:
    static const std::string loggerCat_;
};

} // namespace voreen

#endif // VRN_MRCVOLUMEREADER_H
