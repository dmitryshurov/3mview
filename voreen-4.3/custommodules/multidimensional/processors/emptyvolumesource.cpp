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

#include "emptyvolumesource.h"

#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/datastructures/volume/volumeatomic.h"
#include "voreen/core/processors/processorwidget.h"
#include "voreen/core/io/volumeserializerpopulator.h"
#include "voreen/core/io/volumeserializer.h"
#include "voreen/core/voreenapplication.h"

#include "tgt/vector.h"

namespace voreen {

const std::string EmptyVolumeSource::loggerCat_("voreen.multidimensional.EmptyVolumeSource");

EmptyVolumeSource::EmptyVolumeSource()
    : Processor()
    , outport_(Port::OUTPORT, "volumehandle.volumehandle", "Volume Output")
{
    addPort(outport_);
    VolumeRAM* data = new VolumeAtomic<float>(tgt::ivec3(2,2,2));
    Volume* volume = new Volume(data, tgt::vec3(1,1,1), tgt::vec3(0,0,0));
    outport_.setData(volume, true);
}

Processor* EmptyVolumeSource::create() const {
    return new EmptyVolumeSource();
}

} // namespace
