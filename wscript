# dear emacs, this is -*- python -*-

VERSION = '0.0.1'
APPNAME = 'termites'

top = '.'
out = 'build'

def options(opt):
    opt.load('compiler_c compiler_cxx')

def configure(cnf):
    cnf.env['CC'] = ['clang']
    cnf.env['CXX'] = ['clang++']
    cnf.load('compiler_c compiler_cxx')
    cnf.env.append_value('CXXFLAGS', ['-std=c++11'])
    cnf.check(
        features='cxx cxxprogram',
        cflags=['-Wall'],
    )

def build(bld):
    bld.program(
        features = 'cxx cxxprogram',
        source   = bld.path.ant_glob('src/*.cpp'),
        target   = 'termites',
        # includes = 'src',
    )

def foo(ctx):
    print(type(ctx))
