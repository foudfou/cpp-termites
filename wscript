#! /usr/bin/env python
# Copyright (c) 2014 Foudil Brétel. All rights reserved.

import waflib
from waflib import Build, Logs, Options

VERSION = '0.0.1'
APPNAME = 'termites'

top = '.'
out = 'build'


def options(opt):
    opt.load('compiler_c compiler_cxx')
    opt.load('waf_unit_test')
    opt.add_option('--without-debug', action='store_false', default=True,
                   dest='no_debug', help='Add debug info to binaries')


def configure(cnf):
    cnf.recurse('src')

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
    cnf.check(header_name='getopt.h', features='cxx cxxprogram')
    cnf.find_program('ragel')
    cnf.find_program('valgrind')

    cnf.env['CC'] = ['clang']
    cnf.env['CXX'] = ['clang++']
    cnf.env.append_value('CXXFLAGS', [
        '-fcolor-diagnostics',
        '-Wall', '-pedantic', '-Wextra', '-Weffc++', '-std=c++11'
    ])

    cnf.define('PACKAGE_NAME', APPNAME)
    cnf.define('PACKAGE_VERSION', VERSION)
    cnf.define('PACKAGE_COPYRIGHT', "Copyright (c) 2014 Foudil Brétel. All rights reserved.")
    cnf.write_config_header('include/config.h')

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
    "run linters over source files"
    lint_clang(ctx)
    lint_cpplint(ctx)

def _lint_pre(ctx):
    src_files = ctx.path.ant_glob(
        ['src/**/*.cpp', 'src/Config.rl', 'include/**/*.hpp', 'test/**/*.cpp'],
        excl=['include/catch.hpp'],
        dir=False, src=True,
    )
    src_files_str = " "
    for f in src_files:
         src_files_str += " " + f.relpath()

    incl_dirs = [d[3:] if d.startswith('..') else "src/" + d for d in ctx.env.SRC_INC_DIRS]
    incl_dirs_str = ""
    for d in  incl_dirs:
         incl_dirs_str += " -I" + d

    return src_files_str, incl_dirs_str

def lint_clang(ctx):
    src_files, incl_dirs = _lint_pre(ctx)
    cmd = 'clang++ -fsyntax-only -fcolor-diagnostics '
    cmd += " ".join(ctx.env.CXXFLAGS)
    Logs.info("lint [clang]...")
    ctx.exec_command(cmd + incl_dirs + src_files)

def lint_cpplint(ctx):
    src_files, incl_dirs = _lint_pre(ctx)
    cmd = 'python2 tools/cpplint.py --extensions=hpp,cpp,rl ' \
          '--linelength=85 --filter=-whitespace/braces '
    Logs.info("lint [cpplint]...")
    ctx.exec_command(cmd + src_files)

class LintContext(Build.BuildContext):
    cmd = 'lint'
    fun = 'lint'

class LintClangContext(Build.BuildContext):
    cmd = 'lint_clang'
    fun = 'lint_clang'

class LintCpplintContext(Build.BuildContext):
    cmd = 'lint_cpplint'
    fun = 'lint_cpplint'


def valgrind(ctx):
    "run valgrind over the main binary"
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
