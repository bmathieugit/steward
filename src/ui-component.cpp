#include <ui-component.hpp>

namespace stew::ui
{
    text_field::text_field(const std::string &label, std::size_t max_length)
        : _label(label), _max_length(max_length) {}

    hidden_text_field::hidden_text_field(const std::string &label)
        : _label(label) {}
}