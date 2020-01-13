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
	}
}
