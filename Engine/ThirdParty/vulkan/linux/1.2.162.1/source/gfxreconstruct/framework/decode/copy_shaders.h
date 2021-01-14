/*
** Copyright (c) 2019 LunarG, Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
*/

#ifndef GFXRECON_DECODE_VULKAN_COPY_SHADERS_H
#define GFXRECON_DECODE_VULKAN_COPY_SHADERS_H

// Original HLSL shader source.
#if 0
// Shaders for rendering to a full screen trianlge. For a good explanation, see:
// https://www.saschawillems.de/blog/2016/08/13/vulkan-tutorial-on-rendering-a-fullscreen-quad-without-buffers/

struct VSOut
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

VSOut VSMain(uint id : SV_VertexID)
{
    VSOut vsout;
    vsout.texcoord = float2((id << 1) & 2, id & 2);
    vsout.position = float4(vsout.texcoord * 2.0f + -1.0f, 0.0f, 1.0f);
    return vsout;
}

Texture2D copy_texture;
SamplerState copy_sampler;

float4 PSMainColor(VSOut vsout) : SV_Target0
{
    return copy_texture.SampleLevel(copy_sampler, vsout.texcoord, 0);
}

float PSMainDepth(VSOut vsout) : SV_Depth
{
    return copy_texture.SampleLevel(copy_sampler, vsout.texcoord, 0).x;
}

float PSMainStencil(VSOut vsout) : SV_StencilRef
{
    return copy_texture.SampleLevel(copy_sampler, vsout.texcoord, 0).x;
}
#endif

// Build commands.
#if 0
; Command: dxc -spirv -T vs_6_0 -E VSMain -Fh copy_shaders.h copy_shaders.hlsl
; Command: dxc -spirv -T ps_6_0 -E PSMainColor -Fh copy_shaders.h copy_shaders.hlsl
; Command: dxc -spirv -T ps_6_0 -E PSMainDepth -Fh copy_shaders.h copy_shaders.hlsl
; Command: dxc -spirv -T ps_6_0 -E PSMainStencil -Fh copy_shaders.h copy_shaders.hlsl
#endif

// Shader code.
#if 0
; SPIR-V
; Version: 1.0
; Generator: Google spiregg; 0
; Bound: 34
; Schema: 0
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint Vertex %VSMain "VSMain" %gl_VertexIndex %gl_Position %out_var_TEXCOORD0
               OpSource HLSL 600
               OpName %out_var_TEXCOORD0 "out.var.TEXCOORD0"
               OpName %VSMain "VSMain"
               OpDecorate %gl_VertexIndex BuiltIn VertexIndex
               OpDecorate %gl_Position BuiltIn Position
               OpDecorate %out_var_TEXCOORD0 Location 0
       %uint = OpTypeInt 32 0
     %uint_1 = OpConstant %uint 1
     %uint_2 = OpConstant %uint 2
      %float = OpTypeFloat 32
    %float_2 = OpConstant %float 2
   %float_n1 = OpConstant %float -1
    %v2float = OpTypeVector %float 2
         %12 = OpConstantComposite %v2float %float_n1 %float_n1
    %float_0 = OpConstant %float 0
    %float_1 = OpConstant %float 1
%_ptr_Input_uint = OpTypePointer Input %uint
    %v4float = OpTypeVector %float 4
%_ptr_Output_v4float = OpTypePointer Output %v4float
%_ptr_Output_v2float = OpTypePointer Output %v2float
       %void = OpTypeVoid
         %20 = OpTypeFunction %void
%gl_VertexIndex = OpVariable %_ptr_Input_uint Input
%gl_Position = OpVariable %_ptr_Output_v4float Output
%out_var_TEXCOORD0 = OpVariable %_ptr_Output_v2float Output
     %VSMain = OpFunction %void None %20
         %21 = OpLabel
         %22 = OpLoad %uint %gl_VertexIndex
         %23 = OpShiftLeftLogical %uint %22 %uint_1
         %24 = OpBitwiseAnd %uint %23 %uint_2
         %25 = OpConvertUToF %float %24
         %26 = OpBitwiseAnd %uint %22 %uint_2
         %27 = OpConvertUToF %float %26
         %28 = OpCompositeConstruct %v2float %25 %27
         %29 = OpVectorTimesScalar %v2float %28 %float_2
         %30 = OpFAdd %v2float %29 %12
         %31 = OpCompositeExtract %float %30 0
         %32 = OpCompositeExtract %float %30 1
         %33 = OpCompositeConstruct %v4float %31 %32 %float_0 %float_1
               OpStore %gl_Position %33
               OpStore %out_var_TEXCOORD0 %28
               OpReturn
               OpFunctionEnd

#endif

const unsigned char g_VSMain[] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x56, 0x53, 0x4d, 0x61, 0x69,
    0x6e, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6f, 0x75, 0x74,
    0x2e, 0x76, 0x61, 0x72, 0x2e, 0x54, 0x45, 0x58, 0x43, 0x4f, 0x4f, 0x52, 0x44, 0x30, 0x00, 0x00, 0x00, 0x05, 0x00,
    0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x56, 0x53, 0x4d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b,
    0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x40, 0x2b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0xbf, 0x17, 0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x2c,
    0x00, 0x05, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
    0x2b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04,
    0x00, 0x08, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x20, 0x00, 0x04, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00,
    0x00, 0x13, 0x00, 0x02, 0x00, 0x13, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x14, 0x00, 0x00, 0x00, 0x13, 0x00,
    0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b,
    0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00,
    0x12, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x13, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x15, 0x00,
    0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xc4,
    0x00, 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
    0xc7, 0x00, 0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
    0x00, 0x70, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xc7, 0x00,
    0x05, 0x00, 0x05, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x70,
    0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x50, 0x00, 0x05, 0x00,
    0x0b, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x8e, 0x00, 0x05,
    0x00, 0x0b, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x81, 0x00,
    0x05, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x51,
    0x00, 0x05, 0x00, 0x08, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x51, 0x00, 0x05, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x50, 0x00, 0x07, 0x00, 0x10, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x21,
    0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00,
    0x38, 0x00, 0x01, 0x00
};

#if 0
; SPIR-V
; Version: 1.0
; Generator: Google spiregg; 0
; Bound: 27
; Schema: 0
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint Fragment %PSMainColor "PSMainColor" %gl_FragCoord %in_var_TEXCOORD0 %out_var_SV_Target0
               OpExecutionMode %PSMainColor OriginUpperLeft
               OpSource HLSL 600
               OpName %type_2d_image "type.2d.image"
               OpName %copy_texture "copy_texture"
               OpName %type_sampler "type.sampler"
               OpName %copy_sampler "copy_sampler"
               OpName %in_var_TEXCOORD0 "in.var.TEXCOORD0"
               OpName %out_var_SV_Target0 "out.var.SV_Target0"
               OpName %PSMainColor "PSMainColor"
               OpName %type_sampled_image "type.sampled.image"
               OpDecorate %gl_FragCoord BuiltIn FragCoord
               OpDecorate %in_var_TEXCOORD0 Location 0
               OpDecorate %out_var_SV_Target0 Location 0
               OpDecorate %copy_texture DescriptorSet 0
               OpDecorate %copy_texture Binding 0
               OpDecorate %copy_sampler DescriptorSet 0
               OpDecorate %copy_sampler Binding 1
      %float = OpTypeFloat 32
    %float_0 = OpConstant %float 0
%type_2d_image = OpTypeImage %float 2D 2 0 0 1 Unknown
%_ptr_UniformConstant_type_2d_image = OpTypePointer UniformConstant %type_2d_image
%type_sampler = OpTypeSampler
%_ptr_UniformConstant_type_sampler = OpTypePointer UniformConstant %type_sampler
    %v4float = OpTypeVector %float 4
%_ptr_Input_v4float = OpTypePointer Input %v4float
    %v2float = OpTypeVector %float 2
%_ptr_Input_v2float = OpTypePointer Input %v2float
%_ptr_Output_v4float = OpTypePointer Output %v4float
       %void = OpTypeVoid
         %20 = OpTypeFunction %void
%type_sampled_image = OpTypeSampledImage %type_2d_image
%copy_texture = OpVariable %_ptr_UniformConstant_type_2d_image UniformConstant
%copy_sampler = OpVariable %_ptr_UniformConstant_type_sampler UniformConstant
%gl_FragCoord = OpVariable %_ptr_Input_v4float Input
%in_var_TEXCOORD0 = OpVariable %_ptr_Input_v2float Input
%out_var_SV_Target0 = OpVariable %_ptr_Output_v4float Output
%PSMainColor = OpFunction %void None %20
         %21 = OpLabel
         %22 = OpLoad %v2float %in_var_TEXCOORD0
         %23 = OpLoad %type_2d_image %copy_texture
         %24 = OpLoad %type_sampler %copy_sampler
         %25 = OpSampledImage %type_sampled_image %23 %24
         %26 = OpImageSampleExplicitLod %v4float %25 %22 Lod %float_0
               OpStore %out_var_SV_Target0 %26
               OpReturn
               OpFunctionEnd

#endif

const unsigned char g_PSMainColor[] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x09, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x53, 0x4d, 0x61, 0x69,
    0x6e, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00,
    0x00, 0x58, 0x02, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x05, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x32,
    0x64, 0x2e, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x63,
    0x6f, 0x70, 0x79, 0x5f, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x00, 0x06, 0x00, 0x08, 0x00, 0x00, 0x00, 0x63, 0x6f, 0x70, 0x79, 0x5f, 0x73, 0x61, 0x6d, 0x70, 0x6c,
    0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x2e, 0x76, 0x61,
    0x72, 0x2e, 0x54, 0x45, 0x58, 0x43, 0x4f, 0x4f, 0x52, 0x44, 0x30, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x6f, 0x75, 0x74, 0x2e, 0x76, 0x61, 0x72, 0x2e, 0x53, 0x56, 0x5f, 0x54, 0x61, 0x72, 0x67,
    0x65, 0x74, 0x30, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x53, 0x4d, 0x61, 0x69, 0x6e,
    0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x05, 0x00, 0x07, 0x00, 0x09, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e,
    0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x64, 0x2e, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x47, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04,
    0x00, 0x08, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x0a, 0x00,
    0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x20, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x1a, 0x00,
    0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04,
    0x00, 0x10, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x11, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x13, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00,
    0x14, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
    0x00, 0x3b, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00,
    0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0f,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x13, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x15, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00,
    0x17, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
    0x00, 0x08, 0x00, 0x00, 0x00, 0x56, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x17, 0x00,
    0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x58, 0x00, 0x07, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x19,
    0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00
};

#if 0
; SPIR-V
; Version: 1.0
; Generator: Google spiregg; 0
; Bound: 28
; Schema: 0
               OpCapability Shader
               OpMemoryModel Logical GLSL450
               OpEntryPoint Fragment %PSMainDepth "PSMainDepth" %gl_FragCoord %in_var_TEXCOORD0 %gl_FragDepth
               OpExecutionMode %PSMainDepth OriginUpperLeft
               OpExecutionMode %PSMainDepth DepthReplacing
               OpSource HLSL 600
               OpName %type_2d_image "type.2d.image"
               OpName %copy_texture "copy_texture"
               OpName %type_sampler "type.sampler"
               OpName %copy_sampler "copy_sampler"
               OpName %in_var_TEXCOORD0 "in.var.TEXCOORD0"
               OpName %PSMainDepth "PSMainDepth"
               OpName %type_sampled_image "type.sampled.image"
               OpDecorate %gl_FragCoord BuiltIn FragCoord
               OpDecorate %gl_FragDepth BuiltIn FragDepth
               OpDecorate %in_var_TEXCOORD0 Location 0
               OpDecorate %copy_texture DescriptorSet 0
               OpDecorate %copy_texture Binding 0
               OpDecorate %copy_sampler DescriptorSet 0
               OpDecorate %copy_sampler Binding 1
      %float = OpTypeFloat 32
    %float_0 = OpConstant %float 0
%type_2d_image = OpTypeImage %float 2D 2 0 0 1 Unknown
%_ptr_UniformConstant_type_2d_image = OpTypePointer UniformConstant %type_2d_image
%type_sampler = OpTypeSampler
%_ptr_UniformConstant_type_sampler = OpTypePointer UniformConstant %type_sampler
    %v4float = OpTypeVector %float 4
%_ptr_Input_v4float = OpTypePointer Input %v4float
    %v2float = OpTypeVector %float 2
%_ptr_Input_v2float = OpTypePointer Input %v2float
%_ptr_Output_float = OpTypePointer Output %float
       %void = OpTypeVoid
         %20 = OpTypeFunction %void
%type_sampled_image = OpTypeSampledImage %type_2d_image
%copy_texture = OpVariable %_ptr_UniformConstant_type_2d_image UniformConstant
%copy_sampler = OpVariable %_ptr_UniformConstant_type_sampler UniformConstant
%gl_FragCoord = OpVariable %_ptr_Input_v4float Input
%in_var_TEXCOORD0 = OpVariable %_ptr_Input_v2float Input
%gl_FragDepth = OpVariable %_ptr_Output_float Output
%PSMainDepth = OpFunction %void None %20
         %21 = OpLabel
         %22 = OpLoad %v2float %in_var_TEXCOORD0
         %23 = OpLoad %type_2d_image %copy_texture
         %24 = OpLoad %type_sampler %copy_sampler
         %25 = OpSampledImage %type_sampled_image %23 %24
         %26 = OpImageSampleExplicitLod %v4float %25 %22 Lod %float_0
         %27 = OpCompositeExtract %float %26 0
               OpStore %gl_FragDepth %27
               OpReturn
               OpFunctionEnd

#endif

const unsigned char g_PSMainDepth[] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x09, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x53, 0x4d, 0x61, 0x69,
    0x6e, 0x44, 0x65, 0x70, 0x74, 0x68, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x0c, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x05, 0x00,
    0x06, 0x00, 0x05, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x32, 0x64, 0x2e, 0x69, 0x6d, 0x61, 0x67, 0x65,
    0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x63, 0x6f, 0x70, 0x79, 0x5f, 0x74, 0x65, 0x78,
    0x74, 0x75, 0x72, 0x65, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70,
    0x65, 0x2e, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x08, 0x00,
    0x00, 0x00, 0x63, 0x6f, 0x70, 0x79, 0x5f, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x2e, 0x76, 0x61, 0x72, 0x2e, 0x54, 0x45, 0x58, 0x43, 0x4f,
    0x4f, 0x52, 0x44, 0x30, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x53, 0x4d,
    0x61, 0x69, 0x6e, 0x44, 0x65, 0x70, 0x74, 0x68, 0x00, 0x05, 0x00, 0x07, 0x00, 0x09, 0x00, 0x00, 0x00, 0x74, 0x79,
    0x70, 0x65, 0x2e, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x64, 0x2e, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x00, 0x00, 0x47,
    0x00, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x03, 0x00, 0x00,
    0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x22, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x47, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03,
    0x00, 0x0a, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
    0x00, 0x1a, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
    0x17, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04,
    0x00, 0x11, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x12, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x13, 0x00, 0x00, 0x00, 0x21,
    0x00, 0x03, 0x00, 0x14, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x3b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00,
    0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x11,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x12, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x13, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x15, 0x00, 0x00, 0x00, 0x3d, 0x00,
    0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x18, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x56, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00,
    0x00, 0x17, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x58, 0x00, 0x07, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x1a, 0x00,
    0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x51,
    0x00, 0x05, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3e, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01,
    0x00
};

#if 0
; SPIR-V
; Version: 1.0
; Generator: Google spiregg; 0
; Bound: 28
; Schema: 0
               OpCapability Shader
               OpCapability StencilExportEXT
               OpExtension "SPV_EXT_shader_stencil_export"
               OpMemoryModel Logical GLSL450
               OpEntryPoint Fragment %PSMainStencil "PSMainStencil" %gl_FragCoord %in_var_TEXCOORD0 %4
               OpExecutionMode %PSMainStencil OriginUpperLeft
               OpSource HLSL 600
               OpName %type_2d_image "type.2d.image"
               OpName %copy_texture "copy_texture"
               OpName %type_sampler "type.sampler"
               OpName %copy_sampler "copy_sampler"
               OpName %in_var_TEXCOORD0 "in.var.TEXCOORD0"
               OpName %PSMainStencil "PSMainStencil"
               OpName %type_sampled_image "type.sampled.image"
               OpDecorate %gl_FragCoord BuiltIn FragCoord
               OpDecorate %4 BuiltIn FragStencilRefEXT
               OpDecorate %in_var_TEXCOORD0 Location 0
               OpDecorate %copy_texture DescriptorSet 0
               OpDecorate %copy_texture Binding 0
               OpDecorate %copy_sampler DescriptorSet 0
               OpDecorate %copy_sampler Binding 1
      %float = OpTypeFloat 32
    %float_0 = OpConstant %float 0
%type_2d_image = OpTypeImage %float 2D 2 0 0 1 Unknown
%_ptr_UniformConstant_type_2d_image = OpTypePointer UniformConstant %type_2d_image
%type_sampler = OpTypeSampler
%_ptr_UniformConstant_type_sampler = OpTypePointer UniformConstant %type_sampler
    %v4float = OpTypeVector %float 4
%_ptr_Input_v4float = OpTypePointer Input %v4float
    %v2float = OpTypeVector %float 2
%_ptr_Input_v2float = OpTypePointer Input %v2float
%_ptr_Output_float = OpTypePointer Output %float
       %void = OpTypeVoid
         %20 = OpTypeFunction %void
%type_sampled_image = OpTypeSampledImage %type_2d_image
%copy_texture = OpVariable %_ptr_UniformConstant_type_2d_image UniformConstant
%copy_sampler = OpVariable %_ptr_UniformConstant_type_sampler UniformConstant
%gl_FragCoord = OpVariable %_ptr_Input_v4float Input
%in_var_TEXCOORD0 = OpVariable %_ptr_Input_v2float Input
          %4 = OpVariable %_ptr_Output_float Output
%PSMainStencil = OpFunction %void None %20
         %21 = OpLabel
         %22 = OpLoad %v2float %in_var_TEXCOORD0
         %23 = OpLoad %type_2d_image %copy_texture
         %24 = OpLoad %type_sampler %copy_sampler
         %25 = OpSampledImage %type_sampled_image %23 %24
         %26 = OpImageSampleExplicitLod %v4float %25 %22 Lod %float_0
         %27 = OpCompositeExtract %float %26 0
               OpStore %4 %27
               OpReturn
               OpFunctionEnd

#endif

const unsigned char g_PSMainStencil[] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x95, 0x13, 0x00, 0x00, 0x0a, 0x00,
    0x09, 0x00, 0x53, 0x50, 0x56, 0x5f, 0x45, 0x58, 0x54, 0x5f, 0x73, 0x68, 0x61, 0x64, 0x65, 0x72, 0x5f, 0x73, 0x74,
    0x65, 0x6e, 0x63, 0x69, 0x6c, 0x5f, 0x65, 0x78, 0x70, 0x6f, 0x72, 0x74, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0a, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x50, 0x53, 0x4d, 0x61, 0x69, 0x6e, 0x53, 0x74, 0x65, 0x6e, 0x63, 0x69, 0x6c, 0x00, 0x00, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e, 0x32, 0x64, 0x2e, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x00, 0x00,
    0x00, 0x05, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x63, 0x6f, 0x70, 0x79, 0x5f, 0x74, 0x65, 0x78, 0x74, 0x75,
    0x72, 0x65, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x00, 0x00, 0x74, 0x79, 0x70, 0x65, 0x2e,
    0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x63, 0x6f, 0x70, 0x79, 0x5f, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x07,
    0x00, 0x03, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x2e, 0x76, 0x61, 0x72, 0x2e, 0x54, 0x45, 0x58, 0x43, 0x4f, 0x4f, 0x52,
    0x44, 0x30, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x53, 0x4d, 0x61, 0x69,
    0x6e, 0x53, 0x74, 0x65, 0x6e, 0x63, 0x69, 0x6c, 0x00, 0x00, 0x00, 0x05, 0x00, 0x07, 0x00, 0x09, 0x00, 0x00, 0x00,
    0x74, 0x79, 0x70, 0x65, 0x2e, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x64, 0x2e, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x00,
    0x00, 0x47, 0x00, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x47, 0x00,
    0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x96, 0x13, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x16,
    0x00, 0x03, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00,
    0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00, 0x05, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
    0x00, 0x00, 0x00, 0x1a, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x00,
    0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20,
    0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
    0x12, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x13, 0x00, 0x00,
    0x00, 0x21, 0x00, 0x03, 0x00, 0x14, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x03, 0x00, 0x09, 0x00,
    0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3b, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04,
    0x00, 0x11, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x12, 0x00,
    0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x13, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x15, 0x00, 0x00, 0x00,
    0x3d, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04,
    0x00, 0x05, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00,
    0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x56, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x19,
    0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x58, 0x00, 0x07, 0x00, 0x0e, 0x00, 0x00, 0x00,
    0x1a, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00,
    0x00, 0x51, 0x00, 0x05, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38,
    0x00, 0x01, 0x00
};

#endif // GFXRECON_DECODE_VULKAN_COPY_SHADERS_H
