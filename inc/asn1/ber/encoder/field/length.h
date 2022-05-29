#pragma once
#ifndef PCH
    #include <asn1/ber/encoder/field/base.h>
#endif

namespace asn1::ber::encoder::field
{
    template <typename T = uint16_t>
    class length: public base<T>
    {
    public:
        using base_t = base<T>;

        enum constants : byte
        {
            type_bit = 0x80,
            multiple_octets = 0x1F,
            mask = 0x7F,
        };

        static byte bytes_storing(const T value) noexcept { return value < 127u ? 1u : 1u + bytes_needed(value); }

        length(const T value) noexcept: base_t(value, bytes_storing(value)) {}

        byte* serialize_to(byte* buffer) const noexcept
        {
            if (buffer != nullptr)
            {
                if (base_t::value_ != 0u)
                {
                    if (base_t::length_ == 1u)
                    {
                        *buffer++ = static_cast<byte>(base_t::value_ & static_cast<T>(mask));
                    }
                    else
                    {
                        *buffer++ = type_bit | base_t::length_;
                        T value = base_t::value_;
                        for (auto bytes = base_t::length_ + 1u; bytes != 0u; --bytes, value >>= 8u)
                        {
                            *buffer++ = static_cast<byte>(value & 0xFFu);
                        }
                    }
                }
                else
                {
                    *buffer++ = 0;
                }
            }

            return buffer;
        }
    };
}
