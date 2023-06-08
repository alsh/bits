
project(
  'bits',
  'cpp',
  version : '0.0.0',
  default_options : ['cpp_std=c++20']
)

project_headers = [
  'include/bitmatrix.hh',
]

project_description = 'Library to work with bit objects '

project_test_files = [
]

build_args = [
]

public_headers = include_directories('include')
project_dep = declare_dependency(
  include_directories: public_headers
)

# Install headers
install_headers(
  project_headers,
  subdir : meson.project_name()
)

pkg_mod = import('pkgconfig')

pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  # subdirs : meson.project_name(),
)

# ==========
# Unit Tests
# ==========

if not meson.is_subproject()
  subdir('tests')
endif
