#pragma once
#ifndef PCH
    #include <asn1/ber/decoder/field/length.h>
    #include <asn1/ber/decoder/field/tag.h>
#endif

namespace asn1::ber::decoder
{
    template <typename Tag_, typename Length_>
    class tag_length_base
    {
    public:
        using tag_type = typename Tag_::type;
        using lenght_type = typename Length_::type;

        tag_length_base() noexcept = default;
        ~tag_length_base() = default;

        const tag_type& tag() const noexcept { return tag_; }

        size_t length() const noexcept { return length_; }

        error_t operator()(const byte*& buffer_begin, const byte* const buffer_end) noexcept
        {
            auto result = tag_(buffer_begin, buffer_end);
            return (result == error_t::none) ? length_(buffer_begin, buffer_end) : result;
        }

    protected:
        Tag_ tag_ {};
        Length_ length_ {};
    };
}
