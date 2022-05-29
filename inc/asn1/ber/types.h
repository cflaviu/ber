#pragma once
#ifndef PCH
#endif

namespace asn1
{
    using byte = unsigned char;
}

namespace asn1::ber
{
    enum /*class*/ tag_t : byte
    {
        end_of_content,
        boolean,
        integer,
        string_bit,
        string_octet,
        null,
        object_id,
        object_descriptor,
        instance_of,
        real,
        enumerated,
        embedded_pdv,
        string_utf8,
        object_id_relative,
        reserved1,
        reserved2,
        sequence,
        set,
        string_numeric,
        string_printable,
        string_teletex,
        string_videotext,
        string_ia5,
        time_utc,
        time_generalized,
        string_graphic,
        string_iso646,
        string_general,
        string_universal,
        string_character,
        string_bmp,
        COUNT
    };

    enum class class_t : byte
    {
        universal,
        application,
        context_specific,
        private_,
        COUNT
    };

    enum class content_t : byte
    {
        primitive,
        constructed,
        COUNT
    };

    const char* text_of(const tag_t item) noexcept;
    const char* text_of(const class_t item) noexcept;
    const char* text_of(const content_t item) noexcept;

    namespace decoder
    {
        enum class error_t : byte
        {
            none,
            underflow,
            overflow
        };
    }

    namespace encoder
    {
        enum class error_t : byte
        {
            none,
            overflow
        };
    }

    namespace internal
    {
        template <typename T, byte storage_bytes>
        struct bytes_needed;

        template <typename T>
        struct bytes_needed<T, 1u>
        {
            byte operator()(const T value) const noexcept { return 1u; }
        };

        template <typename T>
        struct bytes_needed<T, 2u>
        {
            byte operator()(const T value) const noexcept { return 1 + ((value & 0xFF00u) != 0u); }
        };

        template <typename T>
        struct bytes_needed<T, 4u>
        {
            byte operator()(const T value) const noexcept
            {
                const byte index = ((value & 0xFF00u) != 0u) | (((value & 0xFF0000u) != 0u) << 1u) | (((value & 0xFF000000u) != 0u) << 2u);
                static const byte sizes[] = {1u, 2u, 3u, 3u, 4u, 4u, 4u, 4u};
                return sizes[index];
            }
        };

        template <typename T>
        struct bytes_needed<T, 8u>
        {
            byte operator()(const T value) const noexcept
            {
                bytes_needed<T, 4u> bn;
                auto hight_part = static_cast<uint32_t>(value >> 32u);
                return (hight_part != 0u) ? 4u + bn(hight_part) : bn(static_cast<uint32_t>(value & 0xFFFFFFFFu));
            }
        };
    }

    template <typename T>
    byte bytes_needed(const T value) noexcept
    {
        return internal::bytes_needed<T, sizeof(T)>()(value);
    }
}
