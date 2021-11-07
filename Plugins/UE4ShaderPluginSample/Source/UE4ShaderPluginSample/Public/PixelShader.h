// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shader.h"
#include "GlobalShader.h"
/**
 * 
 */
class UE4SHADERPLUGINSAMPLE_API FPixelShader : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FPixelShader);

	//SHADER_USE_PARAMETER_STRUCT(FPixelShader, FGlobalShader);

	//BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
	//	SHADER_PARAMETER_TEXTURE(Texture2D<uint>, ComputeShaderOutput)
	//	SHADER_PARAMETER(FVector4, StartColor)
	//	SHADER_PARAMETER(FVector4, EndColor)
	//	SHADER_PARAMETER(FVector2D, TextureSize) // Metal doesn't support GetDimensions(), so we send in this data via our parameters.
	//	SHADER_PARAMETER(float, BlendFactor)
	//END_SHADER_PARAMETER_STRUCT()
public:
	FPixelShader() = default;
	~FPixelShader() = default;

	FPixelShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer) { }

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
};
