#include "sim0mqpp/buffer_serialization.hpp"
#include <boost/endian/conversion.hpp>

namespace sim0mqpp
{

BufferSerializer::BufferSerializer(Buffer& buffer) :
    m_output(std::back_inserter(buffer))
{
}

void BufferSerializer::write_byte(std::uint8_t byte)
{
    *m_output++ = byte;
}

bool BufferSerializer::big_endian() const
{
    return true;
}

void BufferSerializer::write(std::int8_t num8)
{
    *m_output++ = num8;
}

void BufferSerializer::write(std::int16_t num16)
{
    *m_output++ = (num16 >> 8);
    *m_output++ = (num16);
}

void BufferSerializer::write(std::int32_t num32)
{
    *m_output++ = (num32 >> 24);
    *m_output++ = (num32 >> 16);
    *m_output++ = (num32 >> 8);
    *m_output++ = (num32);
}

void BufferSerializer::write(std::int64_t num64)
{
    *m_output++ = (num64 >> 56);
    *m_output++ = (num64 >> 48);
    *m_output++ = (num64 >> 40);
    *m_output++ = (num64 >> 32);
    *m_output++ = (num64 >> 24);
    *m_output++ = (num64 >> 16);
    *m_output++ = (num64 >> 8);
    *m_output++ = (num64);
}

void BufferSerializer::write(float f)
{
    write(*reinterpret_cast<std::int32_t*>(&f));
}

void BufferSerializer::write(double d)
{
    write(*reinterpret_cast<std::int64_t*>(&d));
}

void BufferSerializer::write(bool b)
{
    *m_output++ = b ? 1 : 0;
}

void BufferSerializer::write(char c)
{
    *m_output++ = c;
}

void BufferSerializer::write(char16_t wc)
{
    write(static_cast<std::int16_t>(wc));
}


BufferDeserializer::BufferDeserializer(const Buffer& buffer) :
    BufferDeserializer(buffer.cbegin(), buffer.cend())
{
}

BufferDeserializer::BufferDeserializer(Buffer::const_iterator begin, Buffer::const_iterator end) :
    m_cursor(begin), m_end(end)
{
}

void BufferDeserializer::error(const char* msg)
{
    if (m_error.empty()) {
        m_error = msg;
    }
}

std::uint8_t BufferDeserializer::read_byte()
{
    if (std::distance(m_cursor, m_end) >= 1) {
        return *m_cursor++;
    } else {
        error("buffer ended prematurely reading 1 byte");
    }

    return 0;
}

void BufferDeserializer::read(std::int8_t& i)
{
    i = read_byte();
}

void BufferDeserializer::read(std::int16_t& i)
{
    if (std::distance(m_cursor, m_end) >= 2) {
        const unsigned char* cursor = &(*m_cursor);
        if (m_big_endian) {
            i = boost::endian::endian_load<std::int16_t, 2, boost::endian::order::big>(cursor);
        } else {
            i = boost::endian::endian_load<std::int16_t, 2, boost::endian::order::little>(cursor);
        }
        std::advance(m_cursor, 2);
    } else {
        error("buffer ended prematurely reading 2 bytes");
        i = 0;
    }
}

void BufferDeserializer::read(std::int32_t& i)
{
    if (std::distance(m_cursor, m_end) >= 4) {
        const unsigned char* cursor = &(*m_cursor);
        if (m_big_endian) {
            i = boost::endian::endian_load<std::int32_t, 4, boost::endian::order::big>(cursor);
        } else {
            i = boost::endian::endian_load<std::int32_t, 4, boost::endian::order::little>(cursor);
        }
        std::advance(m_cursor, 4);
    } else {
        error("buffer ended prematurely reading 4 bytes");
        i = 0;
    }
}

void BufferDeserializer::read(std::int64_t& i)
{
    if (std::distance(m_cursor, m_end) >= 8) {
        const unsigned char* cursor = &(*m_cursor);
        if (m_big_endian) {
            i = boost::endian::endian_load<std::int64_t, 8, boost::endian::order::big>(cursor);
        } else {
            i = boost::endian::endian_load<std::int64_t, 8, boost::endian::order::little>(cursor);
        }
        std::advance(m_cursor, 8);
    } else {
        error("buffer ended prematurely reading 8 bytes");
        i = 0;
    }
}

void BufferDeserializer::read(float& f)
{
    read(reinterpret_cast<std::int32_t&>(f));
}

void BufferDeserializer::read(double& d)
{
    read(reinterpret_cast<std::int64_t&>(d));
}

void BufferDeserializer::read(bool& b)
{
    b = (read_byte() != 0);
}

void BufferDeserializer::read(char& c)
{
    c = static_cast<char>(read_byte());
}

void BufferDeserializer::read(char16_t& c)
{
    read(reinterpret_cast<std::int16_t&>(c));
}

} // namespace sim0mqpp
