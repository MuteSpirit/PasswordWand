#if defined(EPOXY_DUINO)
// #include "model_storage.cpp"
// #include "encrypted_block_storage.hpp"
// #include "memory_block_storage.cpp"
// #include <AUnit.h>
// #include "model.hpp"
//
// test(encrypted_model_storage_ctor)
// {
//     MemoryBlockStorage<1024, 64> bs;
//     EncryptedBlockStorage ebs(bs);
//     ModelStorage model(ebs);
// };
//
// test(encrypted_model_storage_add)
// {
//     MemoryBlockStorage<1024, 64> bs;
//     EncryptedBlockStorage ebs(bs);
//     ModelStorage m(ebs);
//
//     Account acc {.name = "n", .username = "u", .password = "p"};
//
//     assertEqual(0, m.count<Account>());
//     assertFalse(m.is_exist<Account>(acc.name));
//
//     assertTrue(m.add(acc));
//
//     assertEqual(1, m.count<Account>());
//     assertTrue(m.is_exist<Account>(acc.name));
// };
#endif // EPOXY_DUINO
