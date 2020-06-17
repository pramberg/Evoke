call dxc -spirv -T vs_5_0 -E main ../Shaders/vert.hlsl -Fo ../Shaders/vert.spirv
call dxc -spirv -T ps_5_0 -E main ../Shaders/frag.hlsl -Fo ../Shaders/frag.spirv