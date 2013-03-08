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

#include "canvasrenderer.h"

#include "voreen/core/utils/voreenpainter.h"
#include "voreen/core/utils/stringutils.h"
#include "voreen/core/processors/processorwidget.h"
#include "voreen/core/processors/processorwidgetfactory.h"
#include "voreen/core/voreenapplication.h"

#include "tgt/glcanvas.h"
#include "tgt/texturemanager.h"
#include "tgt/gpucapabilities.h"
#include "tgt/filesystem.h"

#ifdef VRN_MODULE_DEVIL
#include "modules/devil/devilmodule.h"
#endif

namespace voreen {

const std::string CanvasRenderer::loggerCat_("voreen.core.CanvasRenderer");

CanvasRenderer::CanvasRenderer()
    : RenderProcessor()
    , canvasSize_("canvasSize", "Canvas Size", tgt::ivec2(256), tgt::ivec2(32), tgt::ivec2(2 << 12), Processor::VALID)
    , showCursor_("showCursor", "Show Cursor", true)
    , showFullScreen_("showFullScreen", "Show Fullscreen (F11)", false)
    , screenshotFilename_("screenshotFilename", "File", "Select file...","", "*.*", FileDialogProperty::SAVE_FILE, Processor::VALID)
    , saveScreenshotButton_("saveScreenshot", "Save Screenshot")
    , canvas_(0)
    , shader_(0)
    , inportMono_(Port::INPORT, "image.input", "Mono Input", false, Processor::INVALID_RESULT, RenderPort::RENDERSIZE_ORIGIN)
    , inportLeft_(Port::INPORT, "image.inputLeft", "Left Eye Input", false, Processor::INVALID_RESULT, RenderPort::RENDERSIZE_ORIGIN)
    , inportRight_(Port::INPORT, "image.inputRight", "Right Eye Input", false, Processor::INVALID_RESULT, RenderPort::RENDERSIZE_ORIGIN)
    , errorTex_(0)
    , renderToImage_(false)
    , renderToImageFilename_("")
{
    addPort(inportMono_);
    addPort(inportLeft_);
    addPort(inportRight_);
    addProperty(canvasSize_);

    addProperty(showCursor_);
    addProperty(showFullScreen_);

    addProperty(screenshotFilename_);
    addProperty(saveScreenshotButton_);
    screenshotFilename_.setGroupID("screenshot");
    saveScreenshotButton_.setGroupID("screenshot");
    setPropertyGroupGuiName("screenshot", "Screenshot");

    canvasSize_.onChange(CallMemberAction<CanvasRenderer>(this, &CanvasRenderer::sizePropChanged));
    showCursor_.onChange(CallMemberAction<CanvasRenderer>(this, &CanvasRenderer::boolPropertyChanged));
    showFullScreen_.onChange(CallMemberAction<CanvasRenderer>(this, &CanvasRenderer::boolPropertyChanged));
    saveScreenshotButton_.onChange(CallMemberAction<CanvasRenderer>(this, &CanvasRenderer::saveScreenshotClicked));
}

CanvasRenderer::~CanvasRenderer() {}

Processor* CanvasRenderer::create() const {
    return new CanvasRenderer();
}

void CanvasRenderer::process() {
    if (!canvas_)
        return;

    canvas_->getGLFocus();
    glViewport(0, 0, canvas_->getSize().x, canvas_->getSize().y);

    if (inportMono_.isReady() || (inportLeft_.isReady() && inportRight_.isReady())) {
        // render inport to image, if renderToImage flag has been set
        if (renderToImage_) {
            try {
                renderInportToImage(renderToImageFilename_);
                LINFO("Saved rendering with dimensions " << inportLeft_.getSize() << " to file: " << tgt::FileSystem::cleanupPath(renderToImageFilename_));
            }
            catch (std::bad_alloc& /*e*/) {
                LERROR("Exception in CanvasRenderer::renderInportToImage(): bad allocation (" << getName() << ")");
                renderToImageError_ = "Not enough system memory (bad allocation)";
            }
            catch (VoreenException& e) {
                LERROR(e.what());
                renderToImageError_ = std::string(e.what());
            }
            catch (std::exception& e) {
                LERROR("Exception in CanvasRenderer::renderInportToImage(): " << e.what() << " (" << getName() << ")");
                renderToImageError_ = std::string(e.what());
            }
            renderToImage_ = false;
        }
        // map texture of input target onto a screen-aligned quad
        else {
        	if (inportMono_.isReady()) {
		        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		        // activate shader
		        shader_->activate();

		        // set common uniforms
		        setGlobalShaderParameters(shader_);

		        // manually pass the viewport dimensions to the shader,
		        // since setGlobalShaderParameters() expects a render outport, which we do not have
		        shader_->setIgnoreUniformLocationError(true);
		        shader_->setUniform("screenDim_", tgt::vec2(canvas_->getSize()));
		        shader_->setUniform("screenDimRCP_", 1.f / tgt::vec2(canvas_->getSize()));
		        shader_->setIgnoreUniformLocationError(false);

		        // bind input textures
		        inportMono_.bindTextures(GL_TEXTURE0, GL_TEXTURE1);

		        // pass texture parameters to the shader
		        shader_->setUniform("colorTex_", 0);
		        shader_->setUniform("depthTex_", 1);
		        inportMono_.setTextureParameters(shader_, "texParams_");
		        LGL_ERROR;

		        // execute the shader
		        renderQuad();
		        shader_->deactivate();
		        LGL_ERROR;
            }
            else if (inportLeft_.isReady() && inportRight_.isReady()) {
            	glDrawBuffer(GL_BACK_LEFT);
		        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		        // activate shader
		        shader_->activate();

		        // set common uniforms
		        setGlobalShaderParameters(shader_);

		        // manually pass the viewport dimensions to the shader,
		        // since setGlobalShaderParameters() expects a render outport, which we do not have
		        shader_->setIgnoreUniformLocationError(true);
		        shader_->setUniform("screenDim_", tgt::vec2(canvas_->getSize()));
		        shader_->setUniform("screenDimRCP_", 1.f / tgt::vec2(canvas_->getSize()));
		        shader_->setIgnoreUniformLocationError(false);

		        // bind input textures
		        inportLeft_.bindTextures(GL_TEXTURE0, GL_TEXTURE1);

		        // pass texture parameters to the shader
		        shader_->setUniform("colorTex_", 0);
		        shader_->setUniform("depthTex_", 1);
		        inportLeft_.setTextureParameters(shader_, "texParams_");
		        LGL_ERROR;

		        // execute the shader
		        renderQuad();
		        shader_->deactivate();
		        LGL_ERROR;
		       

		       
            	glDrawBuffer(GL_BACK_RIGHT);
		        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		        // activate shader
		        shader_->activate();

		        // set common uniforms
		        setGlobalShaderParameters(shader_);

		        // manually pass the viewport dimensions to the shader,
		        // since setGlobalShaderParameters() expects a render outport, which we do not have
		        shader_->setIgnoreUniformLocationError(true);
		        shader_->setUniform("screenDim_", tgt::vec2(canvas_->getSize()));
		        shader_->setUniform("screenDimRCP_", 1.f / tgt::vec2(canvas_->getSize()));
		        shader_->setIgnoreUniformLocationError(false);

		        // bind input textures
		        inportRight_.bindTextures(GL_TEXTURE0, GL_TEXTURE1);

		        // pass texture parameters to the shader
		        shader_->setUniform("colorTex_", 0);
		        shader_->setUniform("depthTex_", 1);
		        inportRight_.setTextureParameters(shader_, "texParams_");
		        LGL_ERROR;

		        // execute the shader
		        renderQuad();
		        shader_->deactivate();
		        LGL_ERROR;
            }
        }
    }
    else {
        // render error texture
        if (!errorTex_) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return;
        }
        glClear(GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        errorTex_->bind();
        errorTex_->enable();

        glColor3f(1.f, 1.f, 1.f);
        renderQuad();

        errorTex_->disable();
    }

    glActiveTexture(GL_TEXTURE0);
    LGL_ERROR;
}

bool CanvasRenderer::isReady() const {
    return true;
}

bool CanvasRenderer::isValid() const {
    return false;
}

void CanvasRenderer::initialize() throw (tgt::Exception) {
    RenderProcessor::initialize();

#ifdef VRN_MODULE_DEVIL
    std::string imageExtensions = "*." + strJoin(DevILModule::getSupportedWriteExtensions(), ";; *.");
    screenshotFilename_.setFileFilter(imageExtensions);
#endif

    if (getProcessorWidget())
        getProcessorWidget()->updateFromProcessor();

    shader_ = ShdrMgr.loadSeparate("passthrough.vert", "copyimage.frag", generateHeader(), false);

    errorTex_ = TexMgr.load(VoreenApplication::app()->getResourcePath("textures/error.tga"));
    canvasSize_.setMaxValue(tgt::ivec2(GpuCaps.getMaxTextureSize()));
}

void CanvasRenderer::deinitialize() throw (tgt::Exception) {
   ShdrMgr.dispose(shader_);
   shader_ = 0;
   if (errorTex_)
       TexMgr.dispose(errorTex_);
   errorTex_ = 0;
   LGL_ERROR;

   setCanvas(0);
   LGL_ERROR;

   RenderProcessor::deinitialize();
}

bool CanvasRenderer::isEndProcessor() const {
    return true;
}

void CanvasRenderer::canvasResized(tgt::ivec2 newsize) {
    if (canvas_) {
        canvas_->getGLFocus();
        canvasSize_.set(newsize);
        inportLeft_.requestSize(newsize);
    }
}

void CanvasRenderer::invalidate(int inv) {
    // Since VoreenPainter::paint() already calls process() on the
    // NetworkEvaluator, we bypass Processor::invalidate()
    // in order to prevent a needless double processing of the network.
    PropertyOwner::invalidate(inv);
    if (canvas_)
        canvas_->update();
}

void CanvasRenderer::setCanvas(tgt::GLCanvas* canvas) {
    if (canvas == canvas_)
        return;

    //remove from old canvas:
    if (canvas_) {
        tgt::EventHandler* eh = canvas_->getEventHandler();
        if (eh) {
            eh->removeListener(this);
        }
    }
    canvas_ = canvas;
    //register at new canvas:
    if (canvas_) {
        tgt::EventHandler* eh = canvas_->getEventHandler();
        if (eh) {
            eh->addListenerToFront(this);
        }
        inportLeft_.requestSize(canvas_->getSize());
    }

    invalidate();
}

tgt::GLCanvas* CanvasRenderer::getCanvas() const {
    return canvas_;
}

void CanvasRenderer::onEvent(tgt::Event* e) {
    if (canvas_) {
        canvas_->getGLFocus();
        Processor::onEvent(e);
    }
}

bool CanvasRenderer::renderToImage(const std::string &filename) {
    if (!canvas_) {
        LWARNING("CanvasRenderer::renderToImage(): no canvas assigned");
        renderToImageError_ = "No canvas assigned";
        return false;
    }

    if (!inportLeft_.hasRenderTarget()) {
        LWARNING("CanvasRenderer::renderToImage(): inport has no data");
        renderToImageError_ = "No rendering";
        return false;
    }

    // enable render-to-file on next process
    renderToImageFilename_ = filename;
    renderToImage_ = true;
    renderToImageError_.clear();

    // force rendering pass
    canvas_->repaint();

    return (renderToImageError_.empty());
}

bool CanvasRenderer::renderToImage(const std::string &filename, tgt::ivec2 dimensions) {
    if (!canvas_) {
        LWARNING("CanvasRenderer::renderToImage(): no canvas assigned");
        renderToImageError_ = "No canvas assigned";
        return false;
    }

    if (!inportLeft_.hasRenderTarget()) {
        LWARNING("CanvasRenderer::renderToImage(): inport has no data");
        renderToImageError_ = "No rendering";
        return false;
    }

    tgt::ivec2 oldDimensions = inportLeft_.getSize();
    // resize texture container to desired image dimensions and propagate change
    canvas_->getGLFocus();
    inportLeft_.requestSize(dimensions);

    // render with adjusted viewport size
    bool success = renderToImage(filename);

    // reset texture container dimensions from canvas size
    inportLeft_.requestSize(oldDimensions);

    return success;
}

void CanvasRenderer::renderInportToImage(const std::string& filename) throw (VoreenException) {
    if (!inportLeft_.hasRenderTarget())
        throw VoreenException("No rendering");

    inportLeft_.saveToImage(filename);
}

const tgt::Texture* CanvasRenderer::getImageColorTexture() const {
    if (inportLeft_.hasRenderTarget())
        return inportLeft_.getColorTexture();
    else
        return 0;
}

tgt::Texture* CanvasRenderer::getImageColorTexture() {
    if (inportLeft_.hasRenderTarget())
        return inportLeft_.getColorTexture();
    else
        return 0;
}

const tgt::Texture* CanvasRenderer::getImageDepthTexture() const {
    if (inportLeft_.hasRenderTarget())
        return inportLeft_.getDepthTexture();
    else
        return 0;
}

tgt::Texture* CanvasRenderer::getImageDepthTexture() {
    if (inportLeft_.hasRenderTarget())
        return inportLeft_.getDepthTexture();
    else
        return 0;
}

std::string CanvasRenderer::getRenderToImageError() const {
    return renderToImageError_;
}

tgt::ivec2 CanvasRenderer::getCanvasSize() const {
    return canvasSize_.get();
}

void CanvasRenderer::resizeCanvas(tgt::ivec2 newsize) {
    if (!tgt::hand(tgt::greaterThanEqual(newsize, tgt::ivec2(canvasSize_.getMinValue()))) && tgt::hand(tgt::lessThanEqual(newsize, canvasSize_.getMaxValue()))) {
        LWARNING("Invalid canvas dimensions: " << newsize << ". Ignoring.");
    }

    if (!canvas_)
        return;

    if (getProcessorWidget() && getProcessorWidget()->getSize() != newsize) {
        if (getProcessorWidget()->isVisible()) {
            getProcessorWidget()->setSize(newsize.x, newsize.y);
        }
        newsize = getProcessorWidget()->getSize();
    }

    if (newsize != inportLeft_.getSize()) {
        canvas_->getGLFocus();
        glViewport(0, 0, static_cast<GLint>(newsize.x), static_cast<GLint>(newsize.y));
        inportMono_.requestSize(newsize);
    }

    canvasSize_.set(newsize);
}

void CanvasRenderer::sizePropChanged() {
    resizeCanvas(canvasSize_.get());
}


void CanvasRenderer::boolPropertyChanged() {
    if(getProcessorWidget())
        getProcessorWidget()->updateFromProcessor();
}

void CanvasRenderer::saveScreenshotClicked() {
    if (screenshotFilename_.get().empty()) {
        LWARNING("No screenshot filename set");
        return;
    }

    try {
        renderToImage(screenshotFilename_.get());
        //LINFO("Saved rendering with dimensions " << inportLeft_.getSize() << " to file: " << screenshotFilename_.get());
    }
    catch (VoreenException& e) {
        LERROR("Failed to write screenshot to file '" << screenshotFilename_.get() << "': " << e.what());
    }

}

} // namespace voreen
