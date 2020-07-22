project('phosphor-ryan', 'cpp',
  version: '0.1', meson_version: '>=0.49.0',
  default_options: [
    'warning_level=3',
    'werror=true',
    'cpp_std=c++17'
])

sdbusplus = dependency('sdbusplus')

executable(
    'phosphor-ryan',
    'ryan.cpp',
    dependencies: [
      sdbusplus,
    ],
    implicit_include_directories: false,
    install: true,
    install_dir: get_option('bindir')
)

