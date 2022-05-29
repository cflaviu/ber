#pragma once
#ifndef PCH
    #include <asn1/ber/types.h>
#endif

namespace asn1::ber::decoder::field
{
    class tag
    {
    protected:
        enum constants : byte
        {
            mask = 0x1Fu,
            constructed = 0x20u,
        };

        class_t class_type_ {};
        content_t content_ {};
        byte id_ {};

    public:
        using type = tag;

        tag() noexcept = default;

        class_t class_type() const noexcept { return class_type_; }

        content_t content_type() const noexcept { return content_; }

        bool is_primitive() const noexcept { return content_type() == content_t::primitive; }

        bool is_constructed() const noexcept { return content_type() == content_t::constructed; }

        byte id() const noexcept { return id_; }

        error_t operator()(const byte*& ptr, const byte* const end) noexcept
        {
            error_t result = error_t::none;
            if (ptr != end)
            {
                const auto tag_byte = *ptr++;
                class_type_ = class_t(tag_byte >> 6);
                content_ = content_t((tag_byte & constructed) != 0);
                id_ = tag_byte & mask;
            }
            else
            {
                result = error_t::underflow;
            }

            return result;
        }

        void reset() noexcept
        {
            class_type_ = class_t::universal;
            content_ = content_t::primitive;
            id_ = 0u;
        }

        bool operator==(const tag& item) const noexcept = default;
        bool operator!=(const tag& item) const noexcept = default;
    };
}
