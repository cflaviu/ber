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
            protected:
                using _Engine = engine<tester, _Length>;
                using tag_type = byte;

                std::array<byte, _BufferSize> buffer_;
                _Engine decoder_;

                void on_data(const field::tag& tag, const byte* data, const _Length data_size)
                {
                    tag_t(tag.value());
                }

                void on_error(const byte decoder_id, const byte error, const byte* buffer, const _Length buffer_size)
                {
                }

                //friend _Engine;
            public:
                using input_array = std::vector<std::tuple<tag_type, _Length, const byte*>>;

                tester(std::ostream& output = std::cout) noexcept:
                    decoder_(buffer_.data(), _BufferSize, this)
                {}

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
