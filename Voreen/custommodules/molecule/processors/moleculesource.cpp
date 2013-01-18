#include "moleculesource.h"

#include <fstream>
#include <sstream>

MoleculeSource::MoleculeSource()
  : Processor()
  , outport_(Port::OUTPORT, "molecule", "Molecule Output")
  , inputFile_("inputFile", "Input file", "Load PDB", VoreenApplication::app()->getUserDataPath(), "*.pdb")
  , loadMolecule_("loadMolecule", "Load Molecule")
  , clearMolecule_("clearMolecule", "Clear Molecule")
{
    addPort(outport_);
    
    addProperty(inputFile_);
    addProperty(loadMolecule_);
    addProperty(clearMolecule_);
    
    loadMolecule_.onChange(CallMemberAction<MoleculeSource>(this, &MoleculeSource::readMolecule));
    
    Molecule *molecule = new Molecule();
    molecule->addAtom(tgt::vec3(0.f, 0.f, 0.f));
    molecule->addAtom(tgt::vec3(1.f, 0.f, 0.f));
    molecule->addAtom(tgt::vec3(0.f, 1.f, 0.f));
    molecule->addAtom(tgt::vec3(0.f, 0.f, 1.f));
    
    outport_.setData(molecule);
}

void MoleculeSource::readMolecule() {

    try {
        Molecule* molecule = loadMoleculeFromFile(inputFile_.get());
        tgtAssert(molecule, "null pointer returned (exception expected)");
        outport_.setData(molecule);
    }
    catch (VoreenException& e) {
        LERROR(e.what());
    }
}

Molecule* MoleculeSource::loadMoleculeFromFile(const std::string& filename)
    throw (VoreenException)
{
    std::ifstream stream;
    stream.open(filename.c_str(), std::ios_base::in);
    if (stream.fail())
        throw VoreenException("Failed to open file for reading: " + filename);

    Molecule* molecule = new Molecule();
    
    char buf[82];
	while (stream.getline(buf, 82)) {
	    if (std::string(buf).compare(0, 6, "ATOM  ") != 0)
	        continue;
		
		std::stringstream string(buf + 30);
		
		float x, y, z;
		string >> x >> y >> z;
		
		molecule->addAtom(tgt::vec3(x, y, z));
	}
    
    return molecule;
}