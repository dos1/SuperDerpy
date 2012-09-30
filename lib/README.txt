This directory is used for distributing library binaries together with
Super Derpy packages.

It's optional - Super Derpy will build correctly without any file placed there,
but they're important when trying to create redistributable packages supposed
to run on systems without Allegro installed.

To include those files in Super Derpy build, run cmake with -DPACKAGE_BUILD=1

-------------------------------------------------------------------------------
When building for Windows, place all run-time DLLs in "win32" directory.

This is the list of DLLs included with official release:
 * allegro-5.1.dll
 * allegro_audio-5.1.dll
 * allegro_image-5.1.dll
 * allegro_ttf-5.1.dll
 * allegro_acodec-5.1.dll
 * allegro_font-5.1.dll
 * allegro_primitives-5.1.dll
 * libgcc_s_dw2-1.dll
 * libstdc++-6.dll
 * freetype6.dll
 * libogg-0.dll
 * zlib1.dll

As there's no standard way of installing packages in Windows, including those
files may be a good idea even when building just for yourself, unless you
made all those libraries registered in your system.

-------------------------------------------------------------------------------
When building for GNU/Linux, place all shared libraries in "linux-x86"
directory.

This is the list of shared libraries included with official release:
 * liballegro.so.5.1
 * liballegro_acodec.so.5.1
 * liballegro_audio.so.5.1
 * liballegro_font.so.5.1
 * liballegro_image.so.5.1
 * liballegro_primitives.so.5.1
 * liballegro_ttf.so.5.1

Please note that while Windows version contains also DLLs with dependences from
Allegro, on GNU/Linux there's no need to provide them, as FreeType, libogg or
zlib are commonly available in distribition repositiories. Allegro 5 is not,
that's why it's included here until proper packages make it into the most
common distributions.

If you're on GNU/Linux, and you're not packaging but only building for
yourself, there's no need to place any files here.

Remember to adjust install prefix when packaging.
-------------------------------------------------------------------------------
TODO: Support Mac OS X (if it's needed at all) and other architectures
than x86.
