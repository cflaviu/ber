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
				class base
				{
				public:
					virtual ~base() = default;
				};
			}
		}
	}
}
