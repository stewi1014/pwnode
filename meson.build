project(
    'pwnode',
    'c',
    version: '1.0',
    meson_version: '>=1.5.0',
    default_options: ['c_std=gnu23'],
)

gnome = import('gnome')

gtk_dep = dependency('gtk4', version: '>=4.14')
pipewire_dep = dependency('libpipewire-0.3')

sources = [
    'src' / 'pwnode.c',
    gnome.compile_resources('resources', 'resources/resources.xml', source_dir: 'resources'),
]

executable(
    meson.project_name(),
    sources: sources,
    dependencies: [gtk_dep, pipewire_dep],
    install: true,
)

install_data(
    'install' / 'pwnode.desktop',
    install_dir: get_option('datadir') / 'applications',
    rename: meson.project_name() + 'desktop',
)