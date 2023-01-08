#!/usr/bin/python3

import subprocess
import sys
import os
import datetime

if os.path.exists('src/changeset.cpp'):
    os.remove('src/changeset.cpp')

def ord(n):
    return str(n)+("th" if 4<=n%100<=20 else {1:"st",2:"nd",3:"rd"}.get(n%10, "th"))
def dtStylish(dt,f):
    return dt.strftime(f).replace("{th}", ord(dt.day))

commands = [
        'hg',
        '/usr/bin/hg',
        '/c/applications/development/vcs/tortoisehg/hg.exe',
        '/mnt/c/applications/development/vcs/tortoisehg/hg.exe',
        'c:/applications/development/vcs/tortoisehg/hg.exe',
        ]

have_changeset = False
for command in commands:
    try:
        p1 = subprocess.run([command, 'id', '-i'],
                stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                check=True, encoding='utf8')
        p2 = subprocess.run([command, 'log', '-r', '.', '--template', '{latesttag}-{latesttagdistance}-{node|short}'],
                stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                check=True, encoding='utf8')
    except (subprocess.CalledProcessError, FileNotFoundError):
        continue
    changeset = p1.stdout.strip()
    version = p2.stdout.strip()
    have_changeset = True
    break

if not have_changeset:
    # hg wasn't found.  Try git
    try:
        p = subprocess.run(['git', 'rev-parse', 'HEAD'],
                    stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                    check=True, encoding='utf8')
        changeset = 'git-' + p.stdout.strip()
        version = changeset
        have_changeset = True
    except (subprocess.CalledProcessError, FileNotFoundError):
        pass

if not have_changeset:
    print("ERROR: Could not determine changeset, assuming dev version", file=sys.stderr)
    changeset = 'none'
    version = changeset
    have_changeset = True

with open('src/changeset.cpp', 'w', encoding='utf8') as fh:
    fh.write('const char * changeset = "' + changeset + '";\n')
    fh.write('const char * version = "' + version + '";\n')
    fh.write('const char * builddate = "Compiled ' +
            dtStylish(datetime.datetime.now(), "{th} %B %Y") + '";\n')
