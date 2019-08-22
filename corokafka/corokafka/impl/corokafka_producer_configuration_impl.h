#ifndef BLOOMBERG_COROKAFKA_PRODUCER_CONFIGURATION_IMPL_H
#define BLOOMBERG_COROKAFKA_PRODUCER_CONFIGURATION_IMPL_H

namespace Bloomberg {
namespace corokafka {

// Implementations
template <typename FUNC>
void ProducerConfiguration::setKeyCallback(FUNC&& callback)
{
    using Key = typename std::remove_reference<decltype(argType<0>(callback))>::type;
    _keySerializer.reset(new ConcreteSerializer<Key>(std::forward<FUNC>(callback)));
}

template <typename FUNC>
void ProducerConfiguration::setPayloadCallback(FUNC&& callback)
{
    using Payload = typename std::remove_reference<decltype(argType<1>(callback))>::type;
    _payloadSerializer.reset(new ConcreteSerializerWithHeaders<Payload>(std::forward<FUNC>(callback)));
}

template <typename FUNC>
void ProducerConfiguration::setHeaderCallback(const std::string& name, FUNC&& callback)
{
    using Header = typename std::remove_reference<decltype(argType<0>(callback))>::type;
    _headerSerializers[name].reset(new ConcreteSerializer<Header>(std::forward<FUNC>(callback)));
}

template <typename T>
const Callbacks::KeySerializerCallback<T>& ProducerConfiguration::getKeyCallback() const
{
    return std::static_pointer_cast<ConcreteSerializer<T>>(_keySerializer)->getCallback();
}

template <typename T>
const Callbacks::PayloadSerializerCallback<T>& ProducerConfiguration::getPayloadCallback() const
{
    return std::static_pointer_cast<ConcreteSerializer<T>>(_payloadSerializer)->getCallback();
}

template <typename T>
const Callbacks::HeaderSerializerCallback<T>& ProducerConfiguration::getHeaderCallback(const std::string& name) const
{
    auto it = _headerSerializers.find(name);
    if (it == _headerSerializers.end()) {
        throw std::runtime_error("Invalid header name");
    }
    return std::static_pointer_cast<ConcreteSerializer<T>>(it->second)->getCallback();
}

}
}

#endif //BLOOMBERG_COROKAFKA_PRODUCER_CONFIGURATION_IMPL_H