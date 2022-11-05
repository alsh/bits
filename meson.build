
project(
  'bits',
  'cpp',
  version : '0.0.0',
  default_options : ['cpp_std=c++20']
)

project_headers = [
  'include/bitmatrix.hh',
]

project_description = 'Library to work with bit objects'

project_test_files = [
]

build_args = [
]

# ======
# Target
# ======

public_headers = include_directories('include')

# project_target = shared_library(
#   meson.project_name(),
#   [],
#   install : true,
#   c_args : build_args,
#   gnu_symbol_visibility : 'hidden',
#   include_directories : public_headers,
# )

# =======
# Project
# =======

project_dep = declare_dependency(
  include_directories: public_headers
)
set_variable(meson.project_name() + '_dep', project_dep)

# Make this library usable from the system's
# package manager.
install_headers(
  project_headers,
  subdir : meson.project_name()
)

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : meson.project_name(),
)

# ==========
# Unit Tests
# ==========

if not meson.is_subproject()
  add_languages('cpp')
  subdir('tests')
#   test('all_tests',
#     executable(
#       'run_tests',
#       files(project_test_files),
#       dependencies : [project_dep, test_dep],
#       install : false
#     )
#   )
endif
