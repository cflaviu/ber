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
					buffer_(nullptr),
					buffer_size_(0)
				{}

				value(const _Tag tag, const byte* value, const _Length value_size):
					base_t(tag),
					buffer_(value),
					buffer_size_(value_size)
				{}

				const byte* buffer() const
				{
					return buffer_;
				}

				_Length buffer_size() const
				{
					return buffer_size_;
				}

				void set_buffer(const byte* value, _Length value_size)
				{

				}

				virtual error encode_to(byte* buffer, _Length buffer_size) const
				{
					//tag_encoder tag_encoder;
					//length_encoder length_encoder;
					//value_encoder value_encoder;

					return error::none;
				}

			protected:
				const byte* buffer_;
				_Length buffer_size_;
			};
		}
	}
}
