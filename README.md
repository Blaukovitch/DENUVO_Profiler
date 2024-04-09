# DENUVO_Profiler
This is part of the original source code of **DENUVO_Profiler**, written by 386 Team in 2014-2017 to investigate the first versions of DENUVO (eq VMProtect 2-3), when there were no lifters and other modern devirtualisation tools.  
**DENUVO_Profiler** is a subsequent development of the **SecuROM_Profiler** utility - the simplest tool for visualising the operation of a virtual machine and effectively controlling the data it uses in the byte-code tape (p-code).  
**DENUVO_Profiler** also hunts for Electronic Arts (Origin) licence containers on the current user's machine and can extract their contents, with including the GameToken.  
![TANK80](/~!IMG/t80.PNG)

## Opportunities "inspire"
* Detection number of VMProtect contexts;  
* Detection type of VM "islands" (HANDLES);  
* Hook of VM HANDLES (Profiling);  
* Show you DENUVO HWID (actual for eraly versions, modern version have a two new components);  
* Decode Electronic Arts origin license (XML);  
** Decode DENUVO GameToken;  
![DGTOKEN](/~!IMG/Origin.png)

## DENUVO GameToken
GameToken is Secret X-Table, which contains mathematical dependencies between you current hardware and encrypted HWID data in this table.  
![DGTOKEN](/~!IMG/DENUVO_GAMETOKEN.png)
![DHWID](/~!IMG/DENUVO_HWID.png)

## Folders description
*PRIMARY:*
* `\ENGINE\` - "DENUVO_Inspire" main engine;  
* `\ThirdParty\Origin\` - Electronic Arts Origin (Origin CORE dissaembly info);  

*Secondary:*
* `\BEA\` - BEA engine source;  
* `\BeaEngine\` - BEA engine dlls & lib;  
* `\XED\` - XED dissasembly;  
* `\CRYPTO\` - simple implementation of AES/MD5;  


## Versioning
* DENUVO v1-2  
* VMProtect 2.6.x and hingher  
* VMProtect 3.0.x (early)  

## SONY DADC AG SecuROM_Profiler
![SPR](/~!IMG/SecuROM_Profiler.jpg)

## Tiberium reversing
All materials published as part of **Tiberium reversing** - full stack of articles (xakep.ru), cracking videos (YOUTUBE) and C/C++/ASM coding tools. 
(C) *2011-2024*

