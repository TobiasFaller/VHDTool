ECHO Installing context menu entries

REM Add extension for VHD files
REG ADD HKCR\SystemFileAssociations\.vhd\shell\vhdtool_mount /t REG_SZ /d "Mount" /f
REG ADD HKCR\SystemFileAssociations\.vhd\shell\vhdtool_mount\command /t REG_SZ /d "\"%~dp0VHDTool.exe\" mount \"%%1\"" /f
REG ADD HKCR\SystemFileAssociations\.vhd\shell\vhdtool_unmount /t REG_SZ /d "Unmount" /f
REG ADD HKCR\SystemFileAssociations\.vhd\shell\vhdtool_unmount\command /t REG_SZ /d "\"%~dp0VHDTool.exe\" unmount \"%%1\"" /f

REM Add extension for VHDX files
REG ADD HKCR\SystemFileAssociations\.vhdx\shell\vhdtool_mount /t REG_SZ /d "Mount" /f
REG ADD HKCR\SystemFileAssociations\.vhdx\shell\vhdtool_mount\command /t REG_SZ /d "\"%~dp0VHDTool.exe\" mount \"%%1\"" /f
REG ADD HKCR\SystemFileAssociations\.vhdx\shell\vhdtool_unmount /t REG_SZ /d "Unmount" /f
REG ADD HKCR\SystemFileAssociations\.vhdx\shell\vhdtool_unmount\command /t REG_SZ /d "\"%~dp0VHDTool.exe\" unmount \"%%1\"" /f
