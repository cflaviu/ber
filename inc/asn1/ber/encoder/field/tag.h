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
				enum constants : byte
				{
					constructed_bit = 0x20,
				};

				class tag : public base<byte>
				{
				public:
					using base_t = base<byte>;

					tag(const byte value) : 
						base_t(value, sizeof(byte))
					{}

					virtual byte* serialize_to(byte* buffer) const
					{
						if (buffer != nullptr)
						{
							auto val = base_t::value_;
							std::memcpy(buffer, static_cast<const byte*>(&val), sizeof(val));
							buffer += sizeof(val);
						}

						return buffer;
					}
				};
			}
		}
	}
}
