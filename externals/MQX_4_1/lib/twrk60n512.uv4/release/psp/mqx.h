/* MQX library output paths and names were changed in MQX 4.0.0 */ 

#error Your application project uses incorrect paths and libraries.

/* 
* Update library search paths to match the MQX 4.0.0 layout and refer to new library names
*        
* Modify your project setting:
*     In CW10     - Project Properties/Settings/Linker + Compiler + Assembler/Input
*     In IAR      - Project Options/C\C++ Compiler/Preprocessor
*     In Keil uV4 - Project Options/C\C++, Project Options/ASM    
*
*     Add the following paths to your project
*        lib\<board>.<compiler>\<target>\bsp
*        lib\<board>.<compiler>\<target>\psp
*        lib\<board>.<compiler>\<target>\bsp\dbg
*        lib\<board>.<compiler>\<target>\usb         
*     Modify the following paths (include <target> name in the path; add "debug" or "release")
*        Change lib\<board>.<compiler>\<libname>  -> lib\<board>.<compiler>\<target\><libname>
*        e.g. lib\twrk60n512.cw10\rtcs            -> lib\twrk60n512.cw10\debug\rtcs          
*     Remove all obsolete paths 
*        lib\<board>.<compiler>\mqx
*        lib\<board>.<compiler>\mqx\dbg
* 
* Change linker file search patch
*     In IAR      - Project Options/Linker
*     In Keil UV4 - Project Options/Linker     
*
*     Change lib/<board>.<compiler>/mqx/intflash.icf  ->  lib/<board>.<compiler>/<target>/bsp/intflash.icf     
*              
* Change the library names to use new format and add the new library files to the project
*     Change <lib_name>_<board>_d.a  -> <lib_name>.a
*     e.g. bsp_twrk60n512_d.a        ->  bsp.a
*      
* Change the debugger config files paths (CW10 only - Debug Configurations..., )
*     lib\<board>.<compiler>\<target>\bsp\dbg\<board>.cfg -> lib\<board>.<compiler>\dbg\<board>.tcl (or <board>.cfg in case of ColdFire)
*     lib\<board>.<compiler>\<target>\bsp\dbg\<board>.mem -> lib\<board>.<compiler>\dbg\<board>.mem                      
*/
