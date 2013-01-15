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

#ifndef VRN_FHPTRANSFORMATION_H
#define VRN_FHPTRANSFORMATION_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/processors/imageprocessor.h"

#include "voreen/core/ports/volumeport.h"
#include "voreen/core/properties/boolproperty.h"

namespace voreen {

class FhpTransformation : public ImageProcessor {

public:
    FhpTransformation();
    virtual std::string getCategory() const { return "Image Processing"; }
    virtual std::string getClassName() const { return "FhpTransformation"; }
    virtual CodeState getCodeState() const { return CODE_STATE_TESTING; }
    virtual Processor* create() const { return new FhpTransformation(); }

    virtual ~FhpTransformation();

    void process();
protected:
    virtual void setDescriptions() {
        setDescription("Transforms volume FHPs to world FHPs.");
    }

    BoolProperty applyDatasetTransformationMatrix_;  ///< Apply transformation matrix assigned to dataset.

    RenderPort inport_;
    RenderPort outport_;
    VolumePort volPort_;
};


} // namespace voreen

#endif

