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
			class sequence: public base<_Tag, _Length>
			{
			public:
				typedef base<_Tag, _Length> base_t;
				using error = base_t::error;
				using tag_type = base_t::tag_type;
				using length_type = base_t::length_type;

				sequence(const _Tag tag = 0x22):
					base_t(tag),
					begin_(nullptr),
					end_(nullptr)
				{}

				sequence& operator << (base_t& item)
				{
					if (begin_ == nullptr)
					{
						begin_ = &item;
					}

					if (end_ != nullptr)
					{
						end_->next_ = &item;
					}

					end_ = &item;
					return *this;
				}

				virtual error encode_to(byte* buffer, _Length buffer_size) const
				{
					//tag_encoder tag_encoder;
					//length_encoder length_encoder;
					//value_encoder value_encoder;

					int count = 1;
					for (auto i = begin_; i != nullptr; i = i->next_)
					{
						std::cout << count++ << '\n';
					}

					return error::none;
				}

			protected:
				const base_t* begin_;
				base_t* end_;
			};
		}
	}
}
