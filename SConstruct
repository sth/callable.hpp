
test_src = Split('''
   test/driver.cpp
   test/function_test.cpp
   test/callable_test.cpp
   ''')

env_test = Environment()
env_test.Append(
		CPPPATH = '.',
		CPPDEFINES = ['BOOST_TEST_DYN_LINK'],
		CXXFLAGS = ['-std=c++11'],
		LIBS = ['boost_unit_test_framework']
	)
env_test.Program('callable_test', test_src)

