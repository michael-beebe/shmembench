# amd64
#FROM --platform=linux/amd64 archlinux:base-devel
# arm
FROM ljmf00/archlinux

RUN uname -m

WORKDIR /scratch

RUN pacman -Sy
RUN yes | pacman -S     \
    base-devel          \
    cmake               \
    flex                \
    openpmix            \
    gcc                 \
    git                 \
    libev               \
    make

# Build libfabric
RUN git clone https://github.com/ofiwg/libfabric.git libfabric
WORKDIR /scratch/libfabric
RUN git checkout 159219639b7fd69d140892120121bbb4d694e295
# From arch's libfabric PKGBUILD
RUN ./autogen.sh
RUN autoreconf -fvi
RUN ./configure --prefix=/scratch/libfabric-bin \
                --enable-tcp=yes
RUN make -j$(nproc)
RUN make install

# Build SOS
WORKDIR /scratch
RUN git clone https://github.com/Sandia-OpenSHMEM/SOS.git sos
WORKDIR /scratch/sos
#RUN git submodule update --init # for trunk
RUN git checkout e616ba00c21fe7983840527ec3abea0672fdf003 # for shmem 1.5
#RUN git checkout 1f89a0f04f1e303c09b3f482d1476adab1c21691 # for shmem 1.4
RUN ./autogen.sh
RUN ./configure --prefix=/scratch/sos-bin         \
                --with-ofi=/scratch/libfabric-bin \
                --enable-pmi-simple
RUN make -j$(nproc)
RUN make install
ENV PATH=$PATH:/scratch/sos-bin/bin

# Build Hydra
WORKDIR /scratch
RUN yes | pacman -S wget
RUN wget https://www.mpich.org/static/downloads/4.2.2/hydra-4.2.2.tar.gz
RUN tar -xzvf hydra-4.2.2.tar.gz
WORKDIR /scratch/hydra-4.2.2
RUN ./configure
RUN make
RUN make install

# Copy the entire damn worktree.
WORKDIR /scratch/shmembench
COPY . .

# BUILD
RUN CC=oshcc CXX=oshc++ make

WORKDIR /scratch/shmembench/scripts
ENTRYPOINT ["./RUN.sh"]
