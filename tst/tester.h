#pragma once
#ifndef PCH
    #include <asn1/ber/decoder/printer.h>
    #include <asn1/ber/decoder/tag_length.h>
    #include <span>
    #include <vector>
#endif

namespace asn1::ber::decoder
{
    template <typename _Length = uint16_t>
    class tester
    {
    protected:
        tag_length<_Length> decoder_;

    public:
        using tag_type = byte;
        using input_data = std::vector<byte>;
        using expected_items = std::vector<std::pair<tag_type, std::vector<byte>>>;
        using test_item = std::pair<input_data, expected_items>;
        using test_item_array = std::vector<test_item>;

        void run(const test_item_array& input)
        {
            for (size_t no = 0; no != input.size(); ++no)
            {
                std::cout << "test " << no + 1
                          << " =====================================================================================\n";
                const test_item& test = input[no];
                auto& input = test.first;
                auto& expected_items = test.second;
                auto buffer = input.data();
                auto buffer_end = input.data() + input.size();

                bool ok = true;
                size_t item_index = 0;
                for (auto expected_item = expected_items.begin(); ok && expected_item != expected_items.end();
                     ++expected_item, ++item_index)
                {
                    std::cout << '[' << item_index << ']' << " input: ";
                    print_buffer(std::cout, buffer, std::distance(buffer, buffer_end)) << '\n';

                    std::cout << '[' << item_index << ']' << ' ';
                    auto error = decoder_(buffer, buffer_end);
                    if (error == error_t::none)
                    {
                        auto& tag = decoder_.tag();
                        if (tag.id() == expected_item->first)
                        {
                            auto length = decoder_.length();
                            auto& exp_buffer = expected_item->second;
                            if (std::equal(buffer, buffer + length, exp_buffer.data(), exp_buffer.data() + exp_buffer.size()))
                            {
                                std::cout << "OK\n";
                                if (tag.is_primitive())
                                {
                                    buffer += length;
                                }
                            }
                            else
                            {
                                std::cout << "FAILED - value mismatch\n";
                                std::cout << "expected: ";
                                print_buffer(std::cout, exp_buffer.data(), exp_buffer.size()) << '\n';
                                std::cout << "decoded : ";
                                print_buffer(std::cout, buffer, length) << '\n';
                                ok = false;
                            }
                        }
                        else
                        {
                            std::cout << "FAILED - tag mismatch\n";
                            std::cout << "expected: 0x" << std::hex << unsigned(expected_item->first) << '\n';
                            std::cout << "decoded : 0x" << std::hex << unsigned(tag.id()) << '\n';
                            ok = false;
                        }
                    }
                    else
                    {
                        std::cout << "decoding error " << unsigned(error) << '\n';
                        ok = false;
                    }
                }

                std::cout << (ok ? "PASSED" : "FAILED") << '\n';
            }
        }
    };
}
