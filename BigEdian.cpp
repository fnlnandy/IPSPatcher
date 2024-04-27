#include "Global.hpp"
#include "BigEdian.hpp"

BigEdian::BigEdian(const std::string &fileName, const std::string &mode)
{
    _fileBuffer = fopen(fileName.c_str(), mode.c_str());

    if (_fileBuffer == NULL)
        FATAL_ERROR("Unable to open '" << fileName << "' for reading.");

    _fileName = fileName;
    fseek(_fileBuffer, 0, SEEK_END);
    _size = ftell(_fileBuffer);
    rewind(_fileBuffer);
}

BigEdian::~BigEdian()
{
    fclose(_fileBuffer);
}

u8 BigEdian::readU8()
{
    if (isEnd())
        FATAL_ERROR("Reached end of file: '" << _fileName << "'.");

    return fgetc(_fileBuffer);
}

u8 *BigEdian::readBytes(const size_t &length)
{
    u8 *readArray = new u8[length];

    for (size_t i = 0; i < length; i++)
        readArray[i] = readU8();

    return readArray;
}

u16 BigEdian::readU16()
{
    u16 retVal = readU8();
    retVal <<= BITS_IN(u8);
    retVal |= readU8();

    return retVal;
}

u32 BigEdian::readU24()
{
    u32 retVal = readU16();
    retVal <<= BITS_IN(u8);
    retVal |= readU8();

    return retVal;
}

u32 BigEdian::readU32()
{
    u32 retVal = readU16();
    retVal <<= BITS_IN(u16);
    retVal |= readU16();

    return retVal;
}

void BigEdian::writeU8(const u8 &toWrite)
{
    fputc(toWrite, _fileBuffer);
}

void BigEdian::writeBytes(const u8 *toWrite, const size_t &length)
{
    for (size_t i = 0; i < length; i++)
    {
        writeU8(toWrite[i]);
    }
}

void BigEdian::writeU16(const u16 &toWrite)
{
    writeU8(toWrite >> BITS_IN(u8));
    writeU8(toWrite & BITS_IN(u8));
}

void BigEdian::writeU24(const u32 &toWrite)
{
    writeU16(toWrite >> BITS_IN(u8));
    writeU8(toWrite & BITS_IN(u8));
}

void BigEdian::writeU32(const u32 &toWrite)
{
    writeU16(toWrite >> BITS_IN(u16));
    writeU16(toWrite & BITS_IN(u16));
}

void BigEdian::flush()
{
    fflush(_fileBuffer);
}

void BigEdian::seek(const size_t offset)
{
    fseek(_fileBuffer, offset, SEEK_SET);
}

size_t BigEdian::size()
{
    return _size;
}

bool BigEdian::isEnd()
{
    int currPos = ftell(_fileBuffer);

    if (currPos == -1)
        return true;

    return (static_cast<size_t>(currPos) >= _size);
}