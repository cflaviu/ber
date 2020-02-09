#pragma once
#ifndef PCH
#endif

namespace asn1
{
	using byte = unsigned char;

	namespace ber
	{
		enum /*class*/ tag_t: byte
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

		enum class class_t: byte
		{
			universal,
			application,
			context_specific,
			private_,
			COUNT
		};

		enum class content_t: byte
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
                overflow,
                wrong_field_size
            };
        }

        namespace encoder
        {
            enum class error_t : byte
            {
                none,
                wrong_field_size,
                field_too_big,
                not_enough_space
            };
        }

        namespace internal
        {
            template <typename T, byte storage_bytes>
            struct bytes_needed;

            template <typename T>
            struct bytes_needed<T, 1>
            {
                byte operator () (const T value) const noexcept
                {
                    return 1;
                }
            };

            template <typename T>
            struct bytes_needed<T, 2>
            {
                byte operator () (const T value) const noexcept
                {
                    return 1 + ((value & 0xFF00) != 0);
                }
            };

            template <typename T>
            struct bytes_needed<T, 4>
            {
                byte operator () (const T value) const noexcept
                {
                    byte index =    ((value  & 0xFF00) != 0) |
                                    (((value & 0xFF0000) != 0) << 1) |
                                    (((value & 0xFF000000) != 0) << 2);
                    static const byte sizes[] = { 1, 2, 3, 3, 4, 4, 4, 4 };
                    return sizes[index];
                }
            };

            template <typename T>
            struct bytes_needed<T, 8>
            {
                byte operator () (const T value) const noexcept
                {
                    bytes_needed<T, 4> bn;
                    auto hight_part = static_cast<uint32_t>(value >> 32);
                    return (hight_part != 0) ? 4 + bn(hight_part) : bn(static_cast<uint32_t>(value & 0xFFFFFFFF));
                }
            };
        }

        template <typename T>
        byte bytes_needed(const T value) noexcept
        {
            return internal::bytes_needed<T, sizeof(T)>()(value);
        }
	}
}
