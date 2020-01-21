#pragma once
#ifndef PCH
	#include <asn1/ber/encoder/field/base.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace encoder
		{
			namespace field
			{
				template <typename T = uint16_t>
				class length : public base<T>
				{
				public:
					using base_t = base<T>;

					enum constants : byte
					{
						type_bit = 0x80,
						multiple_octets = 0x1F,
						mask = 0x7F,
					};

					static byte bytes_storing(const T value)
					{
						return 1 + (value < 127 ? 0 : bytes_needed(value));
					}

					length(const T value) : 
						base_t(value, bytes_storing(value))
					{}

					byte* serialize_to(byte* buffer) const
					{
						if (buffer != nullptr)
						{
							if (base_t::value_ != 0)
							{
								if (base_t::length_ == 1)
								{
									*buffer++ = static_cast<byte>(base_t::value_ & static_cast<T>(mask));
								}
								else
								{
									*buffer++ = type_bit | base_t::length_;
									T value = base_t::value_;
									for(auto bytes = base_t::length_ + 1; bytes != 0; --bytes, value >>= 8)
									{
										*buffer++ = static_cast<byte>(value & 0xFF);
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
		}
	}
}
