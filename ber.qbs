import qbs

Project {
    minimumQbsVersion: "1.7.1"
    property bool usePCH: false

    StaticLibrary {
        name : "ber"
        version: "0.1"
        Depends { name: 'cpp' }
        cpp.includePaths: ["inc"]
        cpp.cxxLanguageVersion: "c++20"
        cpp.enableExceptions: false
        cpp.enableRtti: false
        Group {
            name: "base"
            qbs.install: true
            qbs.installSourceBase: "inc/asn1/ber/"
            qbs.installDir: "/usr/local/include/ber/"
            prefix: "inc/asn1/ber/"
            files: [
                "decoder/tag_length.h",
                "decoder/tag_length_base.h",
                "types.h",
                "encoder/field/base.h",
                "encoder/field/length.h",
                "encoder/field/tag.h",
                "encoder/base.h",
                "encoder/structure.h",
                "encoder/value.h",
                "decoder/field/length.h",
                "decoder/field/tag.h",
                "decoder/printer.h",
            ]
        }
        Group {
            name: "extra"
            files: [
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

    CppApplication {
         name : "ber_test"
         version: "0.1"
         Depends { name: "ber" }
         cpp.includePaths: [
             "inc",
             "/usr/local/include"
         ]
         cpp.cxxLanguageVersion: "c++20"
         cpp.enableExceptions: false
         cpp.enableRtti: false
         files: [
            "tst/enc/encoder_test.cpp",
            "tst/dec/decoder_test.cpp",
            "tst/tester.h",
        ]
     }
}
