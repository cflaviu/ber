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
                using input_data = byte_span;
                using expected_data = std::span<std::pair<tag_type, byte_span>>;
                using test_item = std::pair<input_data, expected_data>;
                using test_item_array = std::span<test_item>;

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
                        auto& expected = current_test_->second;
                        auto& expected_data = expected.second;
                        if (index_of_expected_item < expected_data.size())
                        {
                            error test_error;
                            if (tag_t(tag.value()) == expected.first)
                            {
                                error = std::equal(data, data + data_size, expected_data.data(), expected_data.data() + expected_data.size()) ?
                                    error::none : error::data_mismatch;
                            }
                            else
                            {
                                error = error::tag_mismatch;
                            }
                            
                            auto& results = test_results_array_.back();
                            results.push_back(error);

                            if (++index_of_expected_item == expected.second.size())
                            {
                                ++current_test_;
                                index_of_expected_item = 0;
                                test_results_array_.emplace_back(test_results());
                            }
                        }
                    }
                    else
                    {
                        std::cout << "no tests!";
                    }
                }

                void on_error(const byte decoder_id, const byte error, const byte* buffer, const _Length buffer_size) noexcept
                {
                    auto& results = test_results_array_.back();
                    results.push_back(error::decoding);
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

                void operator () (const byte* buffer, const byte* const buffer_end) noexcept
                {
                    for(std::pair<bool, const byte*> result; buffer != buffer_end; )
                    {
                        result = decoder_(buffer, buffer_end);
                        if (!decoder_.good())
                        {
                            break;
                        }

                        buffer = result.second;
                    }
                }
            };
        }
    }
}
