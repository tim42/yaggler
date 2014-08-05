#!/bin/bash
#
# a setup script for YagGLer
#

# params
use_submodules=0
show_help=0

# extraction progs
cmd=xz
ext=xz

# help
if (( $# == 1 ))
then
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
fi

#
# setup submodules
#

function fetch_and_extract # $1: project ftp path (eg: neam/tools), $2: base folder
{
  echo "fetching $1..."
  cd $2
  curl -# ftp://neam.co/projects/$1.tar.$ext | $cmd -d | tar -x
}

if [ $use_submodules = 0 ]
then
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

  # fetching and extracting modules
  fetch_and_extract 'neam/tools' 'yaggler'

else
  echo "using git submodules..."
  git submodule init
  git submodule update
fi



echo "YagGLer is now setup"


