#pragma once
#ifndef PCH
	#include <asn1/ber/field_decoder/base.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace field_decoder
		{
			template <typename T>
			class simple : public base
			{
			public:
				typedef T value_type;

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
