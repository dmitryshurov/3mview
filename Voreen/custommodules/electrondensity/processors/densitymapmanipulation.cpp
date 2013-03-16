#include "densitymapmanipulation.h"

DensityMapManipulation::DensityMapManipulation()
    : ManipulationBase()
    , volumeURLList_("volumeURLList", "Volume URL List", std::vector<std::string>())
{
	addProperty(volumeURLList_);
	allowInvalidation_ = true;
}

void DensityMapManipulation::applyTransformation(tgt::vec3 offset, tgt::mat4 transform) {
        const VolumeCollection* collection = volumeURLList_.getVolumes(true);
        if (collection == 0 || collection->size() == 0) return;
        
        for (size_t i = 0; i < collection->size(); i++) {
            VolumeBase* volume = collection->at(i);
            
            if (typeid(*volume) != typeid(Volume)) {
                LWARNING("Base class is not an instance of Volume");
                continue;
            }
            
            tgt::vec3 volumeOffset = volume->getOffset() + offset;
            static_cast<Volume*>(volume)->setOffset(volumeOffset);
            
            tgt::mat4 matrix = volume->getPhysicalToWorldMatrix();
            static_cast<Volume*>(volume)->setPhysicalToWorldMatrix(transform * matrix);
        }
        
        DensityMapCollectionSource* source = getSourceProcessor();
        if (source == 0) return;
        source->getPort("volumecollection")->invalidatePort();
}

// private methods
//
void DensityMapManipulation::updateSelection() {
    DensityMapCoProcessor::updateSelection();
    const VolumeCollection* collection = getInputVolumeCollection();
    if (collection == 0) {
        LERROR("Collection is NULL at DensityMapManipulation::updateSelection()");
        return;
    }
    volumeURLList_.clear();
    for (size_t i = 0; i < collection->size(); i++)
        volumeURLList_.addVolume(collection->at(i));
}
