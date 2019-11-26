#code to compile different ana code

c++  `root-config --cflags` -o anabinsm anaWTP_sm.cc   `root-config --glibs`
c++  `root-config --cflags` -o anabin16 anaWTP_16.cc   `root-config --glibs`
c++  `root-config --cflags` -o anabin12 anaWTP_12.cc   `root-config --glibs`
c++  `root-config --cflags` -o anabin13 anaWTP_13.cc   `root-config --glibs`
c++  `root-config --cflags` -o anabinnm anaWTP_nm.cc   `root-config --glibs`
c++  `root-config --cflags` -o anabinpm anaWTP_pm.cc   `root-config --glibs`
