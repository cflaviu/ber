#pragma once
#ifndef PCH
    #include <algorithm>
#endif

namespace asn1
{
	namespace ber
	{
        namespace decoder
		{
			namespace field
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
						type_bit = 0x80,
						multiple_octets = 0x1F,
						mask = 0x7F,
					};

                    value_type value_;

                public:
                    length() noexcept: value_(0) {}

                    value_type value() const noexcept { return value_; }

                    operator value_type () const noexcept { return value_; }

                    error_t operator () (const byte*& ptr, const byte* const end) noexcept
                    {
                        value_ = 0;
                        error_t result = error_t::none;
                        if (ptr != end)
                        {
                            auto first_byte = *ptr++;
                            auto length = first_byte & mask;
                            if ((length != 0) && (first_byte & type_bit) != 0) // definite long
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
				};
			}
		}
	}
}
