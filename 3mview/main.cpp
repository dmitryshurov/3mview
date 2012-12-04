#include <QtGui>
#include <QDebug>
#include <QDesktopServices>
#include <QGLFormat>

#include "MainWindow.h"
#include "DataLoaderWidget.h"
#include "Molecule/MoleculeListWidget.h"
#include "DensityMap/DensityMapList.h"
#include "DensityMap/DensityMapListWidget.h"
#include "ModulesWidget.h"
#include "OpenStructure.h"
#include "Stereo.h"
#include "ObjectDispatcher.h"

#include "ost/base/platform.hh"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	MainWindow mainWidow;
	mainWidow.show();



    // TODO: РАЗОБРАТЬСЯ!
    ost::SetPrefixPath("/");

// ---- Создание управляющих модулей и окон ---- //
    MoleculeList* ml = new MoleculeList();
    DensityMapList* dl = new DensityMapList();

    MoleculeListWidget moleculeList(ml);
    moleculeList.show();

    DensityMapListWidget densityMapList(dl);
    densityMapList.show();

    DataLoaderWidget dataLoader(ml, dl);
    dataLoader.show();

    ModulesWidget modulesWidget;
    modulesWidget.show();

    ObjectDispatcher d;
// --------------------------------------------- //

	ost::gui::GLWin* gl_win;
	QWidget* parent = &mainWidow;
    QGLFormat f;
    f.setOverlay(true);
    f.setStereo(true);

	ost::gui::GLCanvas ostGL(gl_win, parent, f);
	ostGL.resize(mainWidow.width(), mainWidow.height());
	ostGL.show();

	ost::gfx::Scene& scene = ost::gfx::Scene::Instance();
	scene.SetBackground(ost::gfx::Color(0.1, 0.1, 0.5));
	scene.SetShowCenter(true);

    Stereo stereo;
    stereo.activate();

    //ml->loadFromFile("/home/dimax/4INS.pdb");
    //dl->createNew("Map1", 200, 200, 200);

   return a.exec();
}
