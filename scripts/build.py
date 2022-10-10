# TODO(caio): Distinguish between debug and release builds
import subprocess as sp
from pathlib import Path
import sys

is_release = sys.argv[1] == '-release'

## ========================================================
## COMPILER

# Output directory
output_dir = '../build/x64_debug'
if is_release:
    output_dir = '../build/x64_release'
output_name = 'typheus'     # Will be composed with relevant extensions (exe, pdb, etc.)

# Include paths
dirs_include = []
dirs_include += ['../include']
dirs_include += ['../dependencies/glad/include']
dirs_include += ['../dependencies/sdl2/include']
dirs_include += ['../dependencies/glm']
dirs_include += ['../dependencies/tinyobjloader']
dirs_include += ['../dependencies/stb_image']
dirs_include += ['../dependencies/imgui']

# Preprocessor defines
preprocessor_defines = []
preprocessor_defines += ['_CRT_SECURE_NO_WARNINGS']
if is_release:
    preprocessor_defines += ['_RELEASE']
else:
    preprocessor_defines += ['_DEBUG']

# Precompiled header
pch_name = 'stdafx'

# Source file paths
paths_src = []
for path in Path('../src').rglob('*.cpp'):
    paths_src.append(str(path.resolve().as_posix()))
for path in Path('../dependencies/glad/src').rglob('*.c'):
    paths_src.append(str(path.resolve().as_posix()))
for path in Path('../dependencies/imgui').glob('*.cpp'):
    paths_src.append(str(path.resolve().as_posix()))
for path in Path('../dependencies/imgui/backends').glob('*.cpp'):
    path_src = str(path.resolve().as_posix())
    if 'opengl3.cpp' in path_src or 'sdl.cpp' in path_src:
        paths_src.append(path_src)

# Additional options
additional_options = []
additional_options += ['/w']                        # Disable warnings beacuse why not
additional_options += ['/permissive-']
additional_options += ['/sdl']
additional_options += ['/FC']                       # Full paths in cl diagnostic text
additional_options += ['/EHsc']                     # Disable exception handling!
additional_options += ['/diagnostics:caret']        # Show issues with caret below indicated text
additional_options += ['/MP5']                      # Multiprocess
if is_release:
    additional_options += ['/O2 /Ot /Oi']           # Optimization level
    additional_options += ['/GL']                   # Whole program optimization
    additional_options += ['/MD']                   # C runtime libs
    additional_options += ['/fp:fast']              # Fast math
else:
    additional_options += ['/Od']                   # Optimization level
    additional_options += ['/JMC']                  # (DEBUG) Debugger only steps through user code
    additional_options += ['/Zi']                   # (DEBUG) Generates debug info
    additional_options += ['/MDd']                  # C debug runtime libs
    additional_options += ['/fp:precise']           # No fast math
    additional_options += ['/RTC1']                     # Runtime checks

# Building compile command
command = 'cl '
command += ' '.join(additional_options) + ' '
command += ' '.join([f'/I \"{i}\"' for i in dirs_include]) + ' '
command += ' '.join([f'/D \"{i}\"' for i in preprocessor_defines]) + ' '
command += f'/Fd\"{output_dir}/vc142.pdb\" '    # TODO(caio): Does this need this name?
command += ' '.join([f'\"{i}\"' for i in paths_src]) + ' '
command += f'/Fo\"{output_dir}/\" '

## ========================================================
## LINKER

linker_options = []
linker_options += ['/MACHINE:X64']              # x64 platform specification
linker_options += ['/SUBSYSTEM:WINDOWS']        # Windows app
if is_release:
    linker_options += ['/INCREMENTAL:NO']       # Disable incremental linking
    linker_options += ['/OPT:REF']              # Eliminates unreferenced functions
else:
    linker_options += ['/INCREMENTAL']          # (DEBUG) Incremental linking
    linker_options += ['/DEBUG']                # (DEBUG) Creates debug info

dirs_libraries = []
dirs_libraries += ['../dependencies/sdl2/lib/x64']   # Remove this when SDL2 dependency is no more

linker_libraries = []
linker_libraries += ['opengl32.lib']
linker_libraries += ['SDL2.lib']
linker_libraries += ['SDL2main.lib']
linker_libraries += ['kernel32.lib']
linker_libraries += ['user32.lib']
linker_libraries += ['gdi32.lib']
linker_libraries += ['winspool.lib']
linker_libraries += ['comdlg32.lib']
linker_libraries += ['advapi32.lib']
linker_libraries += ['shell32.lib']
linker_libraries += ['ole32.lib']
linker_libraries += ['oleaut32.lib']
linker_libraries += ['uuid.lib']
linker_libraries += ['odbc32.lib']
linker_libraries += ['odbccp32.lib']

# Building link command
command += f'/link /OUT:\"{output_dir}/{output_name}.exe\" '
command += ' '.join(linker_options) + ' '
command += ' '.join([f'/LIBPATH:\"{i}\"' for i in dirs_libraries]) + ' '
command += ' '.join([f'\"{i}\"' for i in linker_libraries]) + ' '
if not is_release:
    command += f'/PDB:\"{output_dir}/{output_name}.pdb\" /ILK:\"{output_dir}/{output_name}.ilk\"'

print(command)