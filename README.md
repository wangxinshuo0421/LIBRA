# LIBRA simulation
This is an NS-3 simulator for LIBRA: Multi-Path Transport in Distributed Training Data Center Network (ICNP' 2025). It also includes the implementation of HPCC, DCQCN, TIMELY, DCTCP, PFC, ECN and Broadcom shared buffer switch.

This project is based on the [HPCC NS3](https://github.com/alibaba-edu/High-Precision-Congestion-Control) code framework and supports additional features such as **multi-path spray, out-of-order rdma packet direct write, sender bitmap, selective repeat, switch congestion notification, and path based congestion control**.

## NS-3 simulation
The ns-3 simulation is under `simulation/`. Refer to the README.md under it for more details.

## Traffic generator
The traffic generator is under `traffic_gen/`. Refer to the README.md under it for more details.

## Analysis
We provide a few analysis scripts under `analysis/` to view the packet-level events, and analyzing the fct in the same way as [HPCC](https://liyuliang001.github.io/publications/hpcc.pdf) Figure 11.
Refer to the README.md under it for more details.

## Questions
For technical questions, please create an issue in this repo, so other people can benefit from your questions. 
You may also check the issue list first to see if people have already asked the questions you have :)

## config env for ubuntu 20.04
sudo apt-get -y install gcc g++ python python-dev
sudo apt-get -y install mercurial
sudo apt-get -y install bzr
sudo apt-get -y install gdb valgrind
sudo apt-get -y install gsl-bin libgsl-dev libgslcblas0
sudo apt-get -y install flex bison libfl-dev
sudo apt-get -y install g++-3.4 gcc-3.4
sudo apt-get -y install tcpdump
sudo apt-get -y install sqlite sqlite3 libsqlite3-dev
sudo apt-get -y install libxml2 libxml2-dev
sudo apt-get -y install libgtk2.0-0 libgtk2.0-dev
sudo apt-get -y install vtun lxc
sudo apt-get -y install uncrustify
sudo apt-get -y install doxygen graphviz imagemagick
sudo apt-get -y install python-sphinx dia
sudo easy_install -U Sphinx
sudo apt-get -y install libboost-signals-dev libboost-filesystem-dev
sudo apt-get -y install openmpi*


## config env for ubuntu 18.04
sudo apt-get install -y --assume-yes xfce4
sudo apt-get install -y g++ python3
sudo apt-get install -y g++ python3 python3-dev pkg-config sqlite3
sudo apt-get install -y python3-setuptools git
sudo apt-get install -y qt5-default mercurial
sudo apt-get install -y gir1.2-goocanvas-2.0 python-gi python-gi-cairo python-pygraphviz python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython ipython3
sudo apt-get install -y openmpi-bin openmpi-common openmpi-doc libopenmpi-dev
sudo apt-get install -y autoconf cvs bzr unrar
sudo apt-get install -y gdb valgrind
sudo apt-get install -y uncrustify
sudo apt-get install -y doxygen graphviz imagemagick
sudo apt-get install -y texlive texlive-extra-utils texlive-latex-extra texlive-font-utils dvipng latexmk
sudo apt-get install -y python3-sphinx dia
sudo apt-get install -y gsl-bin libgsl-dev libgsl23 libgslcblas0
sudo apt-get install -y tcpdump
sudo apt-get install -y sqlite sqlite3 libsqlite3-dev
sudo apt-get install -y libxml2 libxml2-dev
sudo apt-get install -y cmake libc6-dev libc6-dev-i386 libclang-6.0-dev llvm-6.0-dev automake pippython3 -m pip install -y --user cxxfilt
sudo apt-get install -y libgtk-3-dev
sudo apt-get install -y vtun lxc uml-utilities
sudo apt-get install -y libboost-signals-dev libboost-filesystem-dev

sudo apt install -y cmake
sudo apt install -y python3-pip
pip3 install distro


For other questions, please contact Xinshuo Wang (wangxs@dsp.ac.com).
