#
# mruby/c  Makefile
#
# Copyright (C) 2015- Kyushu Institute of Technology.
# Copyright (C) 2015- Shimane IT Open-Innovation Center.
# Copyright (C) 2024- HASUMI Hitoshi.
#
#  This file is distributed under BSD 3-Clause License.
#

USER_ID = $(shell id -u)

.PHONY: all mrblib mrubyc_lib mrubyc_bin
all: mrubyc_lib mrubyc_bin

mrblib:
	cd mrblib ; $(MAKE) distclean all

mrubyc_lib:
	cd mrblib ; $(MAKE) all
	cd src ; $(MAKE) -j4 all

mrubyc_bin:
	cd sample_c ; $(MAKE) -j4 all

.PHONY: clean clean_all
clean:
	cd src ; $(MAKE) clean
	cd sample_c ; $(MAKE) clean

# clean including auto generated files.
clean_all: clean
	cd src ; $(MAKE) clean_all

.PHONY: docker_build delete_docker docker_bash

docker_build:
	docker build -t mrubyc-test --build-arg USER_ID=$(USER_ID) $(options) .

docker_bash:
	docker run --rm -it -v $(shell pwd):/work/mrubyc mrubyc-test /bin/bash

docker_bash_root:
	docker run -u root --rm -it -v $(shell pwd):/work/mrubyc mrubyc-test /bin/bash

delete_docker:
	docker rmi mrubyc-test

.PHONY: test test_host_gcc test_host_clang test_mips test_arm test_host_gcc_no_libc test_host_clang_no_libc test_mips_no_libc test_arm_no_libc test_full

test_full: test_host_gcc test_host_gcc_no_libc test_host_clang test_host_clang_no_libc test_arm test_arm_no_libc

test: # if platform includes darwin, test_clang, else, test_host_gcc
	@if [ `uname` = "Darwin" ]; then \
		make test_host_clang_no_libc; \
	else \
		make test_host_gcc_no_libc; \
	fi

test_arm_no_libc:
	docker run -e QEMU_LD_PREFIX=/usr/arm-linux-gnueabihf \
		-e MRUBY_CONFIG=arm-linux-gnueabihf \
		-e PICORUBY_NO_LIBC_ALLOC=1 \
		-e RUBY="qemu-arm build/arm-linux-gnueabihf/bin/picoruby" \
		--rm -v $(shell pwd):/work/mrubyc mrubyc-test \
		bash -c \
		"rake clean && rake && qemu-arm build/arm-linux-gnueabihf/bin/picoruby /work/mrubyc/test/0_runner.rb"

test_arm:
	docker run -e QEMU_LD_PREFIX=/usr/arm-linux-gnueabihf \
		-e MRUBY_CONFIG=arm-linux-gnueabihf \
		-e RUBY="qemu-arm build/arm-linux-gnueabihf/bin/picoruby" \
		--rm -v $(shell pwd):/work/mrubyc mrubyc-test \
		bash -c \
		"rake clean && rake && qemu-arm build/arm-linux-gnueabihf/bin/picoruby /work/mrubyc/test/0_runner.rb"

test_host_gcc_no_libc:
	docker run \
		-e CC=gcc \
		-e MRUBY_CONFIG=default \
		-e PICORUBY_NO_LIBC_ALLOC=1 \
		--rm -v $(shell pwd):/work/mrubyc mrubyc-test \
		bash -c \
		"rake clean && rake && bin/picoruby /work/mrubyc/test/0_runner.rb"

test_host_gcc:
	docker run \
		-e CC=gcc \
		-e MRUBY_CONFIG=default \
		--rm -v $(shell pwd):/work/mrubyc mrubyc-test \
		bash -c \
		"rake clean && rake && bin/picoruby /work/mrubyc/test/0_runner.rb"

test_host_clang_no_libc:
	docker run \
		-e CC=clang \
		-e MRUBY_CONFIG=default \
		-e PICORUBY_NO_LIBC_ALLOC=1 \
		--rm -v $(shell pwd):/work/mrubyc mrubyc-test \
		bash -c \
		"rake clean && rake && bin/picoruby /work/mrubyc/test/0_runner.rb"

test_host_clang:
	docker run \
		-e CC=clang \
		-e MRUBY_CONFIG=default \
		--rm -v $(shell pwd):/work/mrubyc mrubyc-test \
		bash -c \
		"rake clean && rake && bin/picoruby /work/mrubyc/test/0_runner.rb"

test_mips:
	docker run -e QEMU_LD_PREFIX=/usr/mips-linux-gnu \
		-e MRUBY_CONFIG=mips-linux-gnu \
		-e RUBY="qemu-mips -L /usr/mips-linux-gnu build/mips-linux-gnu/bin/picoruby" \
		--rm -v $(shell pwd):/work/mrubyc mrubyc-test \
		bash -c \
		"rake clean && rake && qemu-mips build/mips-linux-gnu/bin/picoruby /work/mrubyc/test/0_runner.rb"

test_mips_no_libc:
	docker run -e QEMU_LD_PREFIX=/usr/mips-linux-gnu \
		-e MRUBY_CONFIG=mips-linux-gnu \
		-e PICORUBY_NO_LIBC_ALLOC=1 \
		-e RUBY="qemu-mips -L /usr/mips-linux-gnu build/mips-linux-gnu/bin/picoruby" \
		--rm -v $(shell pwd):/work/mrubyc mrubyc-test \
		bash -c \
		"rake clean && rake && qemu-mips build/mips-linux-gnu/bin/picoruby /work/mrubyc/test/0_runner.rb"
