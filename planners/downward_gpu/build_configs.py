release = ["-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_PREFIX_PATH=/usr/local/lib/python3.10/dist-packages/torch/share/cmake", 
"-DCMAKE_CUDA_COMPILER=/usr/local/cuda-11.4/bin/nvcc"
]
debug = ["-DCMAKE_BUILD_TYPE=Debug"]
release_no_lp = ["-DCMAKE_BUILD_TYPE=Release", "-DUSE_LP=NO"]
# USE_GLIBCXX_DEBUG is not compatible with USE_LP (see issue983).
glibcxx_debug = ["-DCMAKE_BUILD_TYPE=Debug", "-DUSE_LP=NO", "-DUSE_GLIBCXX_DEBUG=YES"]
minimal = ["-DCMAKE_BUILD_TYPE=Release", "-DDISABLE_PLUGINS_BY_DEFAULT=YES"]

DEFAULT = "release"
DEBUG = "debug"
