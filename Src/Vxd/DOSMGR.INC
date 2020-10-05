Begin_Service_Table DOSMGR, VxD
DOSMGR_Service   DOSMGR_Get_Version, LOCAL
DOSMGR_Service   _DOSMGR_Set_Exec_VM_Data, VxD_PAGEABLE_CODE
DOSMGR_Service   DOSMGR_Copy_VM_Drive_State, VxD_PAGEABLE_CODE
DOSMGR_Service   _DOSMGR_Exec_VM, VxD_PAGEABLE_CODE
DOSMGR_Service   DOSMGR_Get_IndosPtr, VxD_PAGEABLE_CODE
DOSMGR_Service   DOSMGR_Add_Device, VxD_PAGEABLE_CODE
DOSMGR_Service   DOSMGR_Remove_Device, VxD_PAGEABLE_CODE
DOSMGR_Service   DOSMGR_Instance_Device, VxD_ICODE
DOSMGR_Service   DOSMGR_Get_DOS_Crit_Status, LOCAL
DOSMGR_Service   DOSMGR_Enable_Indos_Polling, VxD_ICODE
DOSMGR_Service   DOSMGR_BackFill_Allowed, LOCAL
DOSMGR_Service   DOSMGR_LocalGlobalReg, LOCAL
ifndef	WIN31COMPAT
DOSMGR_Service   DOSMGR_Init_UMB_Area, VxD_ICODE
DOSMGR_Service   DOSMGR_Begin_V86_App, VxD_PAGEABLE_CODE
DOSMGR_Service	 DOSMGR_End_V86_App, VxD_PAGEABLE_CODE
DOSMGR_Service	 DOSMGR_Alloc_Local_Sys_VM_Mem, VxD_ICODE
DOSMGR_Service	 DOSMGR_Grow_CDSs, LOCAL
DOSMGR_Service	 DOSMGR_Translate_Server_DOS_Call, VxD_PAGEABLE_CODE
DOSMGR_Service	 DOSMGR_MMGR_PSP_Change_Notifier, VxD_PAGEABLE_CODE
endif
End_Service_Table DOSMGR, VxD
