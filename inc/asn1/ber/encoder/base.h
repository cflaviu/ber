#pragma once
#ifndef PCH
	#include <asn1/ber/types.h>
	#include <asn1/ber/encoder/field/tag.h>
	#include <asn1/ber/encoder/field/length.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace encoder
		{
			template <typename _Length>
			class base
			{
			public:
				using tag_type = byte;
				using length_type = _Length;
				using error_length_pair = std::pair<error_t, _Length>;

				tag_type tag() const noexcept
				{
					return tag_.value();
				}

				void set_tag(const tag_type item) noexcept
				{
					tag_ = item;
				}

				virtual length_type content_length() const = 0;

				length_type total_length() const noexcept
				{
					auto result = content_length();
					result += tag_.bytes() + length_.bytes();
					return result;
				}

				error_length_pair encode_to(byte* buffer, byte* const buffer_end) const noexcept
				{
					error_length_pair result;
					if (total_length() <= std::distance(buffer, buffer_end))
					{
						buffer = tag_.serialize_to(buffer);
						buffer = length_.serialize_to(buffer);
						result = internal_encode(buffer, buffer_end);  
						result.second += tag_.bytes() + length_.bytes();
					}
					else
					{
						result = std::make_pair(error_t::not_enough_space, 0);
					}

					return result;
				}

			protected:
				using tag_encoder = field::tag;
				using length_encoder = field::length<length_type>;

				template <typename Length>
				friend class structure;

				virtual error_length_pair internal_encode(byte* buffer, byte* const buffer_end) const = 0;

				base(const tag_type tag = 0, const length_type length = 0) noexcept :
					tag_(tag),
					length_(length),
					next_(nullptr)
				{}

				tag_encoder tag_;
				mutable length_encoder length_;
				const base* next_;
			};
		}
	}
}
