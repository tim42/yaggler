#!/bin/bash
#
# a setup script for YagGLer
#

use_submodules=0
show_help=0

case $1 in
  '--use-git-submodules')
    use_submodules=1
  ;;
  '--fetch-submodules')
    use_submodules=0
  ;;
  '--help')
    show_help=1
  ;;
  *)
    echo 'unknow option "'$1'"...'
    show_help=1
  ;;
esac

if [ $show_help = 1 ]
then
  echo "$0 options:"
  echo "  --use-git-submodules : use a git submodule for neam/tools (need authentification)"
  echo "  --fetch-submodules   : do not use a git submodules for neam/tools, but instead fetch the ftp server (default)"
  exit 0
fi


#
# setup submodules
#

if [ $use_submodules = 0 ]
then
  cmd=xz
  ext=xz

  if ! hash $cmd 2>/dev/null
  then
    cmd=bzip2
    ext=bz2

    if ! hash $cmd 2>/dev/null
    then
      echo 1>&2 "missing xz or bzip2 commands... :("
      exit 1
    fi
  fi

  echo "fecthing neam/tools..."
  cd yaggler/
  curl ftp://neam.co/projects/neam/tools.tar.$ext | $cmd -d | tar -x
else
  git submodule init
  git submodule update
fi



echo "YagGLer is now setup"


