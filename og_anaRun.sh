#!/bin/bash

#    1 2 3 4 5 6 7
#  (1) run number
#  (2) theta
#  (3) phi
#  (4) time window  (default 1)
#  (5) cluster size max  (default 2)
#  (6) max events to analyze
#  (7) character(s) to attach at the end of file name (for multi times processing)

set -e

export DYLD_LIBRARY_PATH=/Applications/root_v6.16.00/lib  # this line is needed for sk's mac.

echo compile
c++  `root-config --cflags` -o anabin anaWTP_v03a.cc   `root-config --glibs`
# c++  `root-config --cflags` -o anabin anaWTP_v03a_skipNode3.cc   `root-config --glibs`
echo run
./anabin  rawdata5_run$1.txt  histWTP_$1_$2_$3_$4_$5_$6_$7.root $2 $3 $4 $5 $6

echo done!

cp histWTP_$1_$2_$3_$4_$5_$6_$7.root root2tex/
cd root2tex/

echo In $PWD

c++ `root-config --cflags` -o root2tex_v9 root2tex_v9.cc `root-config --glibs`
echo run root2tex
./root2tex_v9  wta_hlist_02a.txt  histWTP_$1_$2_$3_$4_$5_$6_$7.root

pdflatex plots_histWTP_$1_$2_$3_$4_$5_$6_$7.tex

open plots_histWTP_$1_$2_$3_$4_$5_$6_$7.pdf

echo PDF created
cd ..
