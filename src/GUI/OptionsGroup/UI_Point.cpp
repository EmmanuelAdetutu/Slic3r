#include "OptionsGroup/Field.hpp"
#include "misc_ui.hpp"
#include "Log.hpp"

#include <regex>

namespace Slic3r { namespace GUI {

using namespace std::string_literals;

std::string UI_Point::get_string() {
    std::string result {""};
    result.append(trim_zeroes(_ctrl_x->GetValue().ToStdString()));
    result.append(";"s);
    result.append(trim_zeroes(_ctrl_y->GetValue().ToStdString()));
    return result;
}

Slic3r::Pointf UI_Point::get_point() {
    return Pointf(std::stod(this->_ctrl_x->GetValue().ToStdString()), std::stod(this->_ctrl_y->GetValue().ToStdString()));
}

void UI_Point::set_value(boost::any value) {
    // type detection and handing off to children
    if (value.type() == typeid(Slic3r::Pointf)) {
        this->_set_value(boost::any_cast<Pointf>(value));
    } else if (value.type() == typeid(std::string)) {
        this->_set_value(boost::any_cast<std::string>(value));
    } else if (value.type() == typeid(wxString)) {
        this->_set_value(boost::any_cast<wxString>(value).ToStdString());
    } else {
        Slic3r::Log::warn(this->LogChannel(), LOG_WSTRING("Type " << value.type().name() << " is not handled in set_value."));
    }
}

void UI_Point::_set_value(Slic3r::Pointf value) {
    /// load the controls directly from the value
    this->_ctrl_x->SetValue(trim_zeroes(std::to_string(value.x)));
    this->_ctrl_y->SetValue(trim_zeroes(std::to_string(value.y)));
}

void UI_Point::_set_value(std::string value) {
    /// parse the string into the two parts.
    std::regex format_regex(";");
    auto f_begin { std::sregex_token_iterator(value.begin(), value.end(), format_regex, -1) };
    auto f_end { std::sregex_token_iterator() };

    if (f_begin != f_end) {
        auto iter = f_begin;
        this->_ctrl_x->SetValue(trim_zeroes(iter->str()));
        iter++;
        if (iter != f_end)
            this->_ctrl_y->SetValue(trim_zeroes(iter->str()));
    }


}

UI_Point::UI_Point(wxWindow* parent, Slic3r::ConfigOptionDef _opt, wxWindowID id) {
    Slic3r::Pointf def_val {_opt.default_value == nullptr ? Pointf() : Pointf(*(dynamic_cast<ConfigOptionPoint*>(_opt.default_value))) };
    
    this->_ctrl_x = new wxTextCtrl(parent, wxID_ANY, trim_zeroes(wxString::FromDouble(def_val.x)), wxDefaultPosition, this->field_size);
    this->_ctrl_y = new wxTextCtrl(parent, wxID_ANY, trim_zeroes(wxString::FromDouble(def_val.y)), wxDefaultPosition, this->field_size);

    this->_lbl_x = new wxStaticText(parent, wxID_ANY, wxString("x:"));
    this->_lbl_y = new wxStaticText(parent, wxID_ANY, wxString("y:"));
}

} } // Namespace Slic3r::GUI
