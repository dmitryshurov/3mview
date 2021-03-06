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

#ifndef VRN_CAMERAPROPERTY_H
#define VRN_CAMERAPROPERTY_H

#include "voreen/core/properties/templateproperty.h"
#include "voreen/core/properties/condition.h"
#include "tgt/camera.h"

namespace voreen {

#ifdef DLL_TEMPLATE_INST
template class VRN_CORE_API TemplateProperty<tgt::Camera>;
#endif

class VoreenTrackball;

/**
 * Property encapsulating a tgt::Camera object.
 */
class VRN_CORE_API CameraProperty : public TemplateProperty<tgt::Camera> {

public:

    /**
     * Constructor.
     *
     * @param adjustProjectionToViewport when set to true, the camera's projection matrix
     *        is adjusted automatically to viewport changes. This is especially necessary to
     *        reflect the viewport's aspect ratio.
     */
    CameraProperty(const std::string& id, const std::string& guiText, tgt::Camera const value = tgt::Camera(),
               bool adjustProjectionToViewport = true,
               float maxValue = 500.f,
               int invalidationLevel=Processor::INVALID_RESULT);
    CameraProperty();
    virtual ~CameraProperty();

    virtual Property* create() const;

    virtual std::string getClassName() const       { return "CameraProperty"; }
    virtual std::string getTypeDescription() const { return "Camera"; }

    /**
     * Assigns the passed camera object to the property.
     */
    void set(const tgt::Camera& camera);
    void setPosition(const tgt::vec3& pos);
    void setFocus(const tgt::vec3& focus);
    void setUpVector(const tgt::vec3& up);
    void setFrustum(const tgt::Frustum& frust);
    void setFarDist(float dist);
    void setNearDist(float dist);

    bool setStereoEyeSeparation(float separation);
    bool setStereoEyeMode(tgt::Camera::StereoEyeMode mode);
    //bool stereoShift(tgt::vec3 shift);
    void setStereoAxisMode(tgt::Camera::StereoAxisMode mode);

    /// Set the maximum absolute value that an element of the camera position vector can have
    void setMaxValue(float val);

    /// \sa setMaxValue
    float getMaxValue() const;

    /**
     * Executes all member actions that belong to the property. Generally the owner of the
     * property is invalidated.
     */
    void notifyChange();

    /**
     * @see Property::serialize
     */
    virtual void serialize(XmlSerializer& s) const;

    /**
     * @see Property::deserialize
     */
    virtual void deserialize(XmlDeserializer& s);

    virtual void look(tgt::ivec2 viewportSize);
    VoreenTrackball* getTrackball();

private:

    VoreenTrackball* track_;
    float maxValue_;
};

} // namespace voreen

#endif // VRN_CAMERAPROPERTY_H
