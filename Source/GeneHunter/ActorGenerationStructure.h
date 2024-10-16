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

    // 해당 Mesh의 region을 나타내는 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "informations")
    int32 Region;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "informations")
    float Frequency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "informations")
    FString Category;

    // 기본 생성자
    FActorGenerationStructure()
        : Mesh(nullptr), Region(0), Frequency(0.0f), Category(TEXT("None"))
    {}
};