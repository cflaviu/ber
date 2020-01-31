#pragma once
#ifndef PCH
#include <asn1/ber/decoder/field/base.h>
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
				class length : public simple<T>
				{
				protected:
					using base = simple<T>;

					enum constants : byte
					{
						type_bit = 0x80,
						multiple_octets = 0x1F,
						mask = 0x7F,
					};

                    const byte* first_read(const byte* ptr, const byte* const end) noexcept override
					{
						base::state_ = state_t::reading;
						if ((*ptr & type_bit) == 0) // definite short
						{
							base::length_ = 1;
							base::value_ = *ptr & mask;
							base::state_ = state_t::done;
							++ptr;
						}
						else
						{
							base::length_ = *ptr & mask;
							if (base::length_ != 0) // definite long
                            {
                                if (base::length_ <= static_cast<int8_t>(sizeof(typename base::value_type)))
								{
									base::value_ = 0;
									ptr = read(ptr, end);
								}
								else
								{
									base::set_error(error_t::field_too_big);
								}
							}
							else // indefinite
							{
								base::length_ = -1;
								base::value_ = 0;
								base::state_ = state_t::done;
								++ptr;
							}
						}

						return ptr;
					}

                    const byte* read(const byte* ptr, const byte* const end) noexcept override
					{
						auto len = base::length_;
						for (; ptr != end && len != 0; ++ptr, --len)
						{
							base::value_ = (base::value_ << 8) | *ptr;
						}

						if (len == 0)
						{
							base::state_ = state_t::done;
						}

						return ptr;
					}
				};
			}
		}
	}
}
