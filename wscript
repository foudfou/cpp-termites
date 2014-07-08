#! /usr/bin/env python
# Copyright (c) 2014 Foudil Brétel. All rights reserved.

import waflib
from waflib import Build, Logs, Options

VERSION = '0.0.1'
APPNAME = 'termites'


def _sysinfo():
    import os
    u = os.uname()
    return "%s-%s" % (u.sysname.lower(), u.machine)

top = '.'
out = 'obj-%s' % _sysinfo()


def options(opt):
    opt.load('compiler_c compiler_cxx')
    opt.load('waf_unit_test')

    cnf = opt.get_option_group('configure options')
    cnf.add_option('-c', '--compiler', type='choice', choices=['clang', 'gcc'],
                   default='clang', action='store',
                   help='define compiler to use. clang or gcc [default: clang]')
    cnf.add_option('--with-effc++', action='store_true', default=False,
                   dest='eff_cpp', help='Thorough diagnostics')
    cnf.add_option('--without-debug', action='store_false', default=True,
                   dest='no_debug', help='Add debug info to binaries')
    cnf.add_option('--coverage', action='store_true', default=False,
                   dest='coverage', help='Compile and run coverage')

    bld = opt.get_option_group('build and install options')
    bld.add_option('--valgrind', type='string', action='store', default=False,
                   dest='valgrind', help='Run valgrind with arguments [string]')

    lnt = opt.add_option_group('lint options')
    lnt.add_option('-l', '--linters', type='string', action='store',
                   default=False, dest='linters',
                   help=('Coma-spearated list of linters to apply '
                         '[cppcheck,clang,cpplint]'))

def configure(cnf):
    cnf.recurse('src test')

    if cnf.options.compiler == 'clang':
        cnf.env['CC'] = 'clang'
        cnf.env['CXX'] = 'clang++'
    else:
        cnf.env['CC'] = 'gcc'
        cnf.env['CXX'] = 'g++'

    cnf.load('compiler_c compiler_cxx')
    cnf.load('waf_unit_test')
    cnf.check(
        features='cxx cxxprogram',
        cflags=['-Wall', '-std=c++11'],
    )
    cnf.check(header_name='getopt.h', features='cxx cxxprogram')
    cnf.find_program('ragel')
    cnf.find_program('cppcheck', var='CPPCHECK')
    cnf.find_program('valgrind', var='VALGRIND')
    cnf.find_program('lcov', var='LCOV')
    cnf.find_program('genhtml', var='GENHTML')

    release  = 'Release'
    if cnf.options.no_debug:
        release = 'Debug'
        cnf.env.append_value('CXXFLAGS', ['-g'])
    cnf.msg("Compilation mode", release)

    cnf.env.append_value('CXXFLAGS', [
        '-Wall', '-pedantic', '-Wextra', '-std=c++11'
    ])
    if cnf.options.compiler == 'clang':
        cnf.env.append_value('CXXFLAGS', ['-fcolor-diagnostics'])
    if cnf.options.eff_cpp:
        cnf.env.append_value('CXXFLAGS', ['-Weffc++'])

    if cnf.options.coverage:
        cnf.env.append_value('CXXFLAGS', ['--coverage'])
        cnf.env.LCOV_DIR = cnf.path.find_dir('data').abspath() + '/cov'
        if cnf.options.compiler == 'clang':
            cnf.env.append_value('STLIBPATH', ['/usr/lib/clang/3.4.2/lib/linux'])
            cnf.env.append_value('STLIB', ['clang_rt.profile-x86_64'])
        else:
            cnf.env.append_value('LIB', ['gcov'])

    cnf.define('PACKAGE_NAME', APPNAME)
    cnf.define('PACKAGE_VERSION', VERSION)
    cnf.define('PACKAGE_COPYRIGHT', "Copyright (c) 2014 Foudil Brétel. All rights reserved.")
    cnf.write_config_header('include/config.h')


def build(bld):
    bld.recurse('src test')

    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)


from waflib import Scripting
def distclean(ctx):
    Scripting.distclean(ctx)


def tags(ctx):
    cmd = 'find src include test -type f -name "*.c" -o -name "*.h"' \
          ' -o -name "*.cpp" -o -name "*.hpp" -o -name "Config.rl"' \
          ' | etags -'
    ctx.exec_command(cmd)


def lint(ctx):
    "run linters over source files"
    if ctx.options.linters:
        for linter in ctx.options.linters.split(','):
            globs = globals()
            fn = 'lint_' + linter
            if (fn in globs):
                globs[fn](ctx)
            else:
                Logs.error("linter %s not supported." % linter)

def _lint_pre(ctx):
    src_files = ctx.path.ant_glob(
        ['src/**/*.cpp', 'src/Config.rl', 'include/**/*.hpp', 'test/**/*.cpp'],
        excl=['include/ext/*'],
        dir=False, src=True,
    )
    src_files_str = " "
    for f in src_files:
         src_files_str += " " + f.relpath()

    incl_dirs = [d[3:] if d.startswith('..') else "src/" + d
                 for d in ctx.env.SRC_INC_DIRS]
    incl_dirs_str = ""
    for d in  incl_dirs:
         incl_dirs_str += " -I" + d

    return src_files_str, incl_dirs_str

def lint_cppcheck(ctx):
    src_files, incl_dirs = _lint_pre(ctx)
    # NOTE(fb): we currently can't avoid catch.hpp warning without not
    # processing tests... see http://5.150.254.56:443/trac-cppcheck/ticket/3390
    cmd = 'cppcheck --enable=all --inconclusive --std=posix '
    cmd += incl_dirs + src_files
    Logs.info("lint [cppcheck]...")
    Logs.debug(cmd)
    # ctx.exec_command() is somehow not flushing instantly...
    import subprocess
    ret = subprocess.Popen(cmd, shell=True).wait()
    if ret != 0:
        ctx.fatal(str(cmd) + ' exited with: ' + str(ret))

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
