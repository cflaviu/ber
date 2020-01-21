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
				class tag : public simple<uint8_t>
				{
				protected:
					using base = simple<uint8_t>;

					enum constants : byte
					{
						mask = 0x1F,
						constructed  = 0x20,
					};

					content_t content_;
					class_t class_type_;

					const byte* first_read(const byte* ptr, const byte* const end)
					{
						base::value_ = *ptr & mask;
						base::state_ = state_t::done;

						class_type_ = class_t(*ptr >> 6);
						content_ = content_t((*ptr & constructed) != 0);
						return ptr + 1;
					}

					const byte* read(const byte* /*ptr*/, const byte* const /*end*/)
					{
						return nullptr;
					}

				public:
					class_t class_type() const { return class_type_; }
					content_t content_type() const { return content_; }

					tag() :
						base(sizeof(byte)),
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
