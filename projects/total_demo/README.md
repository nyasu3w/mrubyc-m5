# total_demo
This is a demo of basic features of this project.

`main.cpp` has an implementation to indicate some exception occurred but the `demo.rb` will not make exception.

## point to see
It is almost same as original mruby/c example.
See:
- included files
- definition of mrubycode
  * it is the filename of rb file
- what's to be in setup()
  * calls of these are necessary in this order
    - mrbc_init(mempool, MEMSIZE);
    - my_mrubyc_init();
- 