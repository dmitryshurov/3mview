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

#include "volumesource.h"

#include "voreen/core/datastructures/volume/volume.h"
#include "voreen/core/processors/processorwidget.h"
#include "voreen/core/ports/allports.h"
#include "voreen/core/io/volumeserializerpopulator.h"
#include "voreen/core/io/volumeserializer.h"
#include "voreen/core/voreenapplication.h"

namespace voreen {

const std::string VolumeSource::loggerCat_("voreen.core.VolumeSource");

VolumeSource::VolumeSource()
    : Processor()
    , volumeURL_("volumeURL", "Volume", "")
    , outport_(Port::OUTPORT, "volumehandle.volumehandle", "Volume Output")
{
    addPort(outport_);
    addProperty(volumeURL_);
}

Processor* VolumeSource::create() const {
    return new VolumeSource();
}

void VolumeSource::process() {}

void VolumeSource::initialize() throw (tgt::Exception) {
    Processor::initialize();

    // load volume from URL stored in volumeURL property
    if (volumeURL_.get() != "") {
        try {
            volumeURL_.loadVolume();
        }
        catch (tgt::FileException& e) {
            LERROR(e.what());
        }
        catch (std::bad_alloc&) {
            LERROR("bad allocation while loading volume: " << volumeURL_.get());
        }
        catch (tgt::Exception& e) {
            LERROR("unknown exception while loading volume '" << volumeURL_.get() << "':" << e.what());
        }

        // clear URL, if volume could not be loaded
        if (!volumeURL_.getVolume())
            volumeURL_.clear();
    }

    outport_.setData(volumeURL_.getVolume(), false);

    if (getProcessorWidget())
        getProcessorWidget()->updateFromProcessor();
}

void VolumeSource::deinitialize() throw (tgt::Exception) {
    clearVolume();

    Processor::deinitialize();
}

void VolumeSource::loadVolume(const std::string& url) throw (tgt::FileException, std::bad_alloc) {
    clearVolume();

    volumeURL_.set(url);
    try {
        volumeURL_.loadVolume();
    }
    catch (tgt::FileException& e) {
        LWARNING(e.what());
    }
    catch (std::bad_alloc&) {
        LWARNING("bad allocation while loading volume: " << volumeURL_.get());
    }
    catch (tgt::Exception& e) {
        LWARNING("unknown exception while loading volume '" << volumeURL_.get() << "':" << e.what());
    }
    if (!volumeURL_.getVolume())
        volumeURL_.clear();

    /*if (getProcessorWidget())
        getProcessorWidget()->updateFromProcessor(); */
}

void VolumeSource::clearVolume() {
    if (volumeURL_.getVolume()) {
        volumeURL_.setVolume(0);
    }
}

void VolumeSource::setVolumeHandle(VolumeBase* handle) {
    volumeURL_.setVolume(handle, false);
}

VolumeBase* VolumeSource::getVolumeHandle() const {
    return volumeURL_.getVolume();
}

void VolumeSource::invalidate(int inv) {
    Processor::invalidate(inv);

    if (outport_.getData() != volumeURL_.getVolume()) {
        outport_.setData(volumeURL_.getVolume(), false);
        if (getProcessorWidget())
            getProcessorWidget()->updateFromProcessor();
    }
}

} // namespace
