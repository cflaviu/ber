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
                using input_item_array = std::span<byte_span>;
                using expected_item_array = std::span<std::pair<tag_type, byte_span>>;
                using ok_array = std::span<bool>;

            protected:
                using _Engine = engine<tester, _Length>;

                const input_item_array* input_items_;
                const expected_item_array* expected_items_;
                ok_array ok_array_;
                std::array<byte, _BufferSize> buffer_;
                _Engine decoder_;
                _Length index_;
                bool decoding_error_;
                bool mismatch_error_;

                void on_data(const field::tag& tag, const byte* data, const _Length data_size) noexcept
                {
                    auto& expected = (*expected_items_)[index_];
                    auto& expected_data = expected.second;
                    bool ok = tag_t(tag.value()) == expected.first &&
                            std::equal(data, data + data_size, expected_data.data(), expected_data.data() + expected_data.size());
                    if (!ok)
                    {
                        if (!mismatch_error_)
                        {
                            mismatch_error_ = true;
                        }
                    }

                     ok_array_[index_++] = ok;
                }

                void on_error(const byte decoder_id, const byte error, const byte* buffer, const _Length buffer_size) noexcept
                {
                    if (!decoding_error_)
                    {
                        decoding_error_ = true;
                    }
                }

            public:
                tester(const input_item_array& input_items, const expected_item_array& expected_items, ok_array& oks) noexcept:
                    input_items_(&input_items),
                    expected_items_(&expected_items),
                    ok_array_(oks),
                    decoder_(buffer_.data(), _BufferSize, this),
                    index_(0),
                    decoding_error_(false),
                    mismatch_error_(false)
                {}

                bool decoding_error() const noexcept { return decoding_error_; }

                bool mismatch_error() const noexcept { return mismatch_error_; }

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
