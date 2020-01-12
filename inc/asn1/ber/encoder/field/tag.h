#pragma once
#ifndef PCH
	#include <asn1/ber/encoder/field/simple.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace encoder
		{
			namespace field
			{
				template <typename T = uint32_t>
				class tag : public simple<T>
				{
				public:
				};
			}
		}
	}
}
