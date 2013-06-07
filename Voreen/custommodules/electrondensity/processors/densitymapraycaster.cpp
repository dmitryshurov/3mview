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

#include "densitymapraycaster.h"

#include "tgt/textureunit.h"
#include "voreen/core/ports/conditions/portconditionvolumetype.h"
#include "voreen/core/datastructures/transfunc/preintegrationtable.h"
#include "voreen/core/utils/classificationmodes.h"

#include <sstream>

using tgt::vec3;
using tgt::TextureUnit;

namespace voreen {

const std::string DensityMapRaycaster::loggerCat_("voreen.DensityMapRaycaster");

DensityMapRaycaster::DensityMapRaycaster()
    : VolumeRaycaster()
    , volumeInport_(Port::INPORT, "volumehandle.volumehandle", "Volume Input", false, Processor::INVALID_PROGRAM)
    , entryPort_(Port::INPORT, "image.entrypoints", "Entry-points Input", false, Processor::INVALID_RESULT, RenderPort::RENDERSIZE_ORIGIN)
    , exitPort_(Port::INPORT, "image.exitpoints", "Exit-points Input", false, Processor::INVALID_RESULT, RenderPort::RENDERSIZE_ORIGIN)
    , outport_(Port::OUTPORT, "image.output", "Image Output", true, Processor::INVALID_PROGRAM, RenderPort::RENDERSIZE_RECEIVER)
    , outport1_(Port::OUTPORT, "image.output1", "Image1 Output", true, Processor::INVALID_PROGRAM, RenderPort::RENDERSIZE_RECEIVER)
    , outport2_(Port::OUTPORT, "image.output2", "Image2 Output", true, Processor::INVALID_PROGRAM, RenderPort::RENDERSIZE_RECEIVER)
    , shaderProp_("raycast.prg", "Raycasting Shader", "rc_singlevolume.frag", "passthrough.vert")
    , transferFunc_("transferFunction", "Transfer Function")
    , camera_("camera", "Camera", tgt::Camera(vec3(0.f, 0.f, 3.5f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f)))
    , compositingMode1_("compositing1", "Compositing (OP2)", Processor::INVALID_PROGRAM)
    , compositingMode2_("compositing2", "Compositing (OP3)", Processor::INVALID_PROGRAM)
    , gammaValue_("gammaValue", "Gamma Value (OP1)", 0, -1, 1)
    , gammaValue1_("gammaValue1", "Gamma Value (OP2)", 0, -1, 1)
    , gammaValue2_("gammaValue2", "Gamma Value (OP3)", 0, -1, 1)
{
    // ports
    volumeInport_.addCondition(new PortConditionVolumeTypeGL());
    volumeInport_.showTextureAccessProperties(true);
    addPort(volumeInport_);
    addPort(entryPort_);
    addPort(exitPort_);
    addPort(outport_);
    addPort(outport1_);
    addPort(outport2_);

    addProperty(shaderProp_);

    // shading / classification props
    addProperty(transferFunc_);
    addProperty(camera_);
    addProperty(gradientMode_);
    addProperty(classificationMode_);
    addProperty(shadeMode_);

    gammaValue_.setTracking(true);
    addProperty(gammaValue_);
    gammaValue1_.setTracking(true);
    addProperty(gammaValue1_);
    gammaValue2_.setTracking(true);
    addProperty(gammaValue2_);

    // compositing modes
    addProperty(compositingMode_);
    compositingMode1_.addOption("dvr", "DVR");
    compositingMode1_.addOption("mip", "MIP");
    compositingMode1_.addOption("mida", "MIDA");
    compositingMode1_.addOption("iso", "ISO");
    compositingMode1_.addOption("fhp", "FHP");
    compositingMode1_.addOption("fhn", "FHN");
    addProperty(compositingMode1_);

    compositingMode2_.addOption("dvr", "DVR");
    compositingMode2_.addOption("mip", "MIP");
    compositingMode2_.addOption("mida", "MIDA");
    compositingMode2_.addOption("iso", "ISO");
    compositingMode2_.addOption("fhp", "FHP");
    compositingMode2_.addOption("fhn", "FHN");
    addProperty(compositingMode2_);

    addProperty(isoValue_);

    // lighting
    addProperty(lightPosition_);
    addProperty(lightAmbient_);
    addProperty(lightDiffuse_);
    addProperty(lightSpecular_);
    addProperty(materialShininess_);
    addProperty(applyLightAttenuation_);
    addProperty(lightAttenuation_);

    // assign lighting properties to property group
    lightPosition_.setGroupID("lighting");
    lightAmbient_.setGroupID("lighting");
    lightDiffuse_.setGroupID("lighting");
    lightSpecular_.setGroupID("lighting");
    materialShininess_.setGroupID("lighting");
    applyLightAttenuation_.setGroupID("lighting");
    lightAttenuation_.setGroupID("lighting");
    setPropertyGroupGuiName("lighting", "Lighting Parameters");

    // listen to changes of properties that influence the GUI state (i.e. visibility of other props)
    classificationMode_.onChange(CallMemberAction<DensityMapRaycaster>(this, &DensityMapRaycaster::adjustPropertyVisibilities));
    shadeMode_.onChange(CallMemberAction<DensityMapRaycaster>(this, &DensityMapRaycaster::adjustPropertyVisibilities));
    compositingMode_.onChange(CallMemberAction<DensityMapRaycaster>(this, &DensityMapRaycaster::adjustPropertyVisibilities));
    compositingMode1_.onChange(CallMemberAction<DensityMapRaycaster>(this, &DensityMapRaycaster::adjustPropertyVisibilities));
    compositingMode2_.onChange(CallMemberAction<DensityMapRaycaster>(this, &DensityMapRaycaster::adjustPropertyVisibilities));
    applyLightAttenuation_.onChange(CallMemberAction<DensityMapRaycaster>(this, &DensityMapRaycaster::adjustPropertyVisibilities));
}

Processor* DensityMapRaycaster::create() const {
    return new DensityMapRaycaster();
}

void DensityMapRaycaster::initialize() throw (tgt::Exception) {
    VolumeRaycaster::initialize();
    compile();

    portGroup_.initialize();
    portGroup_.addPort(outport_);
    portGroup_.addPort(outport1_);
    portGroup_.addPort(outport2_);

    adjustPropertyVisibilities();

    if (transferFunc_.get()) {
        transferFunc_.get()->getTexture();
        transferFunc_.get()->invalidateTexture();
    }
}

void DensityMapRaycaster::deinitialize() throw (tgt::Exception) {
    portGroup_.deinitialize();
    LGL_ERROR;

    VolumeRaycaster::deinitialize();
}

void DensityMapRaycaster::compile() {
    shaderProp_.setHeader(generateHeader());
    shaderProp_.rebuild();
}

bool DensityMapRaycaster::isReady() const {
    //check if all inports are connected
    if(!entryPort_.isReady() || !exitPort_.isReady() || !volumeInport_.isReady())
        return false;

    //check if at least one outport is connected
    if (!outport_.isReady() && !outport1_.isReady() && !outport2_.isReady())
        return false;

    if(!shaderProp_.hasValidShader())
        return false;

    return true;
}

void DensityMapRaycaster::beforeProcess() {
    VolumeRaycaster::beforeProcess();
    
    //XXX
    const VolumeBase* vol = volumeInport_.getData();

    if (typeid(*vol) == typeid(const MoleculeVolume)) {
        const MoleculeVolume* v = dynamic_cast<const MoleculeVolume*>(vol);
        TransFunc* tf = v->getTransFunc()->clone();
        transferFunc_.set(tf);

    }
    else {
        LWARNING("Given volume is not a MoleculeVolume");
    }
    //XXX

    // compile program if needed
    if (getInvalidationLevel() >= Processor::INVALID_PROGRAM) {
        PROFILING_BLOCK("compile");
        compile();
    }
    LGL_ERROR;

    transferFunc_.setVolumeHandle(volumeInport_.getData());
}

void DensityMapRaycaster::process() {
    // bind transfer function
    tgt::TextureUnit transferUnit;
    transferUnit.activate();
    LGL_ERROR;
    ClassificationModes::bindTexture(classificationMode_.get(), transferFunc_.get(), getSamplingStepSize(volumeInport_.getData()));

    portGroup_.activateTargets();
    portGroup_.clearTargets();
    LGL_ERROR;

    // bind entry params
    tgt::TextureUnit entryUnit, entryDepthUnit, exitUnit, exitDepthUnit;
    entryPort_.bindTextures(entryUnit, entryDepthUnit);
    LGL_ERROR;

    // bind exit params
    exitPort_.bindTextures(exitUnit, exitDepthUnit);
    LGL_ERROR;

    // vector containing the volumes to bind; is passed to bindVolumes()
    std::vector<VolumeStruct> volumeTextures;

    // add main volume
    TextureUnit volUnit;
    volumeTextures.push_back(VolumeStruct(
        volumeInport_.getData(),
        &volUnit,
        "volume_","volumeStruct_",
        volumeInport_.getTextureClampModeProperty().getValue(),
        tgt::vec4(volumeInport_.getTextureBorderIntensityProperty().get()),
        volumeInport_.getTextureFilterModeProperty().getValue())
    );

    // initialize shader
    tgt::Shader* raycastPrg = shaderProp_.getShader();
    raycastPrg->activate();

    // set common uniforms used by all shaders
    tgt::Camera cam = camera_.get();
    setGlobalShaderParameters(raycastPrg, &cam);
    // bind the volumes and pass the necessary information to the shader
    bindVolumes(raycastPrg, volumeTextures, &cam, lightPosition_.get());

    // pass the remaining uniforms to the shader
    raycastPrg->setUniform("entryPoints_", entryUnit.getUnitNumber());
    raycastPrg->setUniform("entryPointsDepth_", entryDepthUnit.getUnitNumber());
    entryPort_.setTextureParameters(raycastPrg, "entryParameters_");
    raycastPrg->setUniform("exitPoints_", exitUnit.getUnitNumber());
    raycastPrg->setUniform("exitPointsDepth_", exitDepthUnit.getUnitNumber());
    exitPort_.setTextureParameters(raycastPrg, "exitParameters_");

    if (compositingMode_.isSelected("iso")  ||
        compositingMode1_.isSelected("iso") ||
        compositingMode2_.isSelected("iso") )
        raycastPrg->setUniform("isoValue_", isoValue_.get());

    if (ClassificationModes::usesTransferFunction(classificationMode_.get()))
        transferFunc_.get()->setUniform(raycastPrg, "transferFunc_", "transferFuncTex_", transferUnit.getUnitNumber());

    if (compositingMode_.isSelected("mida"))
        raycastPrg->setUniform("gammaValue_", gammaValue_.get());

    if (compositingMode1_.isSelected("mida"))
        raycastPrg->setUniform("gammaValue1_", gammaValue1_.get());

    if (compositingMode2_.isSelected("mida"))
        raycastPrg->setUniform("gammaValue2_", gammaValue2_.get());

    LGL_ERROR;

    {
        PROFILING_BLOCK("raycasting");
        renderQuad();
    }

    raycastPrg->deactivate();
    portGroup_.deactivateTargets();

    TextureUnit::setZeroUnit();
    LGL_ERROR;
}

std::string DensityMapRaycaster::generateHeader() {
    std::string headerSource = VolumeRaycaster::generateHeader();

    headerSource += ClassificationModes::getShaderDefineSamplerType(classificationMode_.get(), transferFunc_.get()); 

    // configure compositing mode for port 2
    headerSource += "#define RC_APPLY_COMPOSITING_1(result, color, samplePos, gradient, t, samplingStepSize, tDepth) ";
    if (compositingMode1_.isSelected("dvr"))
        headerSource += "compositeDVR(result, color, t, samplingStepSize, tDepth);\n";
    else if (compositingMode1_.isSelected("mip"))
        headerSource += "compositeMIP(result, color, t, tDepth);\n";
    else if (compositingMode1_.isSelected("mida"))
        headerSource += "compositeMIDA(result, voxel, color, f_max_i1, t, tDepth, gammaValue1_);\n";
    else if (compositingMode1_.isSelected("iso"))
        headerSource += "compositeISO(result, color, t, tDepth, isoValue_);\n";
    else if (compositingMode1_.isSelected("fhp"))
        headerSource += "compositeFHP(samplePos, result, t, tDepth);\n";
    else if (compositingMode1_.isSelected("fhn"))
        headerSource += "compositeFHN(gradient, result, t, tDepth);\n";

    // configure compositing mode for port 3
    headerSource += "#define RC_APPLY_COMPOSITING_2(result, color, samplePos, gradient, t, samplingStepSize, tDepth) ";
    if (compositingMode2_.isSelected("dvr"))
        headerSource += "compositeDVR(result, color, t, samplingStepSize, tDepth);\n";
    else if (compositingMode2_.isSelected("mip"))
        headerSource += "compositeMIP(result, color, t, tDepth);\n";
    else if (compositingMode2_.isSelected("mida"))
        headerSource += "compositeMIDA(result, voxel, color, f_max_i2, t, tDepth, gammaValue2_);\n";
    else if (compositingMode2_.isSelected("iso"))
        headerSource += "compositeISO(result, color, t, tDepth, isoValue_);\n";
    else if (compositingMode2_.isSelected("fhp"))
        headerSource += "compositeFHP(samplePos, result, t, tDepth);\n";
    else if (compositingMode2_.isSelected("fhn"))
        headerSource += "compositeFHN(gradient, result, t, tDepth);\n";

    portGroup_.reattachTargets();
    headerSource += portGroup_.generateHeader(shaderProp_.getShader());
    return headerSource;
}

void DensityMapRaycaster::adjustPropertyVisibilities() {
    bool useLighting = !shadeMode_.isSelected("none");
    setPropertyGroupVisible("lighting", useLighting);

    bool useIsovalue = (compositingMode_.isSelected("iso")  ||
                        compositingMode1_.isSelected("iso") ||
                        compositingMode2_.isSelected("iso")   );
    isoValue_.setVisible(useIsovalue);

    lightAttenuation_.setVisible(applyLightAttenuation_.get());

    gammaValue_.setVisible(compositingMode_.isSelected("mida"));
    gammaValue1_.setVisible(compositingMode1_.isSelected("mida"));
    gammaValue2_.setVisible(compositingMode2_.isSelected("mida"));
}

} // namespace
