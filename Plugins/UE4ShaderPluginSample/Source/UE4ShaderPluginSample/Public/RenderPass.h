// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE4ShaderPluginSample.h"
/**
 * 
 */
class UE4SHADERPLUGINSAMPLE_API FRenderPass
{
public:
	FRenderPass() = default;
	~FRenderPass() = default;

public:
	static void DrawToRenderTarget_RenderThread(FRHICommandListImmediate& RHICmdList, const FShaderUsageExampleParameters& DrawParameters);
};
