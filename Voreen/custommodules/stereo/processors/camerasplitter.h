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

#ifndef VRN_CAMERASPLITTER_H
#define VRN_CAMERASPLITTER_H

#include "voreen/core/processors/renderprocessor.h"

#include "voreen/core/ports/renderport.h"
#include "voreen/core/properties/floatproperty.h"
#include "voreen/core/properties/cameraproperty.h"
#include "tgt/shadermanager.h"

namespace voreen {

class CameraSplitter : public RenderProcessor {
public:
    CameraSplitter();

    // virtual constructor
    virtual Processor* create() const;

    // documentary functions
    virtual std::string getClassName() const { return "CameraSplitter";       }
    virtual std::string getCategory() const  { return "Camera Managing";      }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void setDescriptions() {
        setDescription("Gets single camera as input and divides it into two cameras, one for each eye.");
    }

    // actual processing code
    virtual void process();

private:
    // ports and properties
    RenderPort inport_;
    RenderPort outport_;
    
    FloatProperty eyeSeparation_;
    CameraProperty mainCamera_;
    CameraProperty leftCamera_;
    CameraProperty rightCamera_;

};

} // namespace

#endif // VRN_CAMERASPLITTER_H
