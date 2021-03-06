#include "densitymapcoprocessor.h"

const std::string DensityMapCoProcessor::loggerCat_("3MTK.DensityMap.DensityMapCoProcessor");

DensityMapCoProcessor::DensityMapCoProcessor()
    : Processor()
    , inport_(Port::INPORT, "DensityMapSource", "Density Map Source")
{
    addPort(inport_);
}

Processor* DensityMapCoProcessor::create() const {
    return new DensityMapCoProcessor();
}

DensityMapCollectionSource* DensityMapCoProcessor::getSourceProcessor() const {
    std::vector<Processor*> processors = inport_.getConnectedProcessors();
    if (processors.size() == 0) {
        LWARNING("Processors.size() is 0 at DensityMapCoProcessor::getSourceProcessor()");
        return 0;
    }
    
    Processor* processor = processors[0];
    
    if (processor == 0) {
        LWARNING("Processor is NULL at DensityMapCoProcessor::getSourceProcessor()");
        return 0;
    }
    
    if (typeid(*processor) != typeid(DensityMapCollectionSource)) {
        LWARNING("Processor is not a DensityMapCollectionSource at DensityMapCoProcessor::getSourceProcessor()");
        return 0;
    }
    
    return dynamic_cast<DensityMapCollectionSource*>(processor);
}

const VolumeCollection* DensityMapCoProcessor::getInputVolumeCollection() const {
    DensityMapCollectionSource* source = getSourceProcessor();
    if (source == 0) {
        LWARNING("Source is NULL at DensityMapCoProcessor::getInputVolumeCollection()");
        return 0;
    }
    
    return source->getSelectedVolumeCollection();
}
