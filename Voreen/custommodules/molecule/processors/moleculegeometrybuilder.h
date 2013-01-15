#ifndef VRN_MOLECULEGEOMETRYBUILDER_H
#define VRN_MOLECULEGEOMETRYBUILDER_H

#include "voreen/core/processors/processor.h"

#include "voreen/core/ports/geometryport.h"
#include "../ports/moleculeport.h"

using namespace voreen;

class MoleculeGeometryBuilder : public Processor {
public:
    MoleculeGeometryBuilder();

    // virtual constructor
    virtual Processor* create() const { return new MoleculeGeometryBuilder(); }

    // documentary functions
    virtual std::string getClassName() const { return "MoleculeGeometryBuilder";       }
    virtual std::string getCategory() const  { return "Geometry";      }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void setDescriptions() {
        setDescription("Build mesh geometry of a given molecule");
    }

    virtual void process();

private:
    // ports and properties
    MoleculePort inport_;
    GeometryPort outport_;

};

#endif // VRN_MOLECULEGEOMETRYBUILDER_H
