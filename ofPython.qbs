import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "/home/arnaud/src/openframeworks/libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '/home/arnaud/src/openframeworks'

    ofApp {
        name: { return FileInfo.baseName(sourceDirectory) }

        files: [
            "bin/openframeworks.py",
            "bin/tester.py",
            "src/glmBindings.cpp",
            "src/main.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
            "src/ofBindings.cpp",
            "src/ofBindings.h",
            "src/ofpy.cpp",
            "src/ofpy.h",
        ]

        of.addons: [

        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: ['python3']       // list of additional system pkgs to include
        of.includePaths: ['/usr/include/python3.6m', '/usr/include/x86_64-linux-gnu/python3.6m']     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: ['-Wno-error=old-style-cast']         // flags passed to the c++ compiler
        of.linkerFlags: ['-lpython3.6m']      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
        // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project
        of.staticLibraries: []  // static libraries
        of.dynamicLibraries: [] // dynamic libraries

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    property bool makeOF: true  // use makfiles to compile the OF library
    // will compile OF only once for all your projects
    // otherwise compiled per project with qbs

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
