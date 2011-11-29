#! /usr/bin/env python
# coding: utf-8

# Copyright (c) 2011, Jon Maken
# License: 3-clause BSD
# Revision: 11/26/2011 10:32:58 PM

import os
import os.path
import sys

PY_MIN_VERSION = (2, 7, 0)
WAF_VERSION = '1.6.9'

APPNAME = 'tma'
VERSION = '0.1.0'

top = '.'
out = 'build'

def options(opts):
    opts.load('compiler_cxx')
    opts.add_option('--without-samples', action='store_true', default=False,
                    help='do not build samples')

def configure(conf):
    conf.load('compiler_cxx')

    conf.env.SAMPLES = not conf.options.without_samples

    # override gcc defaults normally stored in _cache.py
    conf.env.SHLIB_MARKER = ''  # '-Wl,-Bdynamic'
    conf.env.STLIB_MARKER = ''  # '-Wl,-Bstatic'

def build(bld):

    if bld.env.CXX_NAME == 'msvc':
        my_cxxflags = [ '/O2', '/EHsc' ]
        my_linkflags = []
    else:
        my_cxxflags = [ '-Wall', '-O3', '-g' ]
        my_linkflags = [ '-s' ]

    # build driver program
    bld.program(
        source = [ 'src/tma.cpp' ],
        includes = [ 'include' ],
        target = 'tma',
        cxxflags = my_cxxflags,
        linkflags = my_linkflags,
        )

    # build samples
    if bld.env.SAMPLES == True:
        bld.recurse('samples')

# helper functions
def _prepare(args):

    import urllib2
    from contextlib import closing

    # download waf if not already present and make executable if not on Windows
    if not os.path.exists('waf'):
        with closing(urllib2.urlopen(waf.url)) as f, open('waf', 'wb') as w:
            w.write(f.read())
        if not hasattr(sys, 'winver'):
            os.chmod('waf', 0755)
        print('-> downloaded waf from %s' % waf.url)
    else:
        print('-> nothing to download; using existing waf library')


if __name__ == '__main__':
    if sys.version_info < PY_MIN_VERSION:
        print('At least Python v%d.%d.%d required, exiting...' %
            (PY_MIN_VERSION[0], PY_MIN_VERSION[1], PY_MIN_VERSION[2]))
        sys.exit(1)

    class ResourceInfo(object):
        def __init__(self, **kwargs):
            for i in kwargs.iteritems():
                setattr(self, i[0], i[1])

    args = sys.argv[:]

    TASKS = ('prepare',)
    USAGE = '''usage: python wscript TASK [OPTION]

where TASK is one of:
  prepare   prepare current dir for building tma
'''

    if len(args) != 2 or args[1].lower() not in TASKS:
        print(USAGE)
        sys.exit(1)

    waf = ResourceInfo(
        url = 'http://waf.googlecode.com/files/waf-%s' % WAF_VERSION
        )

    task = args[1].lower()

    if task == 'prepare':
        _prepare(args)

# vim: ft=python ai ts=4 sw=4 sts=4 et
