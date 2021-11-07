// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shader.h"
#include "GlobalShader.h"

/**
 * 
 */
class UE4SHADERPLUGINSAMPLE_API FVertexShader : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FVertexShader);

public:
	FVertexShader() = default;

	FVertexShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer) { }

	~FVertexShader() = default;
};
