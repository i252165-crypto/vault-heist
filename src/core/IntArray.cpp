#include "IntArray.h"

IntArray::IntArray()
{
    m_data = nullptr;
    m_size = 0;
    m_cap = 0;
}

IntArray::~IntArray() { delete[] m_data; }

IntArray::IntArray(const IntArray &other)
{
    m_size = other.m_size;
    m_cap = other.m_cap;

    int *newData = new int[m_size];

    for (int i = 0; i < m_size; ++i)
    {
        newData[i] = other.m_data[i];
    }

    m_data = newData;
}

IntArray &IntArray::operator=(const IntArray &other)
{
    if (this == &other)
        return *this;

    delete[] this->m_data;
    int *newData = new int[other.m_size];

    for (int i = 0; i < other.m_size; ++i)
    {
        newData[i] = other.m_data[i];
    }

    m_size = other.m_size;
    m_cap = other.m_cap;

    return *this;
}

void IntArray::reserve(int n)
{
    if (n > m_cap)
    {
        int *newData = new int[n];
        for (int i = 0; i < m_size; ++i)
        {
            newData[i] = m_data[i];
        }

        delete[] m_data;
        m_data = newData;
        m_cap = n;
    }
}

void IntArray::push(int v)
{
    if (m_size == m_cap)
    {
        int newCap = (m_cap == 0) ? 8 : m_cap * 2;
        reserve(newCap);
    }

    m_data[m_size++] = v;
}

void IntArray::pop()
{
    if (m_size > 0)
    {
        --m_size;
    }
}

int IntArray::get(int i) const
{
    if (i < 0 || i >= m_size)
        return -1;

    return m_data[i];
}

int &IntArray::operator[](int i)
{
    if (i < 0 || i >= m_size)
    {
        static int dummy = 0;
        dummy = 0; // reset in case modified
        return dummy;
    }

    return m_data[i];
}

const int &IntArray::operator[](int i) const
{
    if (i < 0 || i >= m_size)
    {
        static int dummy = 0;
        dummy = 0; // reset in case modified
        return dummy;
    }

    return m_data[i];
}

void IntArray::clear() { m_size = 0; }

int IntArray::indexOf(int v) const
{
    for (int i = 0; i < m_size - 1; ++i)
    {
        if (m_data[i] == v)
            return i;
    }

    return -1;
}

void IntArray::swap(int i, int j)
{
    if (i < 0 || i >= m_size || j < 0 || j >= m_size)
        return;

    for (int i = 0; i < m_size; ++i)
    {
        int temp = m_data[i];
        m_data[i] = m_data[j];
        m_data[i] = temp;
    }
}
