MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'
  conf.gem '../mruby-gwi'

  conf.cc do |cc|
    cc.command = "i686-w64-mingw32-gcc"
    cc.flags << '-m64'
    cc.flags << '-Wall'
  end

  conf.cxx do |cxx|
    cxx.command = "i686-w64-mingw32-g++"
    cxx.flags << '-m64'
    cxx.flags << '-Wall'
  end

  conf.linker do |linker|
    linker.command = "i686-w64-mingw32-gcc"
    linker.flags << '-m64'
    linker.flags << '-Wall'
  end

  conf.archiver.command = 'i686-w64-mingw32-ar'

end
