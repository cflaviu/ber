#pragma once
#ifndef PCH
	#include <asn1/ber/decoder/field/base.h>
	#include <asn1/ber/decoder/field/tag.h>
	#include <asn1/ber/decoder/field/length.h>
	#include <asn1/ber/decoder/field/value.h>
	#include <asn1/ber/decoder/basic.h>
#endif

namespace asn1
{
	namespace ber
	{
		namespace decoder
		{
			/*
			struct observer_example
			{
				void on_data(const field::tag& tag, const byte* data, const uint32_t data_size);
				void on_error(const decoder_id item, const byte* buffer, const uint32_t buffer_size);
			};
			*/

			template <typename _Observer, typename _Length = uint16_t>
			class engine
			{
			public:
				using tag_type = byte;
				using length_type = _Length;
				using state_t = typename basic<_Length>::state_t;

			protected:
				using _Decoder = basic<_Length>;

				class internal_data
				{
					const _Decoder& decoder_;
					state_t	state_;
					error_t	error_;
					state_t* state_ptr_;
					error_t* error_ptr_;

				public:
					internal_data(const _Decoder& decoder) noexcept :
						decoder_(decoder),
						state_(state_t::stopped),
						error_(error_t::none),
						state_ptr_(nullptr),
						error_ptr_(nullptr)
					{}

					state_t state() const noexcept
					{
						return (state_ptr_ == nullptr) ? decoder_.state() : *state_ptr_;
					}

					error_t error() const noexcept
					{
						return (error_ptr_ == nullptr) ? decoder_.error() : *error_ptr_;
					}

					bool good() const noexcept{ return field::is_good(state()); }

					void set_error(const error_t item) noexcept
					{
						if (state_ptr_ == nullptr)
						{
							state_ptr_ = &state_;
							error_ptr_ = &error_;
						}

						state_ = state_t::error;
						error_ = item;
					}

					void reset() noexcept
					{
						if (state_ptr_ != nullptr)
						{
							state_ptr_ = nullptr;
							error_ptr_ = nullptr;
						}
					}
				};

				std::pair<bool, const byte*> read(const byte* buffer, const byte* const buffer_end) noexcept;

				_Decoder decoder_;
				internal_data internal_data_;
				_Observer* observer_;

			public:
				engine(byte* decoding_buffer, const length_type decoding_buffer_size, _Observer* const observer = nullptr) noexcept:
					decoder_(decoding_buffer, decoding_buffer + decoding_buffer_size),
					internal_data_(decoder_),
					observer_(observer)
				{}

				state_t state() const noexcept { return internal_data_.state(); }

				bool good() const noexcept { return internal_data_.good(); }

				_Observer* observer() const noexcept { return observer_; }

				void set_observer(_Observer* const item) noexcept { observer_ = item; }

				std::pair<bool, const byte*> operator () (const byte* buffer, const byte* const buffer_size) noexcept;

				void reset() noexcept
				{
					decoder_.reset();
					internal_data_.reset();
				}
			};

			template <typename _Observer, typename _Length>
			std::pair<bool, const byte*> engine<_Observer, _Length>::read(const byte* buffer, const byte* const buffer_end) noexcept
			{
				auto result = std::make_pair(true, buffer_end);
				auto next = decoder_(buffer, buffer_end);
				if (decoder_.state() == state_t::done)
				{
					auto& length_item = decoder_.length();
					auto& tag_item = decoder_.tag();

					const auto length = length_item.value();
					const auto extent = tag_item.bytes() + length_item.bytes() + length;
					const auto remaining = std::distance(buffer, buffer_end);
					if (extent <= remaining)
					{
						result.second = (tag_item.content_type() == content_t::constructed) ? next - length : next;
						if (observer_ != nullptr)
						{
							observer_->on_data(tag_item, decoder_.value().buffer(), length);
						}
					}
					else
					{
						constexpr auto error = error_t::wrong_field_size;
						internal_data_.set_error(error);
						result.first = false;
						if (observer_ != nullptr)
						{
							observer_->on_error(meta_id, static_cast<byte>(error), buffer, static_cast<_Length>(buffer_end - buffer));
						}
					}
				}

				return result;
			}

			template <typename _Observer, typename _Length>
			std::pair<bool, const byte*> engine<_Observer, _Length>::operator () (const byte* buffer, const byte* const buffer_end) noexcept
			{
				std::pair<bool, const byte*> result;
				switch (decoder_.state())
				{
					case state_t::done:
					{
						decoder_.reset();
						[[fallthrough]];
					}
					case state_t::stopped:
					case state_t::reading:
					{
						result = read(buffer, buffer_end);
						break;
					}
					case state_t::error:
					{
						result.first = false;
						if (observer_ != nullptr)
						{
							observer_->on_error(decoder_.current_decoder(), static_cast<byte>(internal_data_.error()), buffer, static_cast<_Length>(buffer_end - buffer));
						}

						break;
					}
					default:
					{
						result.first = false;
					}
				}

				return result;
			}
		}
	}
}
