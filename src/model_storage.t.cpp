#include "model_storage.hpp"
#include "memory_block_storage.cpp"
#include <AUnit.h>
#include "model.hpp"
#include <string.h>

test(model_storage_ctor)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage model(bs);
};

test(model_storage_add)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage m(bs);

    Account acc {.name = "n", .username = "u", .password = "p"};

    assertEqual(0, m.count<Account>());
    assertFalse(m.is_exist<Account>(acc.name));

    assertTrue(m.add(acc));

    assertEqual(1, m.count<Account>());
    assertTrue(m.is_exist<Account>(acc.name));
};

test(model_storage_get)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage m(bs);

    Account acc0 {.name = "n", .username = "u", .password = "p"};

    assertTrue(m.add(acc0));

    Account acc1;
    assertTrue(m.get(0, acc1));

    assertStringCaseEqual(acc0.name, acc1.name);
};

test(model_storage_get_next_prev)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage m(bs);

    Account acc0 {.name = "n0", .username = "u0", .password = "p0"};
    assertTrue(m.add(acc0));

    Account acc1 {.name = "n1", .username = "u1", .password = "p1"};
    assertTrue(m.add(acc1));

    Account acc;
    memset(&acc, 0, sizeof(acc));

    uint16_t idx = 0;
    //
    // Next
    assertTrue(m.get_next(0, acc, idx));

    assertEqual(1, idx);
    assertStringCaseEqual(acc1.name, acc.name);
    //
    // Prev
    memset(&acc, 0, sizeof(acc));

    assertTrue(m.get_prev(idx, acc, idx));

    assertEqual(0, idx);
    assertStringCaseEqual(acc0.name, acc.name);
};

test(model_storage_del_one_object)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage m(bs);

    Account acc0 {.name = "n0", .username = "u0", .password = "p0"};
    assertTrue(m.add(acc0));
    assertTrue(m.del<Account>(acc0.name));

    assertEqual(0, m.count<Account>());
};

test(model_storage_del_three_objects_fifo)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage m(bs);

    Account acc0 {.name = "n0", .username = "u0", .password = "p0"};
    assertTrue(m.add(acc0));

    Account acc1 {.name = "n1", .username = "u1", .password = "p1"};
    assertTrue(m.add(acc1));

    Account acc2 {.name = "n2", .username = "u2", .password = "p2"};
    assertTrue(m.add(acc2));

    assertEqual(3, m.count<Account>());

    assertTrue(m.del<Account>(acc0.name));
    assertEqual(2, m.count<Account>());

    assertTrue(m.del<Account>(acc1.name));
    assertEqual(1, m.count<Account>());

    assertTrue(m.del<Account>(acc2.name));
    assertEqual(0, m.count<Account>());
};

test(model_storage_del_three_objects_lifo)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage m(bs);

    Account acc0 {.name = "n0", .username = "u0", .password = "p0"};
    assertTrue(m.add(acc0));

    Account acc1 {.name = "n1", .username = "u1", .password = "p1"};
    assertTrue(m.add(acc1));

    Account acc2 {.name = "n2", .username = "u2", .password = "p2"};
    assertTrue(m.add(acc2));

    assertTrue(m.del<Account>(acc2.name));
    assertEqual(2, m.count<Account>());

    assertTrue(m.del<Account>(acc1.name));
    assertEqual(1, m.count<Account>());

    assertTrue(m.del<Account>(acc0.name));
    assertEqual(0, m.count<Account>());
};

test(model_storage_get_next_prev_over_free_spot)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage m(bs);

    Account acc0 {.name = "n0", .username = "u0", .password = "p0"};
    assertTrue(m.add(acc0));

    Account acc1 {.name = "n1", .username = "u1", .password = "p1"};
    assertTrue(m.add(acc1));

    Account acc2 {.name = "n2", .username = "u2", .password = "p2"};
    assertTrue(m.add(acc2));

    assertTrue(m.del<Account>(acc1.name));

    Account acc;
    memset(&acc, 0, sizeof(acc));

    uint16_t idx = 0;
    //
    // Next
    assertTrue(m.get_next(0, acc, idx));

    assertEqual(2, idx);
    assertStringCaseEqual(acc2.name, acc.name);
    //
    // Prev
    memset(&acc, 0, sizeof(acc));

    assertTrue(m.get_prev(idx, acc, idx));

    assertEqual(0, idx);
    assertStringCaseEqual(acc0.name, acc.name);
};

test(model_storage_add_del_and_add_to_fill_free_spot)
{
    MemoryBlockStorage<1024, 64> bs;
    ModelStorage m(bs);

    Account acc0 {.name = "n0", .username = "u0", .password = "p0"};
    assertTrue(m.add(acc0));

    Account acc1 {.name = "n1", .username = "u1", .password = "p1"};
    assertTrue(m.add(acc1));

    Account acc2 {.name = "n2", .username = "u2", .password = "p2"};
    assertTrue(m.add(acc2));

    assertTrue(m.del<Account>(acc1.name));
    assertEqual(2, m.count<Account>());

    Account acc3 {.name = "n3", .username = "u3", .password = "p3"};
    assertTrue(m.add(acc3));
    assertEqual(3, m.count<Account>());

    Account acc;
    memset(&acc, 0, sizeof(acc));

    uint16_t idx = 0;
    //
    // Next
    assertTrue(m.get_next(0, acc, idx));

    assertEqual(1, idx);
    assertStringCaseEqual(acc3.name, acc.name);
};
