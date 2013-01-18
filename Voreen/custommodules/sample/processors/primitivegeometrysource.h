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

#ifndef VRN_PRIMITIVEGEOMETRYSOURCE_H
#define VRN_PRIMITIVEGEOMETRYSOURCE_H

#include "voreen/core/processors/processor.h"
#include "voreen/core/ports/geometryport.h"
#include "voreen/core/properties/filedialogproperty.h"
#include "voreen/core/properties/optionproperty.h"
#include "voreen/core/properties/intproperty.h"
#include "voreen/core/properties/buttonproperty.h"

namespace voreen {

/**
 * Reads a Voreen Geometry, point list or segmented list from file
 * and provides it as geometry through its outport.
 */
class PrimitiveGeometrySource : public Processor {
public:
    PrimitiveGeometrySource();
    virtual Processor* create() const;

    virtual std::string getClassName() const { return "PrimitiveGeometrySource";   }
    virtual std::string getCategory() const  { return "Input";            }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL;  }

protected:
    virtual void setDescriptions() {
        setDescription("Loads primitive geometry.");
    }

    virtual void process();
    virtual void initialize() throw (tgt::Exception);

private:
    enum PointListType {
        PointList,          ///< files contain one point per line, elements separated by white space
        SegmentedPointList  ///< files contain one point per line, but with an additional segment id
    };

    /**
     * Delegates to readVoreenGeometry() or readPointList() depending on the selected geometry type
     * and assigns the returned geometry to the outport.
     */
    void readGeometry();

    /**
     * Deserializes a geometry from a Voreen Geometry file (*.vge).
     *
     * @return the read geometry, if deserialization succeeded
     *
     * @throw VoreenException if deserialization failed
     */
    Geometry* createPlaneGeometry() const
        throw (VoreenException);
	
    Geometry* createCubeGeometry() const
        throw (VoreenException);
        
    Geometry* loadGeometryFromFile(const std::string& filename)
        throw (VoreenException);
        
	void readVertices(std::istream& stream, std::vector<tgt::vec3>& vertices);

	void readVertexNormals(std::istream& stream, std::vector<tgt::vec3>& vertexNormals);

	bool readFace(std::istream& stream, std::vector<int>& faceVertices);

    /**
     * Removed the geometry from the output and clears the file property.
     */
    void clearGeometry();

    /// Adjusts the visibility of the skipItemCount_ property.
    void updatePropertyVisibility();

    StringOptionProperty geometryType_; ///< Determines whether the file is read as Voreen geometry, point or segment list.
    FileDialogProperty inputFile_;
    ButtonProperty loadGeometry_;
    ButtonProperty clearGeometry_;

    GeometryPort outport_;

    static const std::string loggerCat_;
};

}   //namespace

#endif // VRN_PRIMITIVEGEOMETRYSOURCE_H