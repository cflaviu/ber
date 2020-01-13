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
				template <typename T = int32_t>
				class length : public base<T>
				{
				public:
					using base_t = base<T>;

					length(const T value) : base_t(value) {}
				};
			}
		}
	}
}
