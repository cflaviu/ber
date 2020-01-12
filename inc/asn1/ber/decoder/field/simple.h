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
				template <typename T>
				class simple : public base
				{
				public:
					using value_type = T;

				protected:
					value_type value_;
					int8_t length_;

				public:
					simple() :
						value_(0),
						length_(0)
					{}

					value_type value() const { return value_; }

					int8_t length() const { return length_; }
					int8_t length2() const { return length_; }

					void reset()
					{
						base::reset();
						value_ = 0;
						length_ = 0;
					}
				};
			}
		}
	}
}
