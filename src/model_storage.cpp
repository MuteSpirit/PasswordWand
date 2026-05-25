#include "model_storage.hpp"
#include "model.hpp"

template<typename T>
bool
ModelStorage::is_exist(const char* key)
{
    (void)(key);
    return false;
}

template<typename T>
uint16_t
ModelStorage::count()
{
    return 0;
}

template<typename T>
bool
ModelStorage::get(const uint8_t idx, T &t)
{
    (void)(idx);
    (void)(t);
    return false;
}

template<typename T>
bool
ModelStorage::get_next(const uint8_t from, T &t, uint16_t &idx)
{
    (void)(from);
    (void)(idx);
    (void)(t);
    return false;
}

template<typename T>
bool
ModelStorage::get_prev(const uint8_t from, T &t, uint16_t &idx)
{
    (void)(from);
    (void)(idx);
    (void)(t);
    return false;
}

template<typename T>
bool
ModelStorage::add(const T &t)
{
    (void)(t);
    return false;
}

template<typename T>
bool
ModelStorage::del(const char* key)
{
    (void)(key);
    return false;
}

template bool ModelStorage::is_exist<Account>(const char* key);

template uint16_t ModelStorage::count<Account>();

template bool ModelStorage::get<Account>(const uint8_t idx, Account &acc);

template bool ModelStorage::get_next<Account>(const uint8_t from, Account &acc, uint16_t &idx);

template bool ModelStorage::get_prev<Account>(const uint8_t from, Account &acc, uint16_t &idx);

template bool ModelStorage::add<Account>(const Account &acc);

template bool ModelStorage::del<Account>(const char* key);

template<>
uint32_t
ModelStorage::min_addr<Account>()
{
    return 0;
}

template<>
uint32_t ModelStorage::max_addr<Account>()
{
    return 256 * sizeof(Account);
}
