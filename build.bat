@echo off

set CommonCompilerFlags=-MTd -nologo -fp:fast -Gm- -GR- -EHa- -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -DDEMO=1 -DGHOST_INTERNAL=1 -DGHOST_SLOW=1 -DGHOST_WIN32=1 -FC -Z7
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib

REM TODO - can we just build both with one ex?

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build

REM 32-bit build
REM cl %CommonCompilerFlags% ..\ghost\code\win32_ghost.cpp /link -subsystem:windows,5.1 %CommonLinkerFlags%

REM 64-bit build
del *.pdb > NUL 2> NUL
REM Optimization switches /O2
echo WAITING FOR PDB > lock.tmp
cl %CommonCompilerFlags% ..\ghost\code\ghost.cpp -Fmghost.map -LD /link -incremental:no -opt:ref -PDB:ghost_%random%.pdb -EXPORT:GameGetSoundSamples -EXPORT:GameUpdateAndRender
del lock.tmp
cl %CommonCompilerFlags% ..\ghost\code\win32_ghost.cpp -Fmwin32_ghost.map /link %CommonLinkerFlags%
popd
