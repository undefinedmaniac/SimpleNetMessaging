#pragma once

#include "Types.h"
#include "DataType.h"

#include <boost/type_index.hpp>

namespace snm
{
    class Variable;
    using VariablePtr = Ptr<Variable>;

    struct VariableObserver
    {
        virtual ~VariableObserver() {}

        // Called whenever the value of an observed Variable changes
        virtual void updated(VariablePtr variable) {}
    };

    class Variable : public std::enable_shared_from_this<Variable>
    {
        const DataType _type;
        DataVariant _data;
        Vector<VariableObserver*> _observers;
    protected:
        Variable(DataType type, DataVariant data);

        // Set the value of the Variable and call updated() on public observers if the Variable value has changed
        // Returns true if Variable value has changed, false otherwise
        template <typename T>
        bool setValueAndNotify(const T& value) {
            if (boost::typeindex::type_id<T>().type_info() != _data.type())
                throw std::runtime_error("Attempted to set the value of a Variable with the wrong type of data");

            bool isNewValue = boost::get<T>(_data) != value;
            if (isNewValue) {
                _data = value;
                for (VariableObserver* observer : _observers)
                    observer->updated(Variable::shared_from_this());
            }

            return isNewValue;
        }

        // Called whenever the Variable value has changed as a result of the public setValue() method
        virtual void notifyPublicSetValue() {}
    public:
        // Disable copying, as Variables are intended to be used via pointers
        Variable(const Variable&) = delete;
        Variable& operator=(const Variable&) = delete;
        virtual ~Variable();

        // Create a new Variable object with the specified starting value
        // The parameterized type MUST be a valid DataType in the DataType enum, otherwise
        // This method will throw an exception
        template <typename T>
        static VariablePtr create(const T& value = T()) {
            static_assert(GetDataTypeRemoveCV<T>::value != DataType::Null, "Invalid variable type");

            struct VariableMakeSharedEnabler : public Variable {
                VariableMakeSharedEnabler(DataType type, DataVariant data) : Variable(type, data) {}
            };
            return std::make_shared<VariableMakeSharedEnabler>(GetDataType<T>::value, value);
        }

        // Retrieve the value of the Variable. The parameterized type MUST match the type of the
        // Variable, or the method will throw an exception
        template <typename T>
        const T& getValue() const { return boost::get<T>(_data); }

        // Set the value of the Variable. The parameterized type MUST match the type of the
        // Variable, or the method will throw an exception. The method will also throw if
        // this Variable is read-only, AKA Variable::isWritable() == false
        template <typename T>
        void setValue(const T& value) {
            // Cannot change the value of a read only Variable
            if (!isWritable())
                throw std::runtime_error("Attempted to set the value of a read only Variable");

            // Set the value of the Variable and notify observers
            if (setValueAndNotify<T>(value))
                notifyPublicSetValue();
        }

        // Check if the Varible is writeable
        virtual bool isWritable() const;

        // Get the DataType of the Variable
        DataType type() const;

        // Add or remove VariableObservers, which will be notified when the value of the Variable changes
        void addObserver(VariableObserver& observer);
        void removeObserver(VariableObserver& observer);
    };
}