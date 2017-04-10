import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "/home/pam/Programmierung/of_v0.9.8_linux64_release/libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: '/home/pam/Programmierung/of_v0.9.8_linux64_release'

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            "src/thirdparty/simplejson/JSONValue.cpp",
            "src/thirdparty/simplejson/JSONValue.h",
            "src/thirdparty/simplejson/JSON.cpp",
            "src/thirdparty/simplejson/JSON.h",
            "src/engine/director.cpp",
            "src/engine/director.h",
            "src/engine/fontmanager.cpp",
            "src/engine/fontmanager.h",
            "src/engine/node.cpp",
            "src/engine/node.h",
            "src/engine/node2d.cpp",
            "src/engine/nodetext.cpp",
            "src/engine/texturemanager.cpp",
            "src/engine/texturemanager.h",
            "src/game/leveldesc/layer.h",
            "src/game/leveldesc/layerobject.h",
            "src/game/leveldesc/level.cpp",
            "src/game/leveldesc/level.h",
            "src/game/leveldesc/levelloader.h",
            "src/game/leveldesc/levelloader.cpp",
            "src/game/leveldesc/objectdesc.h",
            "src/game/leveldesc/objectdesc.cpp",
            "src/game/gamedirector.cpp",
            "src/game/gamedirector.h",
            "src/game/gamescene.cpp",
            "src/game/gamescene.h",
            "src/game/worldstate.h",
            "src/main.cpp",
            "src/ofApp.cpp",
            "src/ofApp.h",
        ]

        of.addons: [

        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
        // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project

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

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
