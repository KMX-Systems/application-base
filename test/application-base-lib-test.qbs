import qbs

CppApplication {
    name: "application-base-lib-test"
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    cpp.includePaths: ["api", "inc", "inc_dep"]
    //cpp.cxxFlags: "-fsanitize=address"
    //cpp.staticLibraries: "asan"
    cpp.staticLibraries: [
            "application-base-lib",
        ]
    Group {
        name: "test/hpp"
        prefix: "inc/kmx/application"
    }
    Group {
        files: [
        ]
        name: "test/cpp"
        prefix: "src/kmx/application"
    }
    Group {
        files: [
            "main.cpp",
        ]
        name: "test/main"
        prefix: "src/"
    }
}
