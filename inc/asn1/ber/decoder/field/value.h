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
				template <typename T = uint32_t>
				class value : public base
				{
				public:
					using length_type = T;

				protected:
					byte* buffer_;
					length_type buffer_size_;
					length_type length_;
					length_type remaining_;

					const byte* first_read(const byte* ptr, const byte* const end)
					{
						state_ = state_t::reading;
						return read(ptr, end);
					}

					const byte* read(const byte* ptr, const byte* const end)
					{
						const auto min_size = std::min(remaining_, static_cast<length_type>(end - ptr));
						std::memcpy(buffer_, ptr, min_size);
						ptr += min_size;
						remaining_ -= min_size;
						if (remaining_ == 0)
						{
							state_ = state_t::done;
						}

						return ptr;
					}

				public:
					value(byte* const buffer = nullptr, const length_type buffer_size = 0, const length_type length = 0) :
						buffer_(buffer),
						buffer_size_(buffer_size),
						length_(length),
						remaining_(length)
					{}

					length_type length() const { return length_; }
					const byte* buffer() const { return buffer_; }
					length_type buffer_size() const { return buffer_size_; }

					bool reset(const length_type length)
					{
						const bool good = (buffer_size_ >= length);
						if (good)
						{
							state_ = state_t::stopped;
							remaining_ = length_ = length;
						}
						else
						{
							set_error(error_t::field_too_big);
							remaining_ = length_ = 0;
						}

						return good;
					}

					bool reset(byte* const buffer, const length_type buffer_size, const length_type length)
					{
						buffer_ = buffer;
						buffer_size_ = buffer_size;
						return reset(length);
					}
				};
			}
		}
	}
}
