
add_languages('cpp')

doctest_dep = dependency('doctest', required: true)

test1_exe = executable(
  'test_bitmatrix',
  'test_bitmatrix.cc',
  cpp_args : ['-fsanitize=address,undefined,bounds'],
  link_args : ['-fsanitize=address,undefined,bounds'],
  include_directories : public_headers,
  dependencies : doctest_dep,
)

test(
  'Test bit matrix class',
  test1_exe,
)
