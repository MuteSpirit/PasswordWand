#ifndef __OBJ_MODEL_STORAGE__
#define __OBJ_MODEL_STORAGE__

#include <cstddef>
#include <cstdint>

// TODO: use size_t instead of uint32_t as address type

class BlockStorage;


template<typename Object>
class ModelStorage
{
public:
    using ObjIndex = uint16_t;

public:
    ModelStorage(BlockStorage &bs) : bs_(bs) {};
    ~ModelStorage() = default;

    bool is_exist(const char* key);

    ObjIndex count();

    bool get(const ObjIndex idx, Object &t);
    bool get_next(const ObjIndex from, Object &t, ObjIndex &idx);
    bool get_prev(const ObjIndex from, Object &t, ObjIndex &idx);
    bool add(const Object &t);
    bool del(const char* key);

protected:
    /// @brief Store not only original model object but also special flag.
    /// meaning fully written data about object.
    /// Before any edit operation with object "commitFlag_" must be set to 0x0.
    /// After finish operation set 0x1 again.
    struct __attribute__((packed)) ObjInStorage
    {
        enum class Committment : uint8_t
        {
            free     = 0,
            comitted = 1,
            COUNT
        };

        Object obj_;
        uint8_t reserve_[15];
        uint8_t commitFlag_;
    };

protected:
    bool is_ok_idx(const ObjIndex idx);
    bool is_free_account(const ObjIndex idx);
    bool get_free_object_index(ObjIndex &idx);
    size_t idx2addr(const ObjIndex idx) const;
    size_t get_key_addr(const ObjIndex idx);
    size_t min_addr() const; // TODO: move into BlockStorage
    size_t max_addr() const; // TODO: move into BlockStorage

protected:
    BlockStorage &bs_;
};

#endif // !__OBJ_MODEL_STORAGE__
