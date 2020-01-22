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
			class structure : public base<_Length>
			{
			public:
				using base_t = base<_Length>;
				using tag_type = base_t::tag_type;
				using length_type = base_t::length_type;

				enum constants : byte
				{
					constructed_bit = 0x20,
				};

				structure(const tag_type tag) :
					base_t(tag | constructed_bit),
					begin_(nullptr),
					end_(nullptr),
					dirty_length_(true)
				{}

				void set_tag(const tag_type item)
				{
					base_t::tag_ = item | constructed_bit;
				}

				virtual length_type content_length() const
				{
					if (dirty_length_)
					{
						dirty_length_ = false;
						length_type len = 0;
						for (auto i = begin_; i != nullptr; i = i->next_)
						{
							len += i->total_length();
						}

						base_t::length_ = base_t::length_encoder(len);
					}

					return base_t::length_.value();
				}

				structure& operator << (base_t& item)
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
					if (!dirty_length_)
					{
						dirty_length_ = true;
					}

					return *this;
				}

			protected:
				virtual error_length_pair internal_encode(byte* buffer, byte* const buffer_end) const
				{
					byte* buf = buffer;
					error_t encoding_error;
					length_type used;
					for (auto i = begin_; i != nullptr; i = i->next_, buf += used)
					{
						std::tie(encoding_error, used) = i->encode_to(buf, buffer_end);
						if (encoding_error != error_t::none)
						{
							break;
						}
					}

					return std::make_pair(encoding_error, static_cast<length_type>(buf - buffer));
				}

				const base_t* begin_;
				base_t* end_;
				mutable bool dirty_length_;
			};
		}
	}
}
