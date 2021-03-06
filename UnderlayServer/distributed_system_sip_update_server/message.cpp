#include "message.h"

void Message::Init(char *d, int length)
{
    data = d;
    maxSize = length;
    size = 0;
    readCount = 0;
    overFlow = false;
    body_length_ = 0;
}


void Message::Clear(void)
{
    size = 0;
    readCount = 0;
    overFlow = false;
    body_length_ = 0;
}


char *Message::GetNewPoint(int length)
{
    char *tempData;
    if (size + length > maxSize)
    {
        Clear();
        overFlow = true;
    }
    tempData = data + size;
    size += length;
    return tempData;
}



void Message::Write(void *d, int length)
{
    memcpy(GetNewPoint(length), d, length);
}

void Message::WriteByte(char c)
{
    char *buf;
    buf = GetNewPoint(1);
    memcpy(buf, &c, 1);
}

void Message::WriteShort(short c)
{
    char *buf;
    buf = GetNewPoint(2);
    memcpy(buf, &c, 2);
}

void Message::WriteLong(long c)
{
    char *buf;
    buf = GetNewPoint(4);
    memcpy(buf, &c, 4);
}

void Message::WriteLongLong(long long c)
{
    char *buf;
    buf = GetNewPoint(8);
    memcpy(buf, &c, 8);
}

void Message::WriteFloat(float c)
{
    char *buf;
    buf = GetNewPoint(4);
    memcpy(buf, &c, 4);
}

void Message::WriteString(char *s)
{
    if (!s)
    {
        return;
    }
    else
        Write(s, strlen(s) + 1);
}

void Message::BeginReading(void)
{
    readCount = 0;
}

void Message::BeginReading(int s)
{
    size = s;
    readCount = 0;
}

char *Message::Read(int s)
{
    static char c[2048];
    if (readCount + s > size)
        return NULL;
    else
        memcpy(&c, &data[readCount], s);
    readCount += s;
    return c;
}

char Message::ReadByte(void)
{
    char c;
    if (readCount + 1 > size)
        c = -1;
    else
        memcpy(&c, &data[readCount], 1);
    readCount++;
    return c;
}

short Message::ReadShort(void)
{
    short c;
    if (readCount + 2 > size)
        c = -1;
    else
        memcpy(&c, &data[readCount], 2);
    readCount += 2;
    return c;
}

long Message::ReadLong(void)
{
    long c;
    if (readCount + 4 > size)
        c = -1;
    else
        memcpy(&c, &data[readCount], 4);
    readCount += 4;
    return c;
}

long long Message::ReadLongLong()
{
    long long c;
    if (readCount + 8 > size)
        c = -1;
    else
        memcpy(&c, &data[readCount], 8);
    readCount += 8;
    return c;
}

float Message::ReadFloat(void)
{
    float c;
    if (readCount + 4 > size)
        c = -1;
    else
        memcpy(&c, &data[readCount], 4);
    readCount += 4;
    return c;
}

char *Message::ReadString(void)
{
    static char string[2048];
    int l, c;
    l = 0;
    do
    {
        c = ReadByte();
        if (c == -1 || c == 0)
            break;
        string[l] = c;
        l++;
    } while (l < sizeof(string) - 1);
    string[l] = 0;
    return string;
}

bool Message::decode_header()
{
    using namespace std;
    char header[header_length + 1] = "";
    strncat(header, data, header_length);
    readCount += header_length;
    body_length_ = atoi(header);
    if (body_length_ > 1396)
    {
        body_length_ = 0;
        return false;
    }
    return true;
}

void Message::encode_header()
{
    using namespace std;
    char header[header_length + 1] = "";
    sprintf(header, "%4d", static_cast<int>(body_length_));
    char *buf;
    buf = GetNewPoint(4);
    memcpy(buf, header, header_length);
}
