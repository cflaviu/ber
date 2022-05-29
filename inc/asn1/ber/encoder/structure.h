#pragma once
#ifndef PCH
    #include <asn1/ber/encoder/base.h>
    #include <tuple>
#endif

namespace asn1::ber::encoder
{
    template <typename _Length = uint16_t>
    class structure: public base<_Length>
    {
    public:
        using base_t = base<_Length>;
        using tag_type = typename base_t::tag_type;
        using length_type = typename base_t::length_type;
        using error_length_pair = typename base_t::error_length_pair;

        enum constants : byte
        {
            constructed_bit = 0x20u,
        };

        structure(const tag_type tag) noexcept: base_t(tag | constructed_bit) {}

        void set_tag(const tag_type item) noexcept { base_t::tag_ = (item | constructed_bit); }

        virtual length_type content_length() const noexcept override
        {
            if (dirty_length_)
            {
                dirty_length_ = false;
                length_type len = 0u;
                for (auto i = begin_; i != nullptr; i = i->next_)
                {
                    len += i->total_length();
                }

                base_t::length_ = typename base_t::length_encoder(len);
            }

            return base_t::length_.value();
        }

        structure& operator<<(base_t& item) noexcept
        {
            if (begin_ == nullptr)
            {
                begin_ = &item;
            }

            if (end_ != nullptr)
            {
                end_->next_ = &item;
            }

            end_ = &item;
            if (!dirty_length_)
            {
                dirty_length_ = true;
            }

            return *this;
        }

    protected:
        virtual error_length_pair internal_encode(byte* buffer, byte* const buffer_end) const noexcept
        {
            byte* buf = buffer;
            error_t encoding_error;
            length_type used;
            for (auto i = begin_; i != nullptr; i = i->next_, buf += used)
            {
                std::tie(encoding_error, used) = i->encode_to(buf, buffer_end);
                if (encoding_error != error_t::none)
                {
                    break;
                }
            }

            return {encoding_error, static_cast<length_type>(buf - buffer)};
        }

        const base_t* begin_ {};
        base_t* end_ {};
        mutable bool dirty_length_ {true};
    };
}
