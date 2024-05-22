add_test( test_std.test_std /home/fhyq/Program/Carinfo-Manager/server/test/test_std [==[--gtest_filter=test_std.test_std]==] --gtest_also_run_disabled_tests)
set_tests_properties( test_std.test_std PROPERTIES WORKING_DIRECTORY /home/fhyq/Program/Carinfo-Manager/server/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set( test_std_TESTS test_std.test_std)
