#pragma once
#ifndef PCH
    #include <asn1/ber/decoder/field/tag.h>
    #include <asn1/ber/decoder/field/length.h>
    #include <asn1/ber/decoder/tag_length_base.h>
#endif

namespace asn1
{
    namespace ber
    {
        namespace decoder
        {
           template <typename LengthType_ = uint16_t>
           using tag_length = tag_length_base<field::tag, field::length<LengthType_>>;
        }
    }
}
