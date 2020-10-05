e:\masm32\bin\ml /coff /DBLD_COFF /DIS_32 /W2 /c /Zm /DMASM6 /Zi dgvoodoo.asm
e:\masm32\bin\link dgvoodoo.obj /MAP /DEBUGTYPE:COFF /VXD /DEF:dgvoodoo.def
mapsym -t dgvoodoo.map
