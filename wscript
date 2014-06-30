#! /usr/bin/env python

VERSION = '0.0.1'
APPNAME = 'termites'

top = '.'
out = 'build'

from waflib import Logs


def options(opt):
    opt.load('compiler_c compiler_cxx')
    opt.load('waf_unit_test')
    opt.add_option('--without-debug', action='store_false', default=True,
                   dest='no_debug', help='Add debug info to binaries')


def configure(cnf):
    release  = 'Release'
    if cnf.options.no_debug:
        release = 'Debug'
        cnf.env.append_value('CXXFLAGS', ['-g'])
    cnf.msg("Compilation mode", release)

    cnf.load('compiler_c compiler_cxx')
    cnf.load('waf_unit_test')
    cnf.check(
        features='cxx cxxprogram',
        cflags=['-Wall'],
    )
    cnf.find_program('ragel')
    cnf.find_program('valgrind')

    cnf.env['CC'] = ['clang']
    cnf.env['CXX'] = ['clang++']
    cnf.env.append_value('CXXFLAGS', ['-Wall', '-std=c++11'])


def build(bld):
    bld.recurse('src test')

    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)
