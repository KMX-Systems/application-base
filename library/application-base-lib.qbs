import qbs

StaticLibrary {
    name: "application-base-lib"
    Depends {
        name: "cpp"
    }
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    cpp.includePaths: ["api", "inc", "inc_dep"]
    //cpp.cxxFlags: "-fsanitize=address"
    //cpp.staticLibraries: "asan"
    Group {
        name: "api/hpp"
        prefix: "api/kmx/application/"
        files: [
            "base.hpp",
        ]
    }
    Group {
        name: "lib/hpp"
        prefix: "inc/kmx/application/"
        files: [
        ]
    }
    Group {
        name: "lib/cpp"
        prefix: "src/kmx/application/"
        files: [
            "base.cpp",
        ]
    }
}
