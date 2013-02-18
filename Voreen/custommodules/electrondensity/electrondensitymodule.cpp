/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Copyright (C) 2005-2012 Visualization and Computer Graphics Group, *
 * Department of Computer Science, University of Muenster, Germany.   *
 * <http://viscg.uni-muenster.de>                                     *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#include "electrondensitymodule.h"

// include classes to be registered
#include "processors/segmentationbase.h"
#include "processors/manipulationbase.h"
#include "processors/densitymapcollectionsource.h"
#include "io/mrcvolumereader.h"

namespace voreen {

ElectronDensityModule::ElectronDensityModule(const std::string& modulePath) 
    : VoreenModule(modulePath)
{
    // module name to be used in the GUI
    setName("Electron Density Maps");
    
    registerProcessor(new SegmentationBase());
    registerProcessor(new ManipulationBase());
    registerProcessor(new DensityMapCollectionSource());
    
    registerVolumeReader(new MRCVolumeReader());
}

} // namespace
