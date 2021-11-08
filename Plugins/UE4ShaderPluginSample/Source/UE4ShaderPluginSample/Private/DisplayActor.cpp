// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayActor.h"

#include "UE4ShaderPluginSample.h"

// Sets default values
ADisplayActor::ADisplayActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UStaticMeshComponent* Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	Cube->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube"));

	if (CubeAsset.Succeeded())
	{
		Cube->SetStaticMesh(CubeAsset.Object);
		Cube->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		Cube->SetWorldScale3D(FVector(1.f));
	}

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> DrawRenderTarget(TEXT("/UE4ShaderPluginSample/RenderTarget"));
	this->RenderTarget = DrawRenderTarget.Object;

	ConstructorHelpers::FObjectFinder<UMaterial> DrawMaterial(TEXT("/UE4ShaderPluginSample/DisplayMaterial"));
	Cube->SetMaterial(0, DrawMaterial.Object);
}

// Called when the game starts or when spawned
void ADisplayActor::BeginPlay()
{
	Super::BeginPlay();
	FUE4ShaderPluginSampleModule::Get().BeginRendering();
}

void ADisplayActor::BeginDestroy()
{
	Super::BeginDestroy();
	FUE4ShaderPluginSampleModule::Get().EndRendering();
}

// Called every frame
void ADisplayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FShaderUsageExampleParameters DrawParameters(RenderTarget);
	FUE4ShaderPluginSampleModule::Get().UpdateParameters(DrawParameters);
}

