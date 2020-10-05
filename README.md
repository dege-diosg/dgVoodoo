# dgVoodoo 1.x series with source code



## -1. Important (this section was added in 2020)

dgVoodoo 1.x series has nothing to do with dgVoodoo 2.x series that is still in development.
If I had knew that dgVoodoo 2.x would become popular and exceed itself then I'm sure I wouldn't have branded it under the name of dgVoodoo.

dgVoodoo 1.x implements only the following modules:
<ul>
<li><b>Glide.dll</b> (Windows)</li>
<li><b>Glide2x.dll</b> (Windows)</li>
<li><b>Glide2x.ovl</b> (DOS)</li>
<li><b>dgVesa</b> (DOS) (VESA interface implementation)</li>
<li><b>dgVoodoo.exe</b> (Windows) (The server app for DOS)</li>
<li><b>dgVoodoo.vxd</b> (Windows) (The Win9x kernel component for the server app)</li>
<li><b>dgVoodooSetup</b> (Windows) (The CPL app)</li>
</ul>

dgVoodoo 1.x is no longer supported. That's why I released its source years ago.<br>
dgVoodoo 2.x is still in development and doesn't shares any code from 1.x.



## 0. License

I release this source under GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.
To complete the reqirements, I inserted a short description along with the license text into each
source file.



## 1. Some thoughts

Well, when Stiletto decided to collect all available graphics wrappers, with all of their related stuffs, into a
big package, I thought of it as making a wrapper museum. A museum? Yes, because most of the wrappers are not supposed
to work on recent modern systems, including dgVoodoo 1.x. He asked if I had any dgVoodoo source stuff to include, and
I thought, after all, why not to release dgVoodoo source. It all is old, aged, outdated and abandoned. No one will
utilize anything from it because nowadays no one is programming Win9x kernel, 32 bit protected mode DOS, etcetera.
No one is supposed to utilize anything from the Glide graphics wrapping part because it is relying on legacy technology,
fixed function pipeline, very simple shaders, and so on. There are better implementations for some time past.

If this stuff is useful for somebody after all, then I'm glad to be helpful. :)



## 2. About the source


This version was ported from MSVC 6 to MSVC 2010 so that most assembly files is been ported to MASM as well!
Since this version is not exactly the same as 1.50 Beta2, I renamed it to 1.50 Beta3. I did some developments
after releasing 1.50 Beta2 that were never released. It affects mostly dgVoodooSetup.


At first, the most important: the source is a total mess. Or, almost...
I was a big assembly-nerd, I wrote all of my codes in assembly in the DOS era. I couldn't even code in C.
I used some Pascal in the school but always laughed at the generated code, it's size and performance...
Size and speed was over all for me, back then (plz don't misunderstand).
That is why there are a lot of assembly code and some of them are really ancient, ported from DOS.

This version of dgVoodoo was ported from C. Keep this in mind when seeing strange this like using int type
instead of bool, etc.


And, sorry about the comments. As far as I remember, later I partly englishized them but not entirely (update: I was wrong...).


Used tools:

- <b>MASM (VS6 or VS2003)</b>         (needed for the Win9x virtual device driver, dgVoodoo.vxd)

- <b>Visual Studio 2010</b>           (for glide.dll, glide2x.dll, dgVoodoo.exe, dgVoodooSetup.exe)
- <b>TASM</b>                         (it was my favourite assembler, RIP :) needed for real mode DOS code like dgVesa.com)

- <b>Watcom C++</b>
- <b>Watcom Assembler</b>             (glide2x.ovl)

Important: when looking at the code, its awful style and some aged programming techniques, keep in mind
that it is not me. I mean, it is an earlier version of me, Dege 1.x. I was kid when wrote it. :)
If I had to rewrite dgVoodoo I would do it in a different way (as I did with 2.x) even if I had to use the
same DX7/DX9 technological level.


Are you sure you want to see it? :)


(If you find the quantity of the assembly pretty large, keep in mind that this version run fast even on a
Pentium2 with a RivaTnT2 graphic card. Since bitmap conversion, colorkeying and so on could nowhere and near
be solved on the GPU back then, it had to be done on the cpu with fast code that could not be written in C.
I still maintain this claim, I still use assembly for fast, time critical codes using SIMD, vectorized SSE
instructions with instruction pairing. The only difference now is that I use assembly for things that really
need that, all other is done in C++.)



## 3. How to compile

dgVesa:                Can only be compiled with TASM with the included batch file, manually.
                       (Since real mode DOS version of TASM is required here, you must use 32 bit Windows or DosBox!)
dgVoodoo.vxd           Can only be compiled with old version MASM (one from VS6 or VS2003) with the included batch file, manually.
dgVoodoo.ovl           Can only be compiled with Watcom C++ compiler, with the included batch files, manually.
dgVoodoo.exe           Use the attached MSVC solution and compile it with configurations 'Debug' or 'Release'.
dgVoodooSetup.exe      Use the attached MSVC solution and compile it with configurations 'Debug' or 'Release'.
Glide.dll              Use the attached MSVC solution and compile it with configurations 'Glide 211 Debug' or 'Glide 211 Release'.
Glide2x.dll            Use the attached MSVC solution and compile it with configurations 'Glide 243 Debug' or 'Glide 243 Release'.


Dege
