#pragma once
#ifndef PCH
    #include <algorithm>
    #include <asn1/ber/types.h>
#endif

namespace asn1::ber::decoder::field
{
    template <typename T = uint16_t>
    class length
    {
    public:
        using type = T;
        using value_type = T;

    protected:
        enum constants : byte
        {
            type_bit = 0x80u,
            multiple_octets = 0x1Fu,
            mask = 0x7Fu,
        };

        value_type value_ {};

    public:
        length() noexcept = default;

        value_type value() const noexcept { return value_; }

        operator value_type() const noexcept { return value_; }

        error_t operator()(const byte*& ptr, const byte* const end) noexcept
        {
            value_ = 0u;
            error_t result = error_t::none;
            if (ptr != end)
            {
                const auto first_byte = *ptr++;
                const auto length = first_byte & mask;
                if ((length != 0u) && ((first_byte & type_bit) != 0u)) // definite long
                {
                    if (length <= static_cast<byte>(sizeof(value_type)))
                    {
                        if (length <= std::distance(ptr, end))
                        {
                            auto value_end = ptr + length;
                            std::reverse_copy(ptr, value_end, reinterpret_cast<byte*>(&value_));
                            ptr = value_end;
                        }
                        else
                        {
                            result = error_t::underflow;
                        }
                    }
                    else
                    {
                        result = error_t::overflow;
                    }
                }
                else // definite short or indefinite
                {
                    value_ = length;
                }
            }
            else
            {
                result = error_t::underflow;
            }

            return result;
        }

        bool operator==(const length& item) const noexcept = default;
        bool operator!=(const length& item) const noexcept = default;
    };
}
