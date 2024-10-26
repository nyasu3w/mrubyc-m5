#
Import("env")
import subprocess
import os
import glob

def need_new_conversion(file1, file2):
  return not os.path.exists(file2) or os.path.getmtime(file1) > os.path.getmtime(file2)

def before_build(source, target, env):
  print("before_build")
  src_dir =env.GetProjectConfig().get("platformio","src_dir")
  rubyfiles=glob.glob(f"{src_dir}/src/*.rb")
  for rubyfile in rubyfiles:
    rubyfile=rubyfile.replace("\\", "/")
    cfile=os.path.splitext(rubyfile)[0]+".c"
    bname=os.path.splitext(os.path.basename(rubyfile))[0]
    if need_new_conversion(rubyfile, cfile):
      print(f"updating {cfile}")
      ret=subprocess.run(['c:/msys64/usr/bin/bash','-l', '-c', f'mrbc -B {bname} {rubyfile}'])
      if ret.returncode!=0 : exit(ret.returncode)
    else:
      print(f"skipping updating {cfile}")

def add_libdeps():
  print("add_libdeps")
  src_dir =env.GetProjectConfig().get("platformio","src_dir")
  libdeps_file = os.path.join(src_dir,'libdeps.txt')
  if os.path.exists(libdeps_file):
    libs=env.GetProjectOption("lib_deps")
    with open(libdeps_file, 'r', encoding='utf-8') as file:
      lines = file.readlines()
      libs.extend([line.strip() for line in lines])
    print(libs)
    env.PioPlatform().config.set("env","lib_deps",libs)

before_build("","",env)
#env.AddPreAction("$PROGPATH", before_build)
add_libdeps()
