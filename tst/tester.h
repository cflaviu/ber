#pragma once
#ifndef PCH
    #include <asn1/ber/decoder/engine.h>
    #include <vector>
    #include <span>
#endif

namespace asn1
{
    namespace ber
    {
        namespace decoder
        {
            template <const size_t _BufferSize = 128, typename _Length = uint16_t>
            class tester
            {
            public:
                using tag_type = byte;
                using input_data = std::vector<byte>;
                using expected_data = std::vector<std::pair<tag_type, std::vector<byte> > >;
                using test_item = std::pair<input_data, expected_data>;
                using test_item_array = std::vector<test_item>;

                enum class error
                {
                    none,
                    tag_mismatch,
                    data_mismatch,
                    decoding
                };

                using ok_array = std::span<bool>;

            protected:
                using _Engine = engine<tester, _Length>;
                using test_results = std::vector<error>;
                using test_results_array = std::vector<test_results>;

                friend _Engine;

                test_item_array::const_iterator current_test_;
                test_item_array::const_iterator end_of_tests_;
                size_t index_of_expected_item;
                test_results_array test_results_array_;
                std::array<byte, _BufferSize> buffer_;
                _Engine decoder_;
                bool decoding_error_;
                bool mismatch_error_;

                void on_data(const field::tag& tag, const byte* data, const _Length data_size) noexcept
                {
                    if (current_test_ != end_of_tests_)
                    {
                        const input_data& input = current_test_->first;
                        const expected_data& output = current_test_->second;
                        if (index_of_expected_item < output.size())
                        {
                            auto& expected_pair = output[index_of_expected_item];
                            auto& expected_data = expected_pair.second;
                            const auto expected_tag = static_cast<tag_t>(expected_pair.first);
                            const auto input_tag = static_cast<tag_t>(tag.value());
                            error test_error;
                            if (input_tag == expected_tag)
                            {
                                std::cout << (int)input_tag << '\n';
                                print(std::cout, data, data_size) << '\n';
                                print(std::cout, expected_data.data(), expected_data.size()) << '\n' << '\n';

                                test_error = (data_size == expected_data.size()) &&
                                    std::equal(data, data + data_size, expected_data.data(), expected_data.data() + expected_data.size()) ?
                                    error::none : error::data_mismatch;
                            }
                            else
                            {
                                test_error = error::tag_mismatch;
                            }

                            auto& results = test_results_array_.back();
                            results.push_back(test_error);

                            std::cout << "test: " << (int)test_error << '\n';

                            if (++index_of_expected_item == output.size())
                            {
                                ++current_test_;
                                index_of_expected_item = 0;
                                test_results_array_.emplace_back(test_results());
                            }
                        }
                    }
                    else
                    {
                        std::cout << "no tests!\n";
                    }
                }

                void on_error(const byte decoder_id, const byte error, const byte* buffer, const _Length buffer_size) noexcept
                {
                    auto& results = test_results_array_.back();
                    results.push_back(error::decoding);
                }

                bool process(const byte* buffer, const byte* const buffer_end) noexcept
                {
                    for (std::pair<bool, const byte*> result; buffer != buffer_end; )
                    {
                        result = decoder_(buffer, buffer_end);
                        if (!decoder_.good())
                        {
                            break;
                        }

                        buffer = result.second;
                    }

                    return buffer != buffer_end;
                }

            public:
                tester(const test_item_array& input) noexcept:
                    current_test_(input.cbegin()),
                    end_of_tests_(input.cend()),
                    index_of_expected_item(0),
                    decoder_(buffer_.data(), _BufferSize, this),
                    decoding_error_(false),
                    mismatch_error_(false)
                {
                    test_results_array_.resize(1);
                }

                bool decoding_error() const noexcept { return decoding_error_; }

                bool mismatch_error() const noexcept { return mismatch_error_; }

                const test_results_array& results() const noexcept { return test_results_array_; }

                void run()
                {
                    while(current_test_ != end_of_tests_)
                    {
                        std::cout << "===================\n";
                        auto& input = current_test_->first;
                        print(std::cout, input.data(), input.size()) << '\n';
                        process(input.data(), input.data() + input.size());
                    }
                }
            };
        }
    }
}
