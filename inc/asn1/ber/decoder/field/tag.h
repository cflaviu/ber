#pragma once
#ifndef PCH
	#include <asn1/ber/decoder/field/simple.h>
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
				class tag : public simple<T>
				{
				protected:
					using base = simple<T>;

					enum constants : byte
					{
						constructed = 0x20,
						more_octets = 0x80,
						multiple_octets = 0x1F,
						mask = 0x7F,
					};

					content_t content_;
					class_t class_type_;

					const byte* first_read(const byte* ptr, const byte* const end)
					{
						const byte tagField = *ptr & multiple_octets;
						if (tagField == multiple_octets)
						{
							base::state_ = state_t::reading;
							base::length_ = 0;
							base::value_ = 0;
							ptr = read(ptr + 1, end);
						}
						else
						{
							base::length_ = 1;
							base::value_ = tagField;
							base::state_ = state_t::done;
						}

						class_type_ = class_t(*ptr >> 6);
						content_ = content_t((*ptr & constructed) != 0);
						return ptr + 1;
					}

					const byte* read(const byte* ptr, const byte* const end)
					{
						for (; ptr != end; ++ptr)
						{
							if (++base::length_ > sizeof(base::value_type))
							{
								base::set_error(error_t::field_too_big);
								break;
							}

							base::value_ = (base::value_ << 7) | (*ptr & mask);
							if ((*ptr & more_octets) == 0)
							{
								base::state_ = state_t::done;
								break;
							}
						}

						return ptr;
					}

				public:
					class_t class_type() const { return class_type_; }
					content_t content_type() const { return content_; }

					tag() :
						class_type_(class_t::universal),
						content_(content_t::primitive)
					{}

					void reset()
					{
						base::reset();
						class_type_ = class_t::universal;
						content_ = content_t::primitive;
					}
				};
			}
		}
	}
}
