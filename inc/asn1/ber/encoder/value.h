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
			template <typename _Tag = uint32_t, typename _Length = uint32_t>
			class value : public base<_Length>
			{
			public:
				using base_t = base<_Length>;
				using error = base_t::error;

				value():
					buffer_(nullptr)
				{}

				value(const _Tag tag) :
					base_t(tag)
				{}

				value(const _Tag tag, const byte* value, const _Length value_size):
					base_t(tag, value_size),
					buffer_(value)
				{}

				const byte* buffer() const
				{
					return buffer_;
				}

				void set_buffer(const byte* value)
				{
					buffer_ = value;
				}

				void set_buffer(const byte* value, _Length value_size)
				{
					buffer_ = value;
					base_t::length_ = value_size;
				}

				virtual std::pair<error, _Length> encode_to(byte* buffer, _Length buffer_size) const
				{
					return std::make_pair(error::none, 10);
				}

			protected:
				const byte* buffer_;
			};
		}
	}
}
