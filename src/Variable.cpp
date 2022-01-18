#include "SimpleNetMessaging/Variable.h"

#include <algorithm>

namespace snm
{
    Variable::Variable(DataType type, DataVariant data) : _type(type), _data(data) {}

    Variable::~Variable() {}

    bool Variable::isWritable() const { return true; }
    DataType Variable::type() const { return _type; }

    void Variable::addObserver(VariableObserver& observer) {
        _observers.push_back(&observer);
    }
    void Variable::removeObserver(VariableObserver& observer) {
        auto iterator = std::find(_observers.begin(), _observers.end(), &observer);
        if (iterator != _observers.end())
            _observers.erase(iterator);
    }
}