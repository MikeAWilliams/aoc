I decided to hit part two of this problem with a big multi threaded stick. I am using for_each with std::execution::par_unseq. GCC will ignore this and use one thread unless you install and link the tbb library. The linking is in the cmake but you need to run the following to install
```
sudo apt-get -y install libtbb-dev
```