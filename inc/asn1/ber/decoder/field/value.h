#pragma once
#ifndef PCH
    #include <asn1/ber/decoder/field/base.h>
    #include <cstring>
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
					byte* buffer_end_;
					length_type length_;
					length_type remaining_;

                    const byte* first_read(const byte* ptr, const byte* const end) noexcept override
					{
						state_ = state_t::reading;
						return read(ptr, end);
					}

                    const byte* read(const byte* ptr, const byte* const end) noexcept override
					{
						const auto min_size = std::min(remaining_, static_cast<length_type>(std::distance(ptr, end)));
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
                    value(byte* const buffer = nullptr, byte* const buffer_end = nullptr, const length_type length = 0) noexcept :
						buffer_(buffer),
						buffer_end_(buffer_end),
						length_(length),
						remaining_(length)
					{}

                    length_type length() const noexcept { return length_; }
                    const byte* buffer() const noexcept { return buffer_; }
                    const byte* buffer_end() const noexcept { return buffer_end_; }
                    length_type buffer_size() const noexcept { return static_cast<length_type>(std::distance(buffer_, buffer_end_)); }

                    bool reset(const length_type length) noexcept
					{
						const bool good = (buffer_size() >= length);
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

                    bool reset(byte* const buffer, const byte* const buffer_end, const length_type length) noexcept
					{
						buffer_ = buffer;
						buffer_end_ = buffer_end;
						return reset(length);
					}
				};
			}
		}
	}
}
