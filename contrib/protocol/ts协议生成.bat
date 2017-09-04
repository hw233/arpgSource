@set root_path=%~dp0

@set tool_exe=%root_path%\CodeBuilder\GZCodeBuilder.exe
@set protocol_dir=%root_path%
@set server_path=%root_path%\..\..\
@set server_net=%server_path%\src\svrcore\protocol
@set robot_net=%server_path%\src\robotd\handler
@set lua_h_dir=%server_path%\contrib\scripts\share
@set as_h_dir=%server_path%\contrib\as
@set ts_h_dir=%server_path%\contrib\ts
@set ts_h_out_dir=E:/web/socket/SocketWeb/net/script


@echo 生成ts协议
%tool_exe% -f %protocol_dir%\Cow.Msg -t %ts_h_dir% -o %ts_h_out_dir%

