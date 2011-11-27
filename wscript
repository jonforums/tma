#! /usr/bin/env python
# coding: utf-8

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

def configure(conf):
    conf.load('compiler_cxx')

    # override gcc defaults normally stored in _cache.py
    conf.env.SHLIB_MARKER = ''  # '-Wl,-Bdynamic'
    conf.env.STLIB_MARKER = ''  # '-Wl,-Bstatic'

def build(bld):
    bld.program(
        source = [ 'samples/mem_alloc.cpp' ],
        includes = [ 'include' ],
        target = 'mem_alloc',
        cxxflags = [ '-Wall', '-O3' ],
        linkflags = '-s',
        )

# helper functions
def _prepare(args):

    import urllib2
    from contextlib import closing

    # download waf if not already present
    if not os.path.exists('waf'):
        with closing(urllib2.urlopen(waf.url)) as f, open('waf', 'wb') as w:
            w.write(f.read())
        print('-> downloaded waf from %s' % waf.url)


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
