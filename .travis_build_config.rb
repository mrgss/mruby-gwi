MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'
  conf.gem '../mruby-gwi'

  conf.cc do |cc|
    cc.command = "i686-w64-mingw32-gcc"
  end

  conf.cxx do |cxx|
    cxx.command = "i686-w64-mingw32-g++"
  end

end
