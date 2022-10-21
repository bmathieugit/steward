#include <ui-component.hpp>

namespace stew::ui
{
    text_field::text_field(const std::string &label, std::size_t max_length)
        : _label(label), _max_length(max_length) {}

    text_message::text_message(const std::string &key) : _key(key) {}
}