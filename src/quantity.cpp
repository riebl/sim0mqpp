#include "sim0mqpp/quantity.hpp"
#include "sim0mqpp/serialization.hpp"

namespace sim0mqpp
{

void serialize(SerializationOutput& out, const ScalarQuantity<float>& q)
{
    out.write(FieldType::FLOAT_32_UNIT);
    out.write(q.unit());
    out.write_byte(q.display());
    out.write(q.value());
}

void serialize(SerializationOutput& out, const ScalarQuantity<double>& q)
{
    out.write(FieldType::DOUBLE_64_UNIT);
    out.write(q.unit());
    out.write_byte(q.display());
    out.write(q.value());
}

void deserialize(SerializationInput& in, ScalarQuantity<float>& q)
{
    q.unit(static_cast<Unit>(in.read_byte()));
    q.display(in.read_byte());
    float tmp;
    in.read(tmp);
    q.value(tmp);
}

void deserialize(SerializationInput& in, ScalarQuantity<double>& q)
{
    q.unit(static_cast<Unit>(in.read_byte()));
    q.display(in.read_byte());
    double tmp;
    in.read(tmp);
    q.value(tmp);
}

void serialize(SerializationOutput& out, const VectorQuantity<float>& q)
{
    out.write(FieldType::FLOAT_32_UNIT_ARRAY);
    out.write(static_cast<std::int32_t>(q.values().size()));
    out.write(q.unit());
    out.write_byte(q.display());
    for (auto it = q.values().begin(); it != q.values().end(); ++it) {
        out.write(*it);
    }
}

void serialize(SerializationOutput& out, const VectorQuantity<double>& q)
{
    out.write(FieldType::DOUBLE_64_UNIT_ARRAY);
    out.write(static_cast<std::int32_t>(q.values().size()));
    out.write(q.unit());
    out.write_byte(q.display());
    for (auto it = q.values().begin(); it != q.values().end(); ++it) {
        out.write(*it);
    }
}

} // namespace sim0mqpp
