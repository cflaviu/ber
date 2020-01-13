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
				template <typename T = int32_t>
				class base
				{
				public:
					base(const T value) : value_(value) {}
					virtual ~base() = default;

					T value() const { return value_; }
					void set_value(const T item) { value_ = item; }

				protected:
					T value_;
				};
			}
		}
	}
}
