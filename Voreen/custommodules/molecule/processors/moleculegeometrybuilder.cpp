#include "moleculegeometrybuilder.h"

#include "../../geometry/utils/primitivegeometrybuilder.h"

#include "tgt/vector.h"
using tgt::vec3;

#include <iostream>

#define REBUILD_MOLECULE_ACTION CallMemberAction<MoleculeGeometryBuilder>(this, &MoleculeGeometryBuilder::rebuildMolecule)

MoleculeGeometryBuilder::MoleculeGeometryBuilder()
  : Processor()
  , inport_(Port::INPORT, "molecule", "Molecule Input")
  , outport_(Port::OUTPORT, "geometry", "Geometry Output")
  , repType_("repType", "Representation")
  , traceTangentLength_("traceTangentLength", "Tangent length", 1.5f, 1.f, 2.f)
  , traceCylinderRadius_("traceCylinderRadius", "Trace radius", 0.1f, 0.01f, 0.3f)
  , traceNumCylinderSides_("traceNumCylinderSides", "Cylinder side count", 5, 2, 12)
  , traceNumSteps_("traceNumSteps", "Step count", 4, 1, 12)
  , showCoords_("showCoords", "Show coords", false)
  , atomRadius_("atomRadius", " Atoms radius", 0.1f, 0.01f, 1.0f)
  , bondRadius_("bondsRadius", " Bond radius", 0.1f, 0.01f, 1.0f)
  , bondAndAtomRes_("bondsCylinderSides", " Atoms and bonds resolution", 3, 5, 8)
  
{
    addPort(inport_);
    addPort(outport_);
    
    addProperty(repType_);
    addProperty(traceTangentLength_);
    addProperty(traceCylinderRadius_);
    addProperty(traceNumCylinderSides_);
    addProperty(traceNumSteps_);
    addProperty(showCoords_);
    addProperty(bondAndAtomRes_);
    addProperty(atomRadius_);
    addProperty(bondRadius_);
    
    repType_.addOption("BallsAndSticks", "Balls and sticks");
    repType_.addOption("RibbonsRep", "Ribbons");
    
    repType_.onChange(REBUILD_MOLECULE_ACTION);
    traceTangentLength_.onChange(REBUILD_MOLECULE_ACTION);
    traceCylinderRadius_.onChange(REBUILD_MOLECULE_ACTION);
    traceNumCylinderSides_.onChange(REBUILD_MOLECULE_ACTION);
    traceNumSteps_.onChange(REBUILD_MOLECULE_ACTION);
    showCoords_.onChange(REBUILD_MOLECULE_ACTION);
    bondRadius_.onChange(REBUILD_MOLECULE_ACTION);
    atomRadius_.onChange(REBUILD_MOLECULE_ACTION);
    bondAndAtomRes_.onChange(REBUILD_MOLECULE_ACTION);
	
	// Create empty data to make this outport valid. Take ownership is true because
	// we want the data to be automatically deleted when replaced at the next setData() call
	outport_.setData(new MeshListGeometry(), true);
}

void MoleculeGeometryBuilder::process() {
	// TODO Check for memory leaks and unnecessary rebuildung of geometry

	// We don't check if this port is valid because we assume that the source
	// always contains molecule data structure (even if it does not contain any data)

	// If inport is not connected or nothing changed since the last call then do nothing
	if (!inport_.isReady() || !inport_.hasChanged()) return;
	
	// Port is ready and input data has changed
    rebuildMolecule();
}

void MoleculeGeometryBuilder::rebuildMolecule() {
	try {
	    const Molecule* mol = inport_.getData();
	    if (!mol) return;
	    
        MeshGeometry* geom;

        // TODO Add @repType as a parameter to the Molecule class
        /**/ if (repType_.get() == "atomsAndBonds")
            geom = buildAtomsAndBondsGeometry(mol);
        else if (repType_.get() == "backboneTrace")
            geom = buildBackboneTraceGeometry(mol);
        
        tgt::mat4 transform = tgt::mat4::createIdentity();
        transform = mol->getTransformationMatrix();
        std::cout << transform << std::endl;
        
        geom->transform(transform);
        // Delete old data and set new
        outport_.setData(geom);
    }
    catch (...) {
        LERROR("Error at MoleculeGeometryBuilder::process()");
    }
}

vec3 MoleculeGeometryBuilder::getAtomColor(int a) {
    vec3 color(1.f, 0.1f, 0.7f);

         if (a==1)  color = vec3(0.9f, 0.9f, 0.9f);
    else if (a==6)  color = vec3(0.3f, 0.3f, 0.3f);
    else if (a==8)  color = vec3(0.8f , 0.f, 0.f);
    else if (a==7)  color = vec3(0.3f, 0.3f, 1.f); 
    else if (a==15) color = vec3(1.f,0.6f,0.f);
    else if (a==16) color = vec3(1.f,0.8f,0.2f); 
    else if (a==17) color = vec3(0.f,0.9f,0.f); 
    else if (a==11) color = vec3(0.f,0.f,0.9f); 
    else if (a==12) color = vec3(0.1f,0.5f,0.1f); 
    else if (a==20) color = vec3(0.5f,0.5f,0.5f); 

    return color;
}

MoleculeGeometry* MoleculeGeometryBuilder::buildAtomsAndBondsGeometry(const Molecule* molecule) {
    tgtAssert(molecule, "molecule parameter is NULL at MoleculeCollectionGeometryBuilder::buildAtomsAndBondsGeometry()");
    
    MoleculeGeometry* moleculeGeometry = new MoleculeGeometry(molecule);
    
    BallsAndSticksRep* rep = dynamic_cast<BallsAndSticksRep*>(molecule->getRepresentation());
    
    float radius = rep->atomsRadius();
    float bondRadius = rep->bondsRadius();
    size_t steps = rep->atomsResolution();
    size_t bondSteps = rep->bondsResolution();
    
    for (size_t i = 0; i < molecule->numAtoms(); i++) {
        Atom* a = molecule->atom(i);
        tgt::vec3 atomCoords(a->x(), a->y(), a->z());
        tgt::vec3 acolor = getAtomColor(a->atomicNumber());
        
        for(int i=0; i < steps; i++) {
            float x1 = 2*i/float(steps) -1;
            float x2 = 2*(i+1)/float(steps) -1;
            float R1 = radius * sqrt(1 - x1*x1);
            float R2 = radius * sqrt(1 - x2*x2);
            tgt::vec3 v1( a->x() + radius*x1, a->y(), a->z());
            tgt::vec3 v2( a->x() + radius*x2, a->y(), a->z());
            
            MeshGeometry cone = PrimitiveGeometryBuilder::createConeCylinder(v1,v2,R1,R2,steps,acolor,true);
            moleculeGeometry->addMesh(cone);
        }
        //MeshGeometry cube = MeshGeometry::createCube(atomCoords - diag, atomCoords + diag,
        //                                                   tex1, tex2, getAtomColor(a->GetAtomicNum()), getAtomColor(a->GetAtomicNum()));
        
        //geometry->addMesh(cube);
    }
    
    // Draw bonds with cylinders 2 cylinders for 1 bond
    for (size_t i = 0; i < molecule->numBonds(); i++) {
        Bond* bond = molecule->bond(i);
        Atom* a1 = bond->atom1();
        Atom* a2 = bond->atom2();
        tgt::vec3 atom1Coords(a1->x(), a1->y(), a1->z());
        tgt::vec3 atomMidCoords(a1->x() + (a2->x() - a1->x())/2, a1->y() + (a2->y() - a1->y())/2, a1->z() + (a2->z() - a1->z())/2);
        tgt::vec3 atom2Coords(a2->x(), a2->y(), a2->z());
        
        MeshGeometry cyl1 = PrimitiveGeometryBuilder::createCylinder(atom1Coords, atomMidCoords, bondRadius, bondSteps, getAtomColor(a1->atomicNumber()));
        MeshGeometry cyl2 = PrimitiveGeometryBuilder::createCylinder(atom2Coords, atomMidCoords, bondRadius, bondSteps, getAtomColor(a2->atomicNumber()));
        moleculeGeometry->addMesh(cyl1);
        moleculeGeometry->addMesh(cyl2);
    }
    
    return moleculeGeometry;
}

MoleculeGeometry* MoleculeGeometryBuilder::buildBackboneTraceGeometry(const Molecule* molecule) {
    MoleculeGeometry* moleculeGeometry = new MoleculeGeometry(molecule);
  
    tgtAssert(molecule, "molecule parameter is NULL at MoleculeCollectionGeometryBuilder::buildBackboneTraceGeometry()");
    std::vector<PolyLine> backbone;
    
    std::vector<tgt::vec3> chainColors;
    chainColors.push_back(tgt::vec3(1, 0.4, 0.4));
    chainColors.push_back(tgt::vec3(0.4, 1, 0.4));
    chainColors.push_back(tgt::vec3(0.4, 0.4, 1));
    chainColors.push_back(tgt::vec3(0, 1, 1));
    chainColors.push_back(tgt::vec3(1, 0, 1));
    chainColors.push_back(tgt::vec3(1, 1, 0));
    
    size_t numResidues = molecule->numResidues();
    if (molecule->numResidues() < 2) return moleculeGeometry;
    
    size_t currentChainNum = 0;
    
	// Read backbone of each chain to a separate PolyLine
    for (size_t i = 0; i < numResidues; i++) {
        
        //OBResidue* res = mol.GetResidue(i);
        MoleculeResidue* res = molecule->residue(i);
        
        const std::vector<Atom*> atoms = res->atoms();
        const std::vector<std::string> atomIDs = res->atomIDs();
        size_t residueChainNum = res->chainNum();
        
        if (residueChainNum > currentChainNum) {
            backbone.push_back(PolyLine());
            currentChainNum = residueChainNum;
        }
        
        size_t numAtoms = atoms.size();
        
        for (size_t i = 0; i < numAtoms; i++) {
            Atom* a = atoms[i];
            std::string atomID = atomIDs[i];
            
            atomID.erase(remove(atomID.begin(), atomID.end(), ' '), atomID.end());
            
            if (atomID.compare("CA") == 0)
                backbone.back().addVertex(tgt::vec3(a->x(),a->y(),a->z()));
        }
        
    }
    
	// For each backbone PolyLine
    for (size_t i = 0; i < backbone.size(); i++) {
		// Build ribbon trace
		
        PolyLine* smoothBackbone = backbone[i].interpolateBezier(traceNumSteps_.get(), traceTangentLength_.get());
        MeshListGeometry* lineGeometry = PrimitiveGeometryBuilder::createPolyLine(smoothBackbone, traceCylinderRadius_.get(), traceNumCylinderSides_.get(), chainColors[i]);
        
        moleculeGeometry->addMeshList(*lineGeometry);
		delete lineGeometry;
        
        // Build moving frame coords (just for testing)  
        if (showCoords_.get()) {  
            MeshListGeometry* coordsGeometry = PrimitiveGeometryBuilder::createPolyLineCoords(smoothBackbone, traceCylinderRadius_.get() * 0.5);
            moleculeGeometry->addMeshList(*coordsGeometry);
			delete coordsGeometry;
        }
    }
    
    return moleculeGeometry;
}
