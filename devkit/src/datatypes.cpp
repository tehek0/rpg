#include "../header/data/datatypes.hpp"


bool dev::is_type_linear(int type_index) {
    return (type_index < datatype::location && type_index > datatype::boolean);
}
bool dev::is_type_struct(int type_index) {
    return (type_index >= datatype::location && type_index <= datatype::item);
}
bool dev::is_type_sybtype(int type_index) {
    return (type_index >= datatype::requirement_subtypes && type_index < datatype::total);
}
bool dev::is_type_unsigned(int type_index) {
    return (type_index >= datatype::u_short && type_index <= datatype::u_long_long);
}
