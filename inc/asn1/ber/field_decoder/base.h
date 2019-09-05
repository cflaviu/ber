#pragma once
#ifndef PCH
	#include <asn1/ber/types.h>
	#include <cstdint>
#endif

namespace asn1
{
	namespace ber
	{
		namespace field_decoder
		{
			enum class state_t: byte
			{
				stopped,
				reading,
				done,
				error
			};

			enum class error_t: byte
			{
				none,
				wrong_field_size,
				field_too_big
			};

			inline bool is_good(const state_t item) { return item != state_t::error; }

			class base
			{
			protected:
				state_t state_;
				error_t error_;

				base() :
					state_(state_t::stopped),
					error_(error_t::none)
				{}

				virtual const byte* first_read(const byte* ptr, const byte* const end) = 0;
				virtual const byte* read(const byte* ptr, const byte* const end) = 0;

				void set_error(const error_t item)
				{
					state_ = state_t::error;
					error_ = item;
				}

			public:
				state_t state() const { return state_; }
				error_t error() const { return error_; }

				const byte* operator () (const byte* ptr, const byte* const end)
				{
					const byte* result;
					switch (state_)
					{
						case state_t::stopped: result = first_read(ptr, end); break;
						case state_t::reading: result = read(ptr, end); break;
						default: result = nullptr;
					}

					return result;
				}

				virtual void reset()
				{
					state_ = state_t::stopped;
					error_ = error_t::none;
				}
			};
		}
	}
}
