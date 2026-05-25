#ifndef __OBJ_MODEL_STORAGE__
#define __OBJ_MODEL_STORAGE__

#include <inttypes.h>

class BlockStorage;

class ModelStorage
{
public:
    ModelStorage(BlockStorage &bs) : bs_(bs) {};
    ~ModelStorage() = default;

    template<typename T>
    bool is_exist(const char* key);

    template<typename T>
    uint16_t count();

    template<typename T>
    bool get(const uint8_t idx, T &t);

    template<typename T>
    bool get_next(const uint8_t from, T &t, uint16_t &idx);

    template<typename T>
    bool get_prev(const uint8_t from, T &t, uint16_t &idx);

    template<typename T>
    bool add(const T &t);

    template<typename T>
    bool del(const char* key);

protected:
    template<typename T>
    uint32_t min_addr();

    template<typename T>
    uint32_t max_addr();

protected:
    BlockStorage &bs_;
};

#endif // !__OBJ_MODEL_STORAGE__
