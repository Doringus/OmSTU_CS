#pragma once

#include <iterator>

namespace omstl {

    template <typename It>
    class ReverseIterator {
    public:
        using iterator_category = typename std::iterator_traits<It>::iterator_category;
        using value_type        = typename std::iterator_traits<It>::value_type;
        using difference_type   = typename std::iterator_traits<It>::difference_type;
        using pointer           = typename std::iterator_traits<It>::pointer;
        using reference         = typename std::iterator_traits<It>::reference;
    public:

        constexpr explicit ReverseIterator(It iterator) noexcept : m_It(iterator) { }

        constexpr reference operator*() const noexcept;
      //  constexpr pointer operator->() const noexcept;

        constexpr ReverseIterator<It>& operator++() noexcept;
        constexpr ReverseIterator<It> operator++(int) & noexcept;

        constexpr ReverseIterator<It>& operator--() noexcept;
        constexpr ReverseIterator<It> operator--(int) & noexcept;

        friend auto operator<=>(const ReverseIterator& lhs, const ReverseIterator& rhs) = default;

    private:
        It m_It;
    };

    template<typename It>
    constexpr inline typename ReverseIterator<It>::reference ReverseIterator<It>::operator*() const noexcept {
        return *std::prev(m_It);
    }
    /*
    template<typename It>
    constexpr inline typename ReverseIterator<It>::pointer ReverseIterator<It>::operator->() const noexcept {
        return nullptr;
    }*/

    template<typename It>
    constexpr inline ReverseIterator<It>& ReverseIterator<It>::operator++() noexcept {
        --m_It;
        return *this;
    }

    template<typename It>
    constexpr inline ReverseIterator<It> ReverseIterator<It>::operator++(int) & noexcept {
        auto tmp = m_It;
        operator++();
        return tmp;
    }

    template<typename It>
    constexpr inline ReverseIterator<It>& ReverseIterator<It>::operator--() noexcept {
        ++m_It;
        return *this;
    }

    template<typename It>
    constexpr inline ReverseIterator<It> ReverseIterator<It>::operator--(int) & noexcept {
        auto tmp = m_It;
        operator--();
        return tmp;
    }

}