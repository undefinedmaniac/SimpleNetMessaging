#pragma once

#include "Variable.h"

namespace snm
{
    class RemoteVariable;
    using RemoteVariablePtr = Ptr<RemoteVariable>;

    struct RemoteVariableObserver
    {
        virtual ~RemoteVariableObserver() {}

        // Called whenever the value of an observed Variable changes
        virtual void updated(RemoteVariablePtr variable) {}
    };

    class RemoteVariable : public Variable
    {
        const bool _isWritable;
        RemoteVariableObserver* _observer;

        RemoteVariable(DataType type, DataVariant data, bool isWritable, RemoteVariableObserver* observer) : 
            Variable(type, data), _isWritable(isWritable), _observer(observer)  {}

        void notifyPublicSetValue() override {
            if (_observer)
                _observer->updated(std::static_pointer_cast<RemoteVariable>(Variable::shared_from_this()));
        }
    public:
        // Create a new RemoteVariable object with the specified starting value
        // isWritable determines if the RemoteVariable should be writable via the public Variable interface
        // The parameterized type MUST be a valid DataType in the DataType enum, otherwise
        // This method will throw an exception
        template <typename T>
        static RemoteVariablePtr create(const T& value, bool isWritable, RemoteVariableObserver* observer = nullptr) {
            static_assert(GetDataTypeRemoveCV<T>::value != DataType::Null, "Invalid variable type");

            struct VariableMakeSharedEnabler : public RemoteVariable {
                VariableMakeSharedEnabler(DataType type, DataVariant data, bool isWritable, RemoteVariableObserver* observer) : 
                RemoteVariable(type, data, isWritable, observer) {}
            };
            return std::make_shared<VariableMakeSharedEnabler>(GetDataType<T>::value, value, isWritable, observer);
        }

        // Set the value of the Variable. Unlike setValue(), this method bypasses checks to determine 
        // if the Variable is writable, allowing it to be set even when isWritable() == false.
        // Additionally, this method will NOT notify any associated RemoteVariableObserver
        // The parameterized type MUST match the type of the Variable, or the method will throw an exception.
        template <typename T>
        void setValueRemote(const T& value) {
            setValueAndNotify<T>(value);
        }

        // Check if the Varible is writeable
        bool isWritable() const override { return _isWritable; }

        // Set the RemoteVariableObserver, which will be notified if this RemoteVariable is changed via
        // the public Variable::setValue() function
        void setObserverRemote(RemoteVariableObserver* observer) {
            _observer = observer;
        }
    };
}