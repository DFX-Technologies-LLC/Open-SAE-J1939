project(
  'Open-SAE-J1939',
  'c',
  version: '0.1',
  default_options: ['warning_level=3'],
)

open_sae_j1939_include_directories = []
open_sae_j1939_include_directories += 'Src/Hardware'
open_sae_j1939_include_directories += 'Src/ISO_11783'
open_sae_j1939_include_directories += 'Src/Open_SAE_J1939'
open_sae_j1939_include_directories += 'Src/SAE_J1939'

open_sae_j1939_sources = files(
  'Src/Hardware/CAN_Transmit_Receive.c',
  'Src/Hardware/FLASH_EEPROM_RAM_Memory.c',
  'Src/Hardware/Save_Load_Struct.c',
  'Src/ISO_11783/ISO_11783-7_Application_Layer/Auxiliary_Valve_Command.c',
  'Src/ISO_11783/ISO_11783-7_Application_Layer/Auxiliary_Valve_Estimated_Flow.c',
  'Src/ISO_11783/ISO_11783-7_Application_Layer/Auxiliary_Valve_Measured_Position.c',
  'Src/ISO_11783/ISO_11783-7_Application_Layer/General_Purpose_Valve_Command.c',
  'Src/ISO_11783/ISO_11783-7_Application_Layer/General_Purpose_Valve_Estimated_Flow.c',
  'Src/Open_SAE_J1939/Closedown_ECU.c',
  'Src/Open_SAE_J1939/Listen_For_Messages.c',
  'Src/Open_SAE_J1939/Startup_ECU.c',
  'Src/SAE_J1939/SAE_J1939-21_Transport_Layer/Acknowledgement.c',
  'Src/SAE_J1939/SAE_J1939-21_Transport_Layer/Request.c',
  'Src/SAE_J1939/SAE_J1939-21_Transport_Layer/Transport_Protocol_Connection_Management.c',
  'Src/SAE_J1939/SAE_J1939-21_Transport_Layer/Transport_Protocol_Data_Transfer.c',
  'Src/SAE_J1939/SAE_J1939-71_Application_Layer/Request_Component_Identification.c',
  'Src/SAE_J1939/SAE_J1939-71_Application_Layer/Request_ECU_Identification.c',
  'Src/SAE_J1939/SAE_J1939-71_Application_Layer/Request_Proprietary.c',
  'Src/SAE_J1939/SAE_J1939-71_Application_Layer/Request_Software_Identification.c',
  'Src/SAE_J1939/SAE_J1939-73_Diagnostics_Layer/DM1.c',
  'Src/SAE_J1939/SAE_J1939-73_Diagnostics_Layer/DM14.c',
  'Src/SAE_J1939/SAE_J1939-73_Diagnostics_Layer/DM15.c',
  'Src/SAE_J1939/SAE_J1939-73_Diagnostics_Layer/DM16.c',
  'Src/SAE_J1939/SAE_J1939-73_Diagnostics_Layer/DM2.c',
  'Src/SAE_J1939/SAE_J1939-73_Diagnostics_Layer/DM3.c',
  'Src/SAE_J1939/SAE_J1939-81_Network_Management_Layer/Address_Claimed.c',
  'Src/SAE_J1939/SAE_J1939-81_Network_Management_Layer/Address_Delete.c',
  'Src/SAE_J1939/SAE_J1939-81_Network_Management_Layer/Address_Not_Claimed.c',
  'Src/SAE_J1939/SAE_J1939-81_Network_Management_Layer/Commanded_Address.c',
)

open_sae_j1939_c_args = ['-Wno-pedantic', '-Wno-unused-parameter']

open_sae_j1939_dep = declare_dependency(
  sources: [open_sae_j1939_sources],
  compile_args: open_sae_j1939_c_args,
  include_directories: open_sae_j1939_include_directories,
)