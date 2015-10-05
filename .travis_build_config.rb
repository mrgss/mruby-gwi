MRuby::Build.new do |conf|
  toolchain "gcc-mingw-64"
  conf.gembox 'default'
  conf.gem '../mruby-gwi'
end
