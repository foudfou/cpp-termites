#! /usr/bin/env python

VERSION = '0.0.1'
APPNAME = 'termites'

top = '.'
out = 'build'

import waflib
from waflib import Build, Logs, Options


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
        cflags=['-Wall', '-Weffc++'],
    )
    cnf.find_program('ragel')
    cnf.find_program('valgrind')

    cnf.env['CC'] = ['clang']
    cnf.env['CXX'] = ['clang++']
    cnf.env.append_value('CXXFLAGS', ['-Wall', '-pedantic', '-Wextra', '-Weffc++', '-std=c++11'])


def build(bld):
    bld.recurse('src test')

    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)


def tags(ctx):
    cmd = 'find src include test -type f -name "*.c" -o -name "*.h"' \
          '-o -name "*.cpp" -o -name "*.hpp" -o -name "Config.rl"' \
          '| etags -'
    ctx.exec_command(cmd)


def lint(ctx):
    sources = ctx.path.ant_glob(
        ['src/**/*.cpp', 'include/**/*.hpp', 'test/**/*.cpp'], dir=False, src=True)
    sources_str = " ".join([f.abspath() for f in sources])

    cmd = 'clang++ -fsyntax-only -fcolor-diagnostics '
    # TODO: include should be defined at conf level, and re-used here
    cmd += " ".join(ctx.env.CXXFLAGS) + " -Iinclude "
    Logs.info("lint [clang]...")
    ctx.exec_command(cmd + sources_str)

    cmd = 'python2 tools/cpplint.py '
    Logs.info("lint [cpplint]...")
    ctx.exec_command(cmd + sources_str)


class LintContext(Build.BuildContext):
    cmd = 'lint'
    fun = 'lint'


def valgrind(ctx):
    # # TODO: add depency to `build` task
    # ctx.add_manual_dependency(
    #     ctx.path.find_node('src/main.cpp'),
    #     ctx.path.find_node('src/main.cpp'))

    Logs.info("valgrind [termites]...")
    cmd = 'valgrind --leak-check=full ./build/src/termites'
    return ctx.exec_command(cmd)


# help here: https://github.com/lycus/mci/blob/master/wscript
class ValgrindContext(Build.BuildContext):
    cmd   = 'valgrind'
    fun   = 'valgrind'
    after = ['termites']
