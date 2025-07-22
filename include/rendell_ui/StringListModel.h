#pragma once
#include "Signal.h"
#include "defines.h"
#include <string>
#include <vector>

namespace rendell_ui {
class StringListModel {
public:
    using String = std::wstring;

    StringListModel(std::initializer_list<String> data = {});
    virtual ~StringListModel() = default;

    size_t getSize() const;
    const std::vector<String> &getData() const;

    void addItem(const String &value);
    void insertItem(const String &value, size_t index);
    void setItem(const String &value, size_t index);
    void removeItem(size_t index);
    void removeItem(const String &value);

    const String &operator[](size_t i);

    Signal<void, const String &, size_t> itemAdded;
    Signal<void, const String &, size_t> itemRemoved;
    Signal<void, const String &, size_t> itemChanged;

private:
    std::vector<String> _data;
};

RENDELL_UI_DECLARE_SHARED_PTR_FACTORY(StringListModel)
} // namespace rendell_ui