/**
 * AddressSanitizer:DEADLYSIGNAL
=================================================================
==1143==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000048 (pc 0x000000503501 bp 0x7ffc4ed5feb0 sp
0x7ffc4ed5fe90 T0)
==1143==The signal is caused by a READ memory access.
==1143==Hint: address points to the zero page.
    #0 0x503501 in bustub::TrieNodeWithValue<int>::GetValue() const
/autograder/bustub/src/include/primer/p0_trie.h:239:31 #1 0x505a42 in int
bustub::Trie::GetValue<int>(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&,
bool*) /autograder/bustub/src/include/primer/p0_trie.h:427:63 #2 0x4ee83e in
bustub::StarterTrieTest_InsertTest_Test::TestBody() /autograder/bustub/test/primer/grading_starter_trie_test.cpp:266:10
    #3 0x5f8856 in void testing::internal::HandleSehExceptionsInMethodIfSupported<testing::Test, void>(testing::Test*,
void (testing::Test::*)(), char const*) /autograder/bustub/third_party/googletest/googletest/src/gtest.cc:2599:10 #4
0x5a7782 in void testing::internal::HandleExceptionsInMethodIfSupported<testing::Test, void>(testing::Test*, void
(testing::Test::*)(), char const*) /autograder/bustub/third_party/googletest/googletest/src/gtest.cc:2635:14 #5 0x55cb5c
in testing::Test::Run() /autograder/bustub/third_party/googletest/googletest/src/gtest.cc:2674:5 #6 0x55e824 in
testing::TestInfo::Run() /autograder/bustub/third_party/googletest/googletest/src/gtest.cc:2853:11 #7 0x55feca in
testing::TestSuite::Run() /autograder/bustub/third_party/googletest/googletest/src/gtest.cc:3012:30 #8 0x58380c in
testing::internal::UnitTestImpl::RunAllTests() /autograder/bustub/third_party/googletest/googletest/src/gtest.cc:5870:44
    #9 0x600a66 in bool testing::internal::HandleSehExceptionsInMethodIfSupported<testing::internal::UnitTestImpl,
bool>(testing::internal::UnitTestImpl*, bool (testing::internal::UnitTestImpl::*)(), char const*)
/autograder/bustub/third_party/googletest/googletest/src/gtest.cc:2599:10 #10 0x5ad802 in bool
testing::internal::HandleExceptionsInMethodIfSupported<testing::internal::UnitTestImpl,
bool>(testing::internal::UnitTestImpl*, bool (testing::internal::UnitTestImpl::*)(), char const*)
/autograder/bustub/third_party/googletest/googletest/src/gtest.cc:2635:14 #11 0x582b48 in testing::UnitTest::Run()
/autograder/bustub/third_party/googletest/googletest/src/gtest.cc:5444:10 #12 0x608420 in RUN_ALL_TESTS()
/autograder/bustub/third_party/googletest/googletest/include/gtest/gtest.h:2293:73 #13 0x608371 in main
/autograder/bustub/third_party/googletest/googlemock/src/gmock_main.cc:70:10 #14 0x7f492ec94d8f in
__libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16 #15 0x7f492ec94e3f in __libc_start_main
csu/../csu/libc-start.c:392:3 #16 0x42af64 in _start (/autograder/bustub/build/test/grading_starter_trie_test+0x42af64)

AddressSanitizer can not provide additional info.
        SUMMARY: AddressSanitizer: SEGV /autograder/bustub/src/include/primer/p0_trie.h:239:31 in
bustub::TrieNodeWithValue<int>::GetValue() const
    ==1143==ABORTING

                       Called process exited with 1
 *
 *
 * */