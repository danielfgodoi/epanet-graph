all:
	g++ src/Main.cpp lib/AdjacentNode.cpp lib/Node.cpp lib/Link.cpp lib/Graph.cpp epanet2.dll -o "EN2 Graph Generator.exe" -Wl,--enable-stdcall-fixup -static-libgcc -static-libstdc++