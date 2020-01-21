#pragma once

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

		const char* text_of(const tag_t item);
		const char* text_of(const class_t item);
		const char* text_of(const content_t item);

		template <typename T>
		byte bytes_needed(const T value)
		{
			byte c = 0; // 0 => size 1
			c |= (!!(value & 0xFF00)); // 1 => size 2
			c |= (!!(value & 0xFF0000)) << 1; // 2 => size 3
			c |= (!!(value & 0xFF000000)) << 2; // 4 => size 4

			static const byte sizes[] = { 1, 2, 3, 3, 4, 4, 4, 4 };
			return sizes[c];
		}

		//template <typename T>
		//byte bytes_needed2(const T value)
		//{
		//#ifdef GCC
		//	byte bits_needed = (sizeof(T) << 3) - __builtin_clzll(x);
		//#elif defined(MSVC)
		//	unsigned long mask = 0x1000;
		//	unsigned long index;
		//	byte bits_needed = _BitScanReverse(&index, mask);
		//#endif
		//	return (bits_needed != 0) ? (bits_needed + 7) >> 3 : 1;
		//}
	}
}
