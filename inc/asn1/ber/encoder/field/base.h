#pragma once
#ifndef PCH
	#include <asn1/ber/types.h>
	#include <cstdint>
#endif

namespace asn1
{
	namespace ber
	{
		namespace encoder
		{
			namespace field
			{
				template <typename T>
				class base
				{
				public:
					base(const T value, const byte length) : 
						value_(value),
						length_(length)
					{}

					virtual ~base() = default;

					T value() const { return value_; }

					byte bytes() const { return length_; }

					virtual byte* serialize_to(byte* buffer) const = 0;

				protected:
					T value_;
					byte length_;
				};
			}
		}
	}
}
