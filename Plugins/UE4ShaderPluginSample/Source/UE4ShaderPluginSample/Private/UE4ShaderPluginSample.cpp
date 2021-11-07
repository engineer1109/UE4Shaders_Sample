// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE4ShaderPluginSample.h"
#include "Interfaces/IPluginManager.h"
#include "RenderTargetPool.h"
#include "RenderPass.h"

#include <iostream>
#include <string>

#define LOCTEXT_NAMESPACE "FUE4ShaderPluginSampleModule"

void FUE4ShaderPluginSampleModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	OnPostResolvedSceneColorHandle.Reset();
	bCachedParametersValid = false;

	// Maps virtual shader source directory to the plugin's actual shaders directory.
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("UE4ShaderPluginSample"))->GetBaseDir(), TEXT("Shaders"));

	AddShaderSourceDirectoryMapping(TEXT("/SimpleShaders"), PluginShaderDir);
}

void FUE4ShaderPluginSampleModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	EndRendering();
}

void FUE4ShaderPluginSampleModule::EndRendering()
{
	if (!OnPostResolvedSceneColorHandle.IsValid())
	{
		return;
	}

	const FName RendererModuleName("Renderer");
	IRendererModule* RendererModule = FModuleManager::GetModulePtr<IRendererModule>(RendererModuleName);
	if (RendererModule)
	{
		RendererModule->GetResolvedSceneColorCallbacks().Remove(OnPostResolvedSceneColorHandle);
	}

	OnPostResolvedSceneColorHandle.Reset();
}

void FUE4ShaderPluginSampleModule::PostResolveSceneColor_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneRenderTargets& SceneContext)
{
	if (!bCachedParametersValid)
	{
		return;
	}

	// Depending on your data, you might not have to lock here, just added this code to show how you can do it if you have to.
	RenderEveryFrameLock.Lock();
	FShaderUsageExampleParameters Copy = CachedShaderUsageExampleParameters;
	RenderEveryFrameLock.Unlock();

	Draw_RenderThread(Copy);
}

void FUE4ShaderPluginSampleModule::Draw_RenderThread(const FShaderUsageExampleParameters& DrawParameters)
{
	check(IsInRenderingThread());

	if (!DrawParameters.RenderTarget)
	{
		return;
	}

	FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();

	QUICK_SCOPE_CYCLE_COUNTER(STAT_ShaderPlugin_Render); // Used to gather CPU profiling data for the UE4 session frontend
	SCOPED_DRAW_EVENT(RHICmdList, ShaderPlugin_Render); // Used to profile GPU activity and add metadata to be consumed by for example RenderDoc

	if (!ComputeShaderOutput.IsValid())
	{
		FPooledRenderTargetDesc ComputeShaderOutputDesc(FPooledRenderTargetDesc::Create2DDesc(DrawParameters.GetRenderTargetSize(), PF_R32_UINT, FClearValueBinding::None, TexCreate_None, TexCreate_ShaderResource | TexCreate_UAV, false));
		ComputeShaderOutputDesc.DebugName = TEXT("ShaderPlugin_ComputeShaderOutput");
		GRenderTargetPool.FindFreeElement(RHICmdList, ComputeShaderOutputDesc, ComputeShaderOutput, TEXT("ShaderPlugin_ComputeShaderOutput"));
	}

	
	FRenderPass::DrawToRenderTarget_RenderThread(RHICmdList, DrawParameters);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE4ShaderPluginSampleModule, UE4ShaderPluginSample)