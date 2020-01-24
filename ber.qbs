import qbs

Project {
    minimumQbsVersion: "1.7.1"

    property bool usePCH: false

    StaticLibrary {

        name : "ber"
        version: "0.1"

        Depends { name: 'cpp' }
        cpp.includePaths: ["inc"]
        cpp.cxxLanguageVersion: "c++17"
        cpp.enableExceptions: false
        cpp.enableRtti: false

        Group {
            name: "decoder"
            prefix: "inc/asn1/ber/decoder/"
            files: [
                "field/base.h",
                "field/length.h",
                "field/simple.h",
                "field/tag.h",
                "field/value.h",
                "basic.h",
                "engine.h",
                "printer.h",
            ]
        }

        Group {
            name: "encoder"
            prefix: "inc/asn1/ber/encoder/"
            files: [
                "field/base.h",
                "field/length.h",
                "field/tag.h",
                "base.h",
                "structure.h",
                "value.h"
            ]
        }

        Group {
            name: "common"
            files: [
                "inc/asn1/ber/types.h",
                "src/asn1/ber/types.cpp",
            ]
        }

        Group {
            name: "pch"
            condition: project.usePCH
            fileTags: ["cpp_pch"]
            files: [
                "inc/asn1/ber/pch.h",
                "src/asn1/ber/pch.cpp"
            ]
        }
    }
}
