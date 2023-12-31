/*
** LinkerScript
*/

/******************************************************************************/
/*                               M4 ROM section                               */
/******************************************************************************/

/* M4 Vector table */
M4_INTVECT_start               = 0x08000000;
M4_SB_region_ROM_start         = M4_INTVECT_start;
M4_Vector_size                 = 0x200;

/* M4 SB Code region */
M4_SB_Code_region_ROM_start    = M4_INTVECT_start + M4_Vector_size;
M4_SB_Code_region_ROM_end      = 0x080057FF;
M4_SB_region_ROM_end           = M4_SB_Code_region_ROM_end;

/* ROM regions size definition */
M4_SB_Code_region_ROM_size     = M4_SB_Code_region_ROM_end - M4_SB_Code_region_ROM_start + 0x1;

/******************************************************************************/
/*                               M0+ ROM section                              */
/******************************************************************************/

/* KMS Data Storage (NVMS) region protected area */
/* KMS Data Storage need for 2 images : 4 kbytes * 2 ==> 8 kbytes */
KMS_DataStorage_start          = 0x08029800;
KMS_DataStorage_end            = KMS_DataStorage_start + 0x1FFF;

/* SE IF ROM: used to locate Secure Engine interface Code out of MPU isolation     */
SE_IF_region_ROM_start         = KMS_DataStorage_end + 0x1; 
SE_IF_region_ROM_end           = SE_IF_region_ROM_start + 0x17FF;

/* SBSFU Code region */
SB_region_ROM_start            = SE_IF_region_ROM_end + 0x1;
SB_region_ROM_end              = 0x08035FFF;

/* M0 Vector table with alignment constraint on Vector_size */
INTVECT_start                  = 0x08036000;
Vector_size                    = 0x200;
C2_BOOT_BASE_ADDR              = INTVECT_start;

/* SE Code region protected by MPU isolation */
SE_Code_region_ROM_start       = INTVECT_start + Vector_size;
SE_CallGate_region_ROM_start   = SE_Code_region_ROM_start; /* No need to do +4 as we have dummy bytes in SE_CoreBin .ld file */
SE_CallGate_region_ROM_end     = SE_Code_region_ROM_start + 0xFF;

/* SE Startup */
SE_Startup_region_ROM_start    = SE_CallGate_region_ROM_end + 0x1;
SE_Code_NoKey_region_ROM_start = SE_Startup_region_ROM_start + 0x100;

/* SE Embedded Keys */
SE_Key_region_ROM_start        = 0x0803E800;
SE_Key_region_ROM_end          = 0x0803EFFF;
SE_Code_region_ROM_end         = SE_Key_region_ROM_end;

/* ROM regions size definition */
KMS_DataStorage_size           = KMS_DataStorage_end - KMS_DataStorage_start + 0x1;
SE_IF_region_ROM_size          = SE_IF_region_ROM_end - SE_IF_region_ROM_start + 0x1;
SB_region_ROM_size             = SB_region_ROM_end - SB_region_ROM_start + 0x1;
SE_Code_region_ROM_size        = SE_Code_region_ROM_end - SE_Code_region_ROM_start + 0x1;
SE_CallGate_region_ROM_size    = SE_CallGate_region_ROM_end - SE_CallGate_region_ROM_start + 0x1;
SE_Startup_region_ROM_size     = SE_Code_NoKey_region_ROM_start - SE_Startup_region_ROM_start;
SE_Code_NoKey_region_ROM_size  = SE_Key_region_ROM_start - SE_Code_NoKey_region_ROM_start;
SE_Key_region_ROM_size         = SE_Key_region_ROM_end - SE_Key_region_ROM_start + 0x1;

/******************************************************************************/
/*                              RAM section                                   */
/*                                                                            */
/******************************************************************************/

/* M4 SB */
M4_SB_region_RAM_start         = 0x20000000;
M4_SB_region_RAM_end           = M4_SB_region_RAM_start + 0xCDF;

/* M0+/M4 Synchronization flag */
M4_M0PLUS_flag_RAM_start       = M4_SB_region_RAM_end + 0x1;
M4_M0PLUS_flag_RAM_end         = M4_M0PLUS_flag_RAM_start + 0x1F;

/* M4 UserApp : allocation limited to half RAM size */
M4_APP_region_RAM_start        = M4_M0PLUS_flag_RAM_end + 0x1;
M4_APP_region_RAM_end          = 0x20007FFF;

/* SBSFU RAM region */
SB_region_RAM_start            = M4_APP_region_RAM_end + 0x1;
SB_region_RAM_end              = 0x2000C7FF;

/* SE RAM region protected area with 1 kBytes alignment constraint (TZIC) ==> 0x2000C800 */
SE_region_RAM_start            = SB_region_RAM_end + 0x1;
SE_region_RAM_stack_top        = SE_region_RAM_start + 0xC00;                /* Secure Engine's private stack */
SE_region_RAM_end              = 0x2000FFEF;

/* KMS data storage encryption/decryption key */
KMS_DataStorageKey_start       = 0x2000FFF0;
KMS_DataStorageKey_end         = 0x2000FFFF;

/* RAM regions size definition */
M4_SB_region_RAM_size          = M4_SB_region_RAM_end - M4_SB_region_RAM_start + 0x1;
M4_APP_region_RAM_size         = M4_APP_region_RAM_end - M4_APP_region_RAM_start + 0x1;
SB_region_RAM_size             = SB_region_RAM_end - SB_region_RAM_start + 0x1;
SE_region_RAM_size             = SE_region_RAM_end - SE_region_RAM_stack_top + 0x1;

/******************************************************************************/
/*                              RAM/ROM regions                               */
/*                                                                            */
/******************************************************************************/

MEMORY
{
 M4_INTVECT_ROM_region (rx)       : ORIGIN = M4_INTVECT_start, LENGTH = M4_Vector_size
 M4_SB_Code_ROM_region (rx)       : ORIGIN = M4_SB_Code_region_ROM_start, LENGTH = M4_SB_Code_region_ROM_size
 SE_IF_ROM_region (rx)            : ORIGIN = SE_IF_region_ROM_start, LENGTH = SE_IF_region_ROM_size
 SB_ROM_region (rx)               : ORIGIN = SB_region_ROM_start, LENGTH = SB_region_ROM_size
 INTVECT_ROM_region (rx)          : ORIGIN = INTVECT_start, LENGTH = Vector_size
 SE_ROM_region (rx)               : ORIGIN = SE_CallGate_region_ROM_start, LENGTH = SE_Code_region_ROM_size
 SE_CallGate_ROM_region (rx)      : ORIGIN = SE_CallGate_region_ROM_start, LENGTH = SE_CallGate_region_ROM_size
 SE_Startup_ROM_region (rx)       : ORIGIN = SE_Startup_region_ROM_start, LENGTH = SE_Startup_region_ROM_size
 SE_Code_NoKey_ROM_region (rx)    : ORIGIN = SE_Code_NoKey_region_ROM_start, LENGTH = SE_Code_NoKey_region_ROM_size
 SE_Key_region_ROM (rx)           : ORIGIN = SE_Key_region_ROM_start, LENGTH = SE_Key_region_ROM_size
 M4_SB_RAM_region (xrw)           : ORIGIN = M4_SB_region_RAM_start, LENGTH = M4_SB_region_RAM_size
 SB_RAM_region (xrw)              : ORIGIN = SB_region_RAM_start, LENGTH = SB_region_RAM_size
 SE_RAM_region (xrw)              : ORIGIN = SE_region_RAM_stack_top, LENGTH = SE_region_RAM_size
}
