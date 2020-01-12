#pragma once
#ifndef PCH
	#include <asn1/ber/types.h>
	#include <asn1/ber/encoder/field/tag.h>
	#include <asn1/ber/encoder/field/length.h>
	#include <asn1/ber/encoder/field/value.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace encoder
		{
			template <typename _Tag = uint32_t, typename _Length = uint32_t>
			class base
			{
			public:
				using tag_type = _Tag;
				using length_type = _Length;

				enum class error
				{
					none,
				};

				_Tag tag() const
				{
					return tag_;
				}

				void set_tag(const _Tag item)
				{
					tag_ = item;
				}

				virtual error encode_to(byte* buffer, _Length buffer_size) const = 0;

			protected:
				using tag_encoder = field::tag<tag_type>;
				using length_encoder = field::length<length_type>;
				using value_encoder = field::value;

				template <typename _Tag, typename _Length>
				friend class sequence;

				base(const tag_type tag = 0) : next_(nullptr), tag_(tag) {}

				const base* next_;
				tag_type tag_;
			};
		}
	}
}
