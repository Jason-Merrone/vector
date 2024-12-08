#include <cstddef>
#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>

namespace usu
{
    template <typename T, std::size_t InitialCapacity = 10>
    class vector
    {
      public:
        using size_type = std::size_t;
        using reference = T&;
        using pointer = std::shared_ptr<T[]>;
        using value_type = T;
        using resize_type = std::function<size_type(size_type)>;

        class iterator
        {
          public:
            using iterator_category = std::bidirectional_iterator_tag; // The notes wanted these ones so I used them
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using size_type = std::size_t;

            iterator();
            iterator(size_type pos, pointer ptr);
            iterator(const iterator& obj);
            iterator(iterator&& obj) noexcept;
            iterator& operator=(const iterator& rhs);
            iterator& operator=(iterator&& rhs);

            iterator operator++();
            iterator operator++(int);
            iterator operator--();
            iterator operator--(int);

            reference operator*();
            pointer operator->();

            bool operator==(const iterator& rhs) const;
            bool operator!=(const iterator& rhs) const;

          private:
            size_type m_pos;
            pointer m_ptr;
        };

        vector();
        vector(size_type size);
        vector(resize_type resize);
        vector(size_type size, resize_type resize);
        vector(std::initializer_list<T> list);
        vector(std::initializer_list<T> list, resize_type resize);

        reference operator[](size_type index);
        void add(T value);
        void insert(size_type index, T value);
        void remove(size_type index);
        void clear();
        size_type size();
        size_type capacity();
        iterator begin();
        iterator end();
        void map(std::function<void(reference)> func);

      private:
        size_type m_size;
        size_type m_capacity;
        pointer m_data;
        resize_type m_resize_func;
    };
    // Vector definitions below this line!

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::vector()
    {
        m_size = 0;
        m_capacity = InitialCapacity;
        m_resize_func = [](size_type currentCapacity) -> size_type
        {
            return currentCapacity * 2;
        };
        m_data = pointer(new T[m_capacity]());
    }

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::vector(size_type size)
    {
        m_resize_func = [](size_type currentCapacity) -> size_type
        {
            return currentCapacity * 2;
        };
        if (size > InitialCapacity)
        {
            m_capacity = size * 2;
        }
        else
        {
            m_capacity = InitialCapacity;
        }
        m_size = size;
        m_data = pointer(new T[m_capacity]());
    }

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::vector(resize_type resize)
    {
        m_size = 0;
        m_capacity = InitialCapacity;
        m_resize_func = resize;
        m_data = pointer(new T[m_capacity]());
    }

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::vector(size_type size, resize_type resize)
    {
        m_resize_func = resize;
        if (size > InitialCapacity)
        {
            m_capacity = size * 2;
        }
        else
        {
            m_capacity = InitialCapacity;
        }
        m_size = size;
        m_data = pointer(new T[m_capacity]());
    }

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::vector(std::initializer_list<T> list)
    {
        m_size = 0;
        m_capacity = InitialCapacity;
        m_resize_func = [](size_type currentCapacity) -> size_type
        {
            return currentCapacity * 2;
        };
        m_data = pointer(new T[m_capacity]());
        for (auto& val : list)
        {
            add(val);
        }
    }

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::vector(std::initializer_list<T> list, resize_type resize)
    {
        m_size = 0;
        m_capacity = InitialCapacity;
        m_resize_func = resize;
        m_data = pointer(new T[m_capacity]());
        for (auto& val : list)
        {
            add(val);
        }
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::reference vector<T, InitialCapacity>::operator[](size_type index)
    {
        if (index >= m_size)
        {
            throw std::range_error("Index out of bounds");
        }
        return m_data[index];
    }

    template <typename T, std::size_t InitialCapacity>
    void vector<T, InitialCapacity>::add(T value)
    {
        if (m_size == m_capacity)
        {
            size_type new_cap = m_resize_func(m_capacity);
            pointer new_data(new T[new_cap]());
            for (size_type i = 0; i < m_size; i++)
            {
                new_data[i] = m_data[i];
            }
            m_capacity = new_cap;
            m_data = new_data;
        }
        m_data[m_size] = value;
        m_size++;
    }

    template <typename T, std::size_t InitialCapacity>
    void vector<T, InitialCapacity>::insert(size_type index, T value)
    {
        if (index > m_size)
        {
            throw std::range_error("Index out of bounds for insert");
        }

        if (m_size == m_capacity)
        {
            size_type new_cap = m_resize_func(m_capacity);
            pointer new_data(new T[new_cap]());
            for (size_type i = 0; i < m_size; i++)
            {
                new_data[i] = m_data[i];
            }
            m_data = new_data;
            m_capacity = new_cap;
        }

        for (size_type i = m_size; i > index; i--)
        {
            m_data[i] = m_data[i - 1];
        }
        m_data[index] = value;
        m_size++;
    }

    template <typename T, std::size_t InitialCapacity>
    void vector<T, InitialCapacity>::remove(size_type index)
    {
        if (index >= m_size) // I learned the hard way not to disclude index 0!
        {
            throw std::range_error("Index out of bounds for remove");
        }

        for (size_type i = index; i < m_size - 1; i++)
        {
            m_data[i] = m_data[i + 1];
        }
        m_size--;
    }

    template <typename T, std::size_t InitialCapacity>
    void vector<T, InitialCapacity>::map(std::function<void(reference)> func)
    {
        for (size_type i = 0; i < m_size; i++)
        {
            func(m_data[i]);
        }
    }

    // These are the efinitions for vector's iterator methods
    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator::reference vector<T, InitialCapacity>::iterator::operator*()
    {
        return m_ptr[m_pos];
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator::pointer vector<T, InitialCapacity>::iterator::operator->()
    {
        return &m_ptr[m_pos];
    }

    template <typename T, std::size_t InitialCapacity>
    bool vector<T, InitialCapacity>::iterator::operator!=(const iterator& rhs) const
    {
        return m_pos != rhs.m_pos || m_ptr != rhs.m_ptr;
    }

    // These are the efinitions for vector  methods
    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator vector<T, InitialCapacity>::begin()
    {
        return iterator(0, m_data.get());
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator vector<T, InitialCapacity>::end()
    {
        return iterator(m_size, m_data.get());
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::size_type vector<T, InitialCapacity>::size()
    {
        return m_size;
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::size_type vector<T, InitialCapacity>::capacity()
    {
        return m_capacity;
    }

    template <typename T, std::size_t InitialCapacity>
    bool vector<T, InitialCapacity>::iterator::operator==(const iterator& rhs) const
    {
        return m_pos == rhs.m_pos && m_ptr == rhs.m_ptr;
    }

    // Iterator definitions below this line!
    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::iterator::iterator()
    {
        m_pos = 0;
        m_ptr = nullptr;
    }

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::iterator::iterator(size_type pos, pointer ptr)
    {
        m_pos = pos;
        m_ptr = ptr;
    }

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::iterator::iterator(const iterator& obj)
    {
        m_pos = obj.m_pos;
        m_ptr = obj.m_ptr;
    }

    template <typename T, std::size_t InitialCapacity>
    vector<T, InitialCapacity>::iterator::iterator(iterator&& obj) noexcept
    {
        m_pos = obj.m_pos;
        m_ptr = obj.m_ptr;
        obj.m_pos = 0;
        obj.m_ptr = nullptr;
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator& vector<T, InitialCapacity>::iterator::operator=(const iterator& rhs)
    {
        if (this != &rhs)
        {
            m_pos = rhs.m_pos;
            m_ptr = rhs.m_ptr;
        }
        return *this;
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator& vector<T, InitialCapacity>::iterator::operator=(iterator&& rhs)
    {
        if (this != &rhs)
        {
            m_pos = rhs.m_pos;
            m_ptr = rhs.m_ptr;
            rhs.m_pos = 0;
            rhs.m_ptr = nullptr;
        }
        return *this;
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator vector<T, InitialCapacity>::iterator::operator++()
    {
        m_pos++;
        return *this;
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator vector<T, InitialCapacity>::iterator::operator++(int)
    {
        iterator temp = *this;
        m_pos++;
        return temp;
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator vector<T, InitialCapacity>::iterator::operator--()
    {
        m_pos--;
        return *this;
    }

    template <typename T, std::size_t InitialCapacity>
    typename vector<T, InitialCapacity>::iterator vector<T, InitialCapacity>::iterator::operator--(int)
    {
        iterator temp = *this;
        m_pos--;
        return temp;
    }

} // namespace usu