#pragma once



namespace aarith {


auto operator<<(std::ostream& out, const Integer& value) -> std::ostream&
{
    if (out.flags() & std::ios::hex)
    {
        out << to_hex(value);
    }
    else if (out.flags() & std::ios::oct)
    {
        out << to_octal(value);
    }
    else
    {
        out << to_decimal(value);
    }
    return out;
}
}