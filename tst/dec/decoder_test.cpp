#include "asn1/ber/pch.h"
#ifndef  PCH
	#include <string>
    #include <vector>
#endif
#include <asn1/ber/decoder/printer.h>
#include "../tester.h"
//#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <cassert>

namespace asn1 {
namespace ber {

namespace decoder {

using my_tester = decoder::tester<uint32_t>;

const my_tester::test_item_array input_items =
{
    { // test1
        // input
        { 0x01, 0x01, 0x01 },
        { // expected
            { { tag_t::boolean }, { 1 } }
        }
    },
    { // test2
        // input
        { 0x01, 0x01, 0xff },
        { // expected
            { { tag_t::boolean }, { 0xff } }
        }
    },
    { // test3
        // input
        { 0x11, 0x08, 0x02, 0x01, 0x80, 0x09, 0x03, 0x80, 0xFB, 0x05 },
        { // expected
            { { tag_t::set }, { 0x02, 0x01, 0x80, 0x09, 0x03, 0x80, 0xFB, 0x05 } }
        }
    },
    { // test4
        // input
        { 0x30, 0x13, 0x02, 0x01, 0x05, 0x16, 0x0e, 0x41, 0x6e, 0x79, 0x62, 0x6f, 0x64, 0x79, 0x20, 0x74, 0x68, 0x65, 0x72, 0x65, 0x3f },
        { // expected
            { { tag_t::sequence }, { 0x02, 0x01, 0x05, 0x16, 0x0e, 0x41, 0x6e, 0x79, 0x62, 0x6f, 0x64, 0x79, 0x20, 0x74, 0x68, 0x65, 0x72, 0x65, 0x3f } },
            { { tag_t::integer }, { 0x05 } },
            { { tag_t::string_ia5 }, { 0x41, 0x6e, 0x79, 0x62, 0x6f, 0x64, 0x79, 0x20, 0x74, 0x68, 0x65, 0x72, 0x65, 0x3f } }
        }
    },
    { // test5
        // input
        { 0x30, 0x19, 0x93, 0x0A, 0x4A, 0x6F, 0x68, 0x6E, 0x20, 0x53, 0x6D, 0x69, 0x74, 0x68, 0x93, 0x0B, 0x39, 0x38, 0x37, 0x20, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30 },
        { // expected
            { { tag_t::sequence }, { 0x93, 0x0A, 0x4A, 0x6F, 0x68, 0x6E, 0x20, 0x53, 0x6D, 0x69, 0x74, 0x68, 0x93, 0x0B, 0x39, 0x38, 0x37, 0x20, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30 } },
            { { tag_t::string_printable }, { 0x4A, 0x6F, 0x68, 0x6E, 0x20, 0x53, 0x6D, 0x69, 0x74, 0x68 } },
            { { tag_t::string_printable }, { 0x39, 0x38, 0x37, 0x20, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30 } }
        }
    }
};

void test()
{
    my_tester t;
    t.run(input_items);
}

}}}

int main()
{
    using namespace asn1::ber;
    assert(bytes_needed<uint64_t>(0xFF00000000000000) == 8);
    assert(bytes_needed<uint64_t>(0x1000000000000000) == 8);
    assert(bytes_needed<uint64_t>(0x00FF000000000000) == 7);
    assert(bytes_needed<uint64_t>(0x0010000000000000) == 7);
    assert(bytes_needed<uint64_t>(0x0000FF0000000000) == 6);
    assert(bytes_needed<uint64_t>(0x0000100000000000) == 6);
    assert(bytes_needed<uint64_t>(0x000000FF00000000) == 5);
    assert(bytes_needed<uint64_t>(0x0000001000000000) == 5);

    assert(bytes_needed<uint64_t>(0x00000000FF000000) == 4);
    assert(bytes_needed<uint64_t>(0x0000000010000000) == 4);
    assert(bytes_needed<uint64_t>(0x0000000000FF0000) == 3);
    assert(bytes_needed<uint64_t>(0x0000000000100000) == 3);
    assert(bytes_needed<uint64_t>(0x000000000000FF00) == 2);
    assert(bytes_needed<uint64_t>(0x0000000000001000) == 2);
    assert(bytes_needed<uint64_t>(0x00000000000000FF) == 1);
    assert(bytes_needed<uint64_t>(0x0000000000000010) == 1);
    assert(bytes_needed<uint64_t>(0x0000000000000000) == 1);

    assert(bytes_needed<uint32_t>(0xFF000000) == 4);
    assert(bytes_needed<uint32_t>(0x10000000) == 4);
    assert(bytes_needed<uint32_t>(0x00FF0000) == 3);
    assert(bytes_needed<uint32_t>(0x00100000) == 3);
    assert(bytes_needed<uint32_t>(0x0000FF00) == 2);
    assert(bytes_needed<uint32_t>(0x00001000) == 2);
    assert(bytes_needed<uint32_t>(0x000000FF) == 1);
    assert(bytes_needed<uint32_t>(0x00000010) == 1);
    assert(bytes_needed<uint32_t>(0x00000000) == 1);

    assert(bytes_needed<uint16_t>(0xFF00) == 2);
    assert(bytes_needed<uint16_t>(0x1000) == 2);
    assert(bytes_needed<uint16_t>(0x00FF) == 1);
    assert(bytes_needed<uint16_t>(0x0010) == 1);
    assert(bytes_needed<uint16_t>(0x0000) == 1);

    asn1::ber::decoder::test();
}
