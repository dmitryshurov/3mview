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

#include "commands_shgen.h"

#include "voreen/core/utils/cmdparser/commandlineparser.h"

#include "tgt/init.h"
#include "tgt/exception.h"

using namespace voreen;

int main(int argc, char** argv) {
    std::string loggerCat_ = "voreen.voltool";

    tgt::Singleton<tgt::LogManager>::init();

    //add a console logger:
    tgt::Log* clog = new tgt::ConsoleLog();
    clog->addCat("", true, tgt::Debug);
    LogMgr.addLog(clog);

    CommandlineParser cmdparser("Voltool");

    cmdparser.addCommand(new CommandSHGenBrdfMerl());
    cmdparser.addCommand(new CommandSHGenBrdfPhong());
    cmdparser.addCommand(new CommandSHGenLightScalar());
    cmdparser.addCommand(new CommandSHGenLightProbe());

    cmdparser.setCommandLine(argc, argv);
    cmdparser.execute();

    // Display the help if no argument is given
    if (argc == 1)
        cmdparser.displayHelp();

    return EXIT_SUCCESS;
}
