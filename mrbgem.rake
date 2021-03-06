MRuby::Gem::Specification.new('mruby-gwi') do |spec|
  spec.license = 'MIT'
  spec.author  = 'mruby developers'

  # Add compile flags
  # spec.cc.flags << '-g'
  spec.cc.flags << '-DUNICODE'
  spec.cc.flags << '-D_UNICODE'
  
  # Add cflags to all
  # spec.mruby.cc.flags << '-g'

  # Add libraries
  # spec.linker.libraries << 'external_lib'
  spec.linker.libraries << 'Comdlg32'
  spec.linker.libraries << 'comctl32'
  spec.linker.libraries << 'Gdi32'
  
  # Default build files
  # spec.rbfiles = Dir.glob("#{dir}/mrblib/*.rb")
  # spec.objs = Dir.glob("#{dir}/src/*.{c,cpp,m,asm,S}").map { |f| objfile(f.relative_path_from(dir).pathmap("#{build_dir}/%X")) }
  # spec.test_rbfiles = Dir.glob("#{dir}/test/*.rb")
  # spec.test_objs = Dir.glob("#{dir}/test/*.{c,cpp,m,asm,S}").map { |f| objfile(f.relative_path_from(dir).pathmap("#{build_dir}/%X")) }
  # spec.test_preload = 'test/assert.rb'

  # Values accessible as TEST_ARGS inside test scripts
  # spec.test_args = {'tmp_dir' => Dir::tmpdir}
end
