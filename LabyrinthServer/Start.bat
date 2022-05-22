@ECHO OFF
TITLE LabyrinthServer
SET server_name="Server.exe"
ECHO Make sure the server is called : Server.exe
SET /P map_size="Map size: "
SET /P difficulty="Map difficulty: "
SET /P min_clients="Minimun number of clients: "
SET /P max_clients="Maximum number of clients: "
SET /P player_wait="Time to wait for players to join: "
SET /P port="Port: "
SET "ip="
SET /P ip="IP (optional): "
:START
CLS
IF EXIST %server_name% (
IF DEFINED ip %server_name% %map_size% %map_size% %difficulty% %min_clients% %max_clients% %player_wait% %port% %ip%
IF NOT DEFINED ip %server_name% %map_size% %map_size% %difficulty% %min_clients% %max_clients% %player_wait% %port%
)
IF NOT EXIST %server_name% (
SET /P server_name="Enter server name(with .exe): "
GOTO START
)
PAUSE
EXIT / B 0
