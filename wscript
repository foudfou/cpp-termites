#! /usr/bin/env python

VERSION = '0.0.1'
APPNAME = 'termites'

top = '.'
out = 'build'


def options(opt):
    opt.load('compiler_c compiler_cxx')
    opt.load('waf_unit_test')


def configure(cnf):
    cnf.load('compiler_c compiler_cxx')
    cnf.load('waf_unit_test')
    cnf.env['CC'] = ['clang']
    cnf.env['CXX'] = ['clang++']
    cnf.env.append_value('CXXFLAGS', ['-std=c++11'])
    cnf.check(
        features='cxx cxxprogram',
        cflags=['-Wall'],
    )
    cnf.find_program('ragel')


def build(bld):
    bld.recurse('src test')

    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)
