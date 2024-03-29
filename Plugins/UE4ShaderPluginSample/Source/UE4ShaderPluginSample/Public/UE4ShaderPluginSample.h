// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "RenderGraphResources.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"


struct FShaderUsageExampleParameters
{
	UTextureRenderTarget2D* RenderTarget;

	FIntPoint GetRenderTargetSize() const
	{
		return CachedRenderTargetSize;
	}

	FShaderUsageExampleParameters() { }
	FShaderUsageExampleParameters(UTextureRenderTarget2D* InRenderTarget)
		: RenderTarget(InRenderTarget)
	{
		CachedRenderTargetSize = RenderTarget ? FIntPoint(RenderTarget->SizeX, RenderTarget->SizeY) : FIntPoint::ZeroValue;
	}

private:
	FIntPoint CachedRenderTargetSize;
};

class FUE4ShaderPluginSampleModule : public IModuleInterface
{
public:

	static inline FUE4ShaderPluginSampleModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FUE4ShaderPluginSampleModule>("UE4ShaderPluginSample");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("UE4ShaderPluginSample");
	}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void BeginRendering();
	void EndRendering();

	void UpdateParameters(FShaderUsageExampleParameters& DrawParameters);

public:
	TRefCountPtr<IPooledRenderTarget> ComputeShaderOutput;
	FShaderUsageExampleParameters CachedShaderUsageExampleParameters;
	FDelegateHandle OnPostResolvedSceneColorHandle;
	FCriticalSection RenderEveryFrameLock;
	volatile bool bCachedParametersValid;

	void PostResolveSceneColor_RenderThread(FRHICommandListImmediate& RHICmdList, class FSceneRenderTargets& SceneContext);
	void Draw_RenderThread(const FShaderUsageExampleParameters& DrawParameters);
};
