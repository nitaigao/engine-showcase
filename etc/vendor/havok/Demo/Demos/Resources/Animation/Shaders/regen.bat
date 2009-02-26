@echo off
fxc /E PShade /T ps_1_1 /Fo Skinning_ps20_dx9.pso SkinningShader.fx
fxc /E VShade /T vs_2_0 /Fo Skinning_vs20_dx9.vso SkinningShader.fx
fxc /E PShade /T ps_1_1 /Fh Skinning_ps20_dx9.psh SkinningShader.fx
fxc /E VShade /T vs_2_0 /Fh Skinning_vs20_dx9.vsh SkinningShader.fx

