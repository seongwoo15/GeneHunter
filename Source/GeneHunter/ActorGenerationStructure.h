// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "ActorGenerationStructure.generated.h"

USTRUCT(BlueprintType)

struct FActorGenerationStructure
{
    GENERATED_BODY()

public:
    // Static Mesh를 참조하는 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "informations")
    UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "informations")
    float Frequency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "informations")
    float Depth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "informations")
    FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "informations")
    float Size;

    // 기본 생성자
    FActorGenerationStructure()
        : Mesh(nullptr), Frequency(0.0f), Depth(0.0f), Rotation(0.0f,0.0f,0.0f) , Size(1.0f)
    {}
};