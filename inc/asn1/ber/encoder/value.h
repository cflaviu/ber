#pragma once
#ifndef PCH
	#include <asn1/ber/encoder/base.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace encoder
		{
			template <typename _Length = uint16_t>
			class value : public base<_Length>
			{
			public:
				using base_t = base<_Length>;
				using tag_type = base_t::tag_type;
				using length_type = base_t::length_type;

				value():
					buffer_(nullptr)
				{}

				value(const tag_type tag) :
					base_t(tag)
				{}

				value(const tag_type tag, const byte* value, const _Length value_size):
					base_t(tag, value_size),
					buffer_(value)
				{}

				virtual _Length content_length() const
				{
					return base_t::length_.value();
				}

				const byte* buffer() const
				{
					return buffer_;
				}

				void set_buffer(const byte* value, _Length value_size)
				{
					buffer_ = value;
					base_t::length_ = value_size;
				}

			protected:
				virtual error_length_pair internal_encode(byte* buffer, byte* const /*buffer_end*/) const
				{
					std::memcpy(buffer, buffer_, base_t::length_.value());
					return std::make_pair(error_t::none, base_t::length_.value());
				}

				const byte* buffer_;
			};
		}
	}
}
